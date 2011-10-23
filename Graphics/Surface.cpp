#include "Surface.h"
#include <cmath>

namespace nV {
namespace Graphics {
Surface::Surface(F2P* f3d, double xstart, double xend, double ystart, double yend) {
    this->f = f3d;
    xmin = xstart;
    xmax = xend;
    ymin = ystart;
    ymax = yend;
    spx = xmax - xmin;
    spy = ymax - ymin;
    dx = spx / LARGE;
    dy = spy / LARGE;
    //makeplotrange(f3d, zmin, zmax, xmin, xmax, ymin, ymax);
    initsurface();
}

void Surface::initsurface() {
    nx = getNx();
    ny = getNy();
    double *x = new double[nx*ny];
    double *y = new double[nx*ny];
    double *z = new double[nx*ny];
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            x[i*nx+j] = xmin + (xmax - xmin) * (double)j / (double)(nx - 1);
        }
    }
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            y[i*nx+j] = ymin + (ymax - ymin) * (double)i / (double)(ny - 1);
        }
    }
    f->getArrayData(x, y, z, nx*ny);
    for (int i = 0; i < nx*ny; i++) {
        vert.add(new Point3d(x[i], y[i], z[i]));
        double d_temp_x = f->getDerivativeX(x[i], y[i], dx);
        double d_temp_y = f->getDerivativeY(x[i], y[i], dy);
        vert_derivate.push_back(vert_d(d_temp_x, d_temp_y));
    }
    xline = new IndexStripLineX*[ny];
    yline = new IndexStripLineY*[nx];
    for (int i = 0; i < ny; i++) {
        xline[i] = new IndexStripLineX(&vert);
    }
    for (int i = 0; i < nx; i++) {
        yline[i] = new IndexStripLineY(&vert);
    }
    sur.resize(0);
    int count = 0;
    for (int y = 0; y < ny - 1; y++) {
        for (int x = 0; x < nx - 1; x++) {
            int v = nx * y + x;
            int a = v;
            int b = v + 1;
            v += nx;
            int c = v;
            int d = v + 1;
            sur.add(new IndexPolygonForSurface(a, b, c, d, y + 1, x + 1, y == 0, x == 0));
            xline[y+1]->add(d);
            yline[x+1]->add(d);
            if (y == 0) {
                xline[0]->add(b);
                yline[x+1]->add(b);
            }
            if (x == 0) {
                yline[0]->add(c);
                xline[y+1]->add(c);
            }
            if (x == 0 && y == 0) {
                xline[0]->add(a);
                yline[0]->add(a);
            }
        }
    }
    
    //double d_x = f->getDerivativeX(vert[0]->x, vert[0]->y, dx);
    //double d_y = f->getDerivativeY(vert[0]->x, vert[0]->y, dy);

    //zmin = vert[0]->z;
    //zmax = vert[0]->z;
    zmin = zmax = 0.0;
    uint i = 0;
    for (; i < nx * ny; ++i) {
        if (!isNaN(vert[i]->z)) {
            zmin = zmax = vert[i]->z;
            break;
        }
    }
    for (; i < nx*ny; i++) {
        if (!isNaN(vert[i]->z)) {
            if (vert[i]->z > zmax) zmax = vert[i]->z;
            if (vert[i]->z < zmin) zmin = vert[i]->z;
        }
    }
//    zmax = 5;
//    zmin = -5;
    spz = zmax - zmin;
    graphScalex = spz / spx;
    graphScaley = spz / spy;
    if (zmin != zmax) {
        makeplotrange(f, zmax, zmin, dx, dy, vert, vert_derivate);
    }
    delete []x;
    delete []y;
    delete []z;
    //unsigned int surnum = sur.size();
    //for (unsigned int i = 0; i < surnum; i++) {
    for (uint i = 0; i < sur.size(); ++i) {
        trySplitPolygonX(i, 1);
    }
    //double surnum = sur.size();
    for (unsigned int i = 0; i < sur.size(); i++) {
        trySplitPolygonY(i, 1);
    }
    
    adjustmesh();
}

Surface::~Surface() {
    for (int i = 0; i < ny; i++) {
        delete xline[i];
    }
    for (int i = 0; i < nx; i++) {
        delete yline[i];
    }
    delete xline;
    delete yline;
}

int Surface::getNx() {
    return 20;
    //return 3;
}

int Surface::getNy() {
    return 20;
    //return 3;
}

void Surface::adjustmesh() {
    meshx.clear();
    meshy.clear();

    bool state = false;
    for (uint i = 0; i < ny; ++i) {
        std::vector< std::vector<int> >::iterator temp;
        state = false;
        for (uint j = 0; j < xline[i]->p.size(); ++j) {
            if (vert[xline[i]->p[j]]->z <= zmax && vert[xline[i]->p[j]]->z >= zmin &&
                std::abs(vert_derivate[xline[i]->p[j]].vert_d_x / graphScalex) < MAX_D &&
                std::abs(vert_derivate[xline[i]->p[j]].vert_d_y / graphScaley) < MAX_D) {
                if (state) {
                    (*temp).push_back(xline[i]->p[j]);
                } else {
                    meshx.push_back(std::vector<int>());
                    temp = meshx.end() - 1;
                    //temp.clear();
                    (*temp).push_back(xline[i]->p[j]);
                    state = true;
                }
            } else {
                if (state) {
                    if ((*temp).size() < 2) {
                        meshx.pop_back();
                    }
                    //meshx.push_back(temp);
                    //.clear();
                    state = false;
                } else {
                }
            }
        }
        if (meshx.size() > 0) {
            if ((*(meshx.end() - 1)).size() < 2) {
                meshx.pop_back();
            }
        }
    }

    state = false;
    for (uint i = 0; i < nx; ++i) {
        std::vector< std::vector<int> >::iterator temp;
        state = false;
        for (uint j = 0; j < yline[i]->p.size(); ++j) {
            if (vert[yline[i]->p[j]]->z <= zmax && vert[yline[i]->p[j]]->z >= zmin &&
                std::abs(vert_derivate[yline[i]->p[j]].vert_d_x / graphScalex) < MAX_D &&
                std::abs(vert_derivate[yline[i]->p[j]].vert_d_y / graphScaley) < MAX_D) {
                if (state) {
                    (*temp).push_back(yline[i]->p[j]);
                } else {
                    meshy.push_back(std::vector<int>());
                    temp = meshy.end() - 1;
                    //temp.clear();
                    (*temp).push_back(yline[i]->p[j]);
                    state = true;
                }
            } else {
                if (state) {
                    if ((*temp).size() < 2) {
                        meshy.pop_back();
                    }
                    //meshy.push_back(temp);
                    //temp.clear();
                    state = false;
                } else {
                }
            }
        }

        if (meshy.size() > 0) {
            if ((*(meshy.end() - 1)).size() < 2) {
                meshy.pop_back();
            }
        }
    }
}
void Surface::makeplotrange(F2P *f3d_f, double &zmin_f, double &zmax_f, double dx_f, double dy_f,const ArrayList<Point3d*> &vert_f, const std::vector<vert_d> &vert_d_f) {
    //std::vector<double> vdx_f;
    //std::vector<double> vdy_f;
    //for (uint i = 0; i < vert_f.size(); ++i) {
        //vdx_f.push_back(f3d_f->getDerivativeX(vert_f[i]->x, vert_f[i]->y, dx_f));
        //vdy_f.push_back(f3d_f->getDerivativeY(vert_f[i]->x, vert_f[i]->y, dy_f));
        //vdx_f.push_back(vert_derivate[i].vert_d_x);
        //vdx_f.push_back(vert_derivate[i].vert_d_y);
    //}

    bool vrm = true;
    bool isnanfunction = false;
    std::vector<bool> flag(vert_f.size(), false); //if true, the vert's derivative is too big
    std::vector<bool> nanflag(vert_f.size(), false);  //if true, the vert's value is NaN
    int count = 0;
    while (vrm) {
        vrm = false;
        for (unsigned int i = 0; i < vert_f.size(); i++) {
            if(isNaN(vert_d_f[i].vert_d_x) || isNaN(vert_d_f[i].vert_d_y)) {
                flag[i] = true;
                nanflag[i] = true;
                isnanfunction = true;
            } else {
                if(!flag[i]) {
                    if(graphScalex != 0 && graphScaley != 0) {
                        if (std::abs(vert_d_f[i].vert_d_x) / graphScalex > MAX_D || std::abs(vert_d_f[i].vert_d_y) / graphScaley > MAX_D) {
                            //vert.remove(i);
                            //d.remove(i);
                            //singularity = true;
                            flag[i] = true;
                            vrm = true;
                        }
                    }
                }
            }
        }
        uint i = 0;
        for (; i < vert_f.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                zmin = vert_f[i]->z;
                zmax = vert_f[i]->z;
                break;
            }
        }
        for (; i < vert_f.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                if (vert_f[i]->z > zmax) zmax = vert_f[i]->z;
                if (vert_f[i]->z < zmin) zmin = vert_f[i]->z;
            }
        }
        spz = zmax - zmin;
        graphScalex = spz / spx;
        graphScaley = spz / spy;
        if(!(graphScalex > MIN_GRAPHSCALE_MINIMUM && graphScaley > MIN_GRAPHSCALE_MINIMUM)) {
            break;
        }
        if(count >= MAX_DELETE_AMOUNT) {
            break;
        }
        count++;
    }
}

void Surface::trySplitPolygonX(unsigned int id, int depth) {
    if (depth > MAX_SPLIT_DEPTH)
        return;
    if (!sur[id]->isshow) return;
    Point3d* p1 = vert[sur[id]->p11];
    Point3d* p2 = vert[sur[id]->p12];
    Point3d* p3 = vert[sur[id]->p21];
    Point3d* p4 = vert[sur[id]->p22];
    //double netk1 = f->getDerivativeX(p1->x, p1->y, dx) / graphScalex;
    //double netk2 = f->getDerivativeX(p2->x, p2->y, -dx) / graphScalex;
    //double netk3 = f->getDerivativeX(p3->x, p3->y, dx) / graphScalex;
    //double netk4 = f->getDerivativeX(p4->x, p4->y, -dx) /graphScalex;
    double netk1 = vert_derivate[sur[id]->p11].vert_d_x / graphScalex;
    double netk2 = vert_derivate[sur[id]->p12].vert_d_x / graphScalex;
    double netk3 = vert_derivate[sur[id]->p21].vert_d_x / graphScalex;
    double netk4 = vert_derivate[sur[id]->p22].vert_d_x / graphScalex;
    if (judgmentDX(id, std::abs(netk1), std::abs(netk2), std::abs(netk3), std::abs(netk4))) {
        return;
    }

    double dangle = std::acos((1 + netk1 * netk2) / std::sqrt(1 + netk1 * netk1) / std::sqrt(1 + netk2 * netk2));
    if (dangle > ADD_POINT_ANGLE) {	//need to add data
        double x = (p1->x + p2->x) / 2;
        double y = vert[sur[id]->p11]->y;
        double z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
        y = vert[sur[id]->p21]->y; //creat another point
        z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
        sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
        sur[id]->p12 = vert.size() - 2;
        sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
        if (sur[id]->xlineid != -1) {
            xline[sur[id]->xlineid]->add(vert.size() - 1);
        }
        if (sur[id]->x0line) {
            xline[0]->add(vert.size() - 2);
        }
        sur[id]->ylineid = -1;
        //if (z > zmax) zmax = z;
        //if (z < zmin) zmin = z;
        //trySplitPolygonX(sur.size() - 1, depth + 1); //continue split the two polygon
        trySplitPolygonX(id, depth + 1);
    } else { //try add data
        double x = (p1->x + p2->x) / 2;
        double y = p1->y;
        double netk3 = f->getDerivativeX(x, y, dx) / graphScalex;
        if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) { //有拐点
            double z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
            y = vert[sur[id]->p21]->y; //creat another point
            z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
            sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
            sur[id]->p12 = vert.size() - 2;
            sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
            if (sur[id]->xlineid != -1) {
                xline[sur[id]->xlineid]->add(vert.size() - 1);
            }
            if (sur[id]->x0line) {
                xline[0]->add(vert.size() - 2);
            }
            sur[id]->ylineid = -1;
            //if (z > zmax) zmax = z;
            //if (z < zmin) zmin = z;
            //trySplitPolygonX(sur.size() - 1, depth + 1); //continue split the two polygon
            trySplitPolygonX(id, depth + 1);
        }
    }
}
bool Surface::judgmentDX(unsigned int id, double netk1, double netk2, double netk3, double netk4) {
    while (true) {
        double dis1 = 0.0;
        double dis2 = 0.0;
        //if ((netk1 > MAX_D && netk2 > MAX_D) || (netk3 > MAX_D && netk4 > MAX_D)) {
        if (false) {
            sur[id]->isshow = false;
            return true;
        } else if (netk1 > MAX_D || netk2 > MAX_D) {
            if (sur[id]->times > MAX_SPLIT) {
                sur[id]->isshow = false;
                return true;
            }
            dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) +
                ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) +
                ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz));
            dis2 = std::sqrt(((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) +
                ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) +
                ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz));
            if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
                sur[id]->isshow = false;
                return true;
            }
            double border = (vert[sur[id]->p11]->x + vert[sur[id]->p12]->x) / 2.0;
            //double border2 = (vert[sur[id]->p21]->x + vert[sur[id]->p22]->x) / 2.0;
            if (!getborderx(id, border, vert[sur[id]->p11]->y, netk1, netk2, vert[sur[id]->p11]->x, vert[sur[id]->p12]->x, 5)) {
                sur[id]->isshow = false;
                return true;
            }

            //double netkmiddle2 = f->getDerivativeX(border, vert[sur[id]->p21]->y, dx) / graphScalex;

            double z = f->getSingleData(border, vert[sur[id]->p11]->y);
            vert.add(new Point3d(border, vert[sur[id]->p11]->y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(border, vert[sur[id]->p11]->y, dx), f->getDerivativeY(border, vert[sur[id]->p11]->y, dy)));
            double y = vert[sur[id]->p21]->y; //creat another point
            double z1 = f->getSingleData(border, y);
            vert.add(new Point3d(border, y, z1));
            vert_derivate.push_back(vert_d(f->getDerivativeX(border, y, dx), f->getDerivativeY(border, y, dy)));
            //uint position = netk1 > MAX_D ? sur[id]->p12 : sur[id]->p11;
            if (netk1 > MAX_D) {
                sur.add(new IndexPolygonForSurface(sur[id]->p11, vert.size() - 2, sur[id]->p21, vert.size() - 1, sur[id]->xlineid, sur[id]->ylineid, false , false, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p11 = vert.size() - 2;
                sur[id]->p21 = vert.size() - 1;
                sur[id]->times++;
            } else {
                sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p12 = vert.size() - 2;
                sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
                sur[id]->times++;
            }

            if (sur[id]->xlineid != -1) {
                xline[sur[id]->xlineid]->add(vert.size() - 1);
            }
            if (sur[id]->x0line) {
                xline[0]->add(vert.size() - 2);
            }
            sur[id]->ylineid = -1;

            if (!(std::abs((*(vert_derivate.end() - 1)).vert_d_x / graphScalex) < MAX_D)) {
                continue;
            } else {
                if (z > zmax) 
                    zmax = z;
                if (z < zmin) 
                    zmin = z;
                if (z1 > zmax) 
                    zmax = z1;
                if (z1 < zmin) 
                    zmin = z1;
                //spz = zmax - zmin;
            }
            //double dmiddle = f->getDerivativeX(border, vert[sur[id]->p11]->y, dx);
        } else if (netk3 > MAX_D || netk4 > MAX_D) {
            if (sur[id]->times > MAX_SPLIT) {
                sur[id]->isshow = false;
                return true;
            }
            dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) +
                ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) +
                ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz));
            dis2 = std::sqrt(((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) +
                ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) +
                ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz));
            if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
                sur[id]->isshow = false;
                return true;
            }
            double border = (vert[sur[id]->p21]->x + vert[sur[id]->p22]->x) / 2.0;
            //double border2 = (vert[sur[id]->p21]->x + vert[sur[id]->p22]->x) / 2.0;
            if (!getborderx(id, border, vert[sur[id]->p21]->y, netk1, netk2, vert[sur[id]->p11]->x, vert[sur[id]->p12]->x, 5)) {
                sur[id]->isshow = false;
                return true;
            }

            //double netkmiddle2 = f->getDerivativeX(border, vert[sur[id]->p11]->y, dx) / graphScalex;

            double z = f->getSingleData(border, vert[sur[id]->p11]->y);
            vert.add(new Point3d(border, vert[sur[id]->p11]->y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(border, vert[sur[id]->p11]->y, dx), f->getDerivativeY(border, vert[sur[id]->p11]->y, dy)));
            double y = vert[sur[id]->p21]->y; //creat another point
            double z1 = f->getSingleData(border, y);
            vert.add(new Point3d(border, y, z1));
            vert_derivate.push_back(vert_d(f->getDerivativeX(border, y, dx), f->getDerivativeY(border, y, dy)));
            //uint position = netk1 > MAX_D ? sur[id]->p12 : sur[id]->p11;
            if (netk3 > MAX_D) {
                sur.add(new IndexPolygonForSurface(sur[id]->p11, vert.size() - 2, sur[id]->p21, vert.size() - 1, sur[id]->xlineid, sur[id]->ylineid, false , false, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p11 = vert.size() - 2;
                sur[id]->p21 = vert.size() - 1;
                sur[id]->times++;
            } else {
                sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p12 = vert.size() - 2;
                sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
                sur[id]->times++;
            }

            if (sur[id]->xlineid != -1) {
                xline[sur[id]->xlineid]->add(vert.size() - 1);
            }
            if (sur[id]->x0line) {
                xline[0]->add(vert.size() - 2);
            }
            sur[id]->ylineid = -1;

            if (!(std::abs((*(vert_derivate.end() - 1)).vert_d_x / graphScalex) < MAX_D)) {
                continue;
            } else {
                if (z > zmax) 
                    zmax = z;
                if (z < zmin) 
                    zmin = z;
                if (z1 > zmax) 
                    zmax = z1;
                if (z1 < zmin) 
                    zmin = z1;
                //spz = zmax - zmin;
            }
        }
        dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p12]->x) / spx) +
            ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p12]->y) / spy) +
            ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p12]->z) / spz));
        dis2 = std::sqrt(((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p21]->x - vert[sur[id]->p22]->x) / spx) +
            ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p21]->y - vert[sur[id]->p22]->y) / spy) +
            ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p21]->z - vert[sur[id]->p22]->z) / spz));
        if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
            return true;
        }
        return false;
        //++id;
    }
    return true;
}

bool Surface::getborderx(uint id, double &border, double y, double netk1, double netk2, double x1, double x2, int depth) {
    double netkmiddle = f->getDerivativeX(border, y, dx) / graphScalex;
    if (depth <= 0) {
        if (std::abs(netkmiddle) > MAX_D) {
            if (std::abs(netk1) > MAX_D) {
                if (border == x2) {
                    return false;
                } else {
                    border = x2;
                    return true;
                }
            } else {
                if (border == x1) {
                    return false;
                } else {
                    border = x1;
                    return true;
                }
            }
        } else {
            return true;
        }
    }

    //if (depth <= 0) {
    //    if (netkmiddle > MAX_D) {
    //        sur[id]->isshow = false;
    //        return false;
    //    } else {
    //        return true;
    //    }
    //}
    if (std::abs(netkmiddle) > MAX_D) {
        if (std::abs(netk1) > MAX_D) {
            double border_old = border;
            border = (x2 + border) / 2.0;
            return getborderx(id, border, y,netkmiddle, netk2, border_old, x2, --depth);
        } else {
            double border_old = border;
            border = (x1 + border) / 2.0;
            return getborderx(id, border, y, netk1, netkmiddle, x1, border_old, --depth);
        }
    } else {
        if (std::abs(netk1) > MAX_D) {
            double border_old = border;
            border = (x1 + border) / 2.0;
            return getborderx(id, border, y, netk1, netkmiddle, x1, border_old, --depth);
        } else {
            double border_old = border;
            border = (x2 + border) / 2.0;
            return getborderx(id, border, y, netkmiddle, netk2, border_old, x2, --depth);
        }
    }
}
void Surface::trySplitPolygonY(unsigned int id, int depth) {
    if (depth > MAX_SPLIT_DEPTH)
        return;
    if (!sur[id]->isshow) return;
    Point3d* p1 = vert[sur[id]->p11];
    Point3d* p2 = vert[sur[id]->p21];
    Point3d* p3 = vert[sur[id]->p12];
    Point3d *p4 = vert[sur[id]->p22];
    //double netk1 = f->getDerivativeY(p1->x, p1->y, dy) / graphScaley;
    //double netk2 = f->getDerivativeY(p2->x, p2->y, -dy) / graphScaley;
    //double netk3 = f->getDerivativeY(p3->x, p3->y, dy) / graphScaley;
    //double netk4 = f->getDerivativeY(p4->x, p4->y, -dy) / graphScaley;
    double netk1 = vert_derivate[sur[id]->p11].vert_d_y / graphScaley;
    double netk2 = vert_derivate[sur[id]->p21].vert_d_y / graphScaley;
    double netk3 = vert_derivate[sur[id]->p12].vert_d_y / graphScaley;
    double netk4 = vert_derivate[sur[id]->p22].vert_d_y / graphScaley;
    if (judgmentDY(id, std::abs(netk1), std::abs(netk2), std::abs(netk3), std::abs(netk4))) {
        return;
    }

    double dangle = std::acos((1 + netk1 * netk2) / std::sqrt(1 + netk1 * netk1) / std::sqrt(1 + netk2 * netk2));
    if (dangle > ADD_POINT_ANGLE) {	//need to add data
        double x = p1->x;
        double y = (p1->y + p2->y) / 2;
        double z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
        x = vert[sur[id]->p12]->x; //creat another point
        z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
        sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
        sur[id]->p21 = vert.size() - 2;
        sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
        if (sur[id]->ylineid != -1) {
            yline[sur[id]->ylineid]->add(vert.size() - 1);
        }
        if (sur[id]->y0line) {
            yline[0]->add(vert.size() - 2);
        }
        sur[id]->xlineid = -1;
        //if (z > zmax) zmax = z;
        //if (z < zmin) zmin = z;
        //trySplitPolygonY(sur.size() - 1, depth + 1); //continue split the two polygon
        trySplitPolygonY(id, depth + 1);
    } else { //try add data
        double x = p1->x;
        double y = (p1->y + p2->y) / 2;
        double netk3 = f->getDerivativeY(x, y, dy) / graphScaley;
        if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) { //有拐点
            double z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
            x = vert[sur[id]->p12]->x; //creat another point
            z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, y, dx), f->getDerivativeY(x, y, dy)));
            sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
            sur[id]->p21 = vert.size() - 2;
            sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
            if (sur[id]->ylineid != -1) {
                yline[sur[id]->ylineid]->add(vert.size() - 1);
            }
            if (sur[id]->y0line) {
                yline[0]->add(vert.size() - 2);
            }
            sur[id]->xlineid = -1;
            //if (z > zmax) zmax = z;
            //if (z < zmin) zmin = z;
            //trySplitPolygonY(sur.size() - 1, depth + 1); //continue split the two polygon
            trySplitPolygonY(id, depth + 1);
        }
    }
}

bool Surface::judgmentDY(uint id, double netk1, double netk2, double netk3, double netk4) {
    while (true) {
        double dis1 = 0.0;
        double dis2 = 0.0;
        //if ((netk1 > MAX_D && netk2 > MAX_D) || (netk3 > MAX_D && netk4 > MAX_D)) {
        if (false) {
            sur[id]->isshow = false;
            return true;
        } else if (netk1 > MAX_D || netk2 > MAX_D) {
            if (sur[id]->times > MAX_SPLIT) {
                sur[id]->isshow = false;
                return true;
            }
            dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) +
                ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) +
                ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz));
            dis2 = std::sqrt(((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) +
                ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) +
                ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz));
            if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
                sur[id]->isshow = false;
                return true;
            }

            double border = (vert[sur[id]->p11]->y + vert[sur[id]->p21]->y) / 2.0;
            if (!getbordery(id, border, vert[sur[id]->p11]->x, netk1, netk2, vert[sur[id]->p11]->y, vert[sur[id]->p21]->y, 5)) {
                sur[id]->isshow = false;
                return true;
            }

            //double netkmiddle2 = f->getDerivativeY(vert[sur[id]->p12]->x, border, dy) / graphScaley;

            double z = f->getSingleData(vert[sur[id]->p11]->x, border);
            vert.add(new Point3d(vert[sur[id]->p11]->x, border, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(vert[sur[id]->p11]->x, border, dx), f->getDerivativeY(vert[sur[id]->p11]->x, border, dy)));
            double x = vert[sur[id]->p12]->x; //creat another point
            double z1 = f->getSingleData(x, border);
            vert.add(new Point3d(x, border, z1));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, border, dx), f->getDerivativeY(x, border, dy)));
            //uint position = netk1 > MAX_D ? sur[id]->p12 : sur[id]->p11;
            if (netk1 > MAX_D) {
                sur.add(new IndexPolygonForSurface(sur[id]->p11, sur[id]->p12, vert.size() - 2, vert.size() - 1, sur[id]->xlineid, sur[id]->ylineid, false , false, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p11 = vert.size() - 2;
                sur[id]->p12 = vert.size() - 1;
                sur[id]->times++;
            } else {
                sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p21 = vert.size() - 2;
                sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
                sur[id]->times++;
            }

            if (sur[id]->ylineid != -1) {
                yline[sur[id]->ylineid]->add(vert.size() - 1);
            }
            if (sur[id]->y0line) {
                yline[0]->add(vert.size() - 2);
            }
            sur[id]->xlineid = -1;

            if (!std::abs(((*(vert_derivate.end() - 1)).vert_d_y / graphScaley) < MAX_D)) {
                continue;
            } else {
                if (z > zmax) zmax = z;
                if (z < zmin) zmin = z;
                if (z1 > zmax) zmax = z1;
                if (z1 < zmin) zmin = z1;
                //spz = zmax - zmin;
            }
            //double dmiddle = f->getDerivativeX(border, vert[sur[id]->p11]->y, dx);
        } else if (netk3 > MAX_D || netk4 > MAX_D) {
            if (sur[id]->times > MAX_SPLIT) {
                sur[id]->isshow = false;
                return true;
            }
            dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) +
                ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) +
                ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz));
            dis2 = std::sqrt(((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) +
                ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) +
                ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz));
            if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
                sur[id]->isshow = false;
                return true;
            }

            double border = (vert[sur[id]->p12]->y + vert[sur[id]->p22]->y) / 2.0;
            if (!getbordery(id, border, vert[sur[id]->p12]->x, netk1, netk2, vert[sur[id]->p11]->y, vert[sur[id]->p21]->y, 5)) {
                sur[id]->isshow = false;
                return true;
            }

            //double netkmiddle2 = f->getDerivativeY(vert[sur[id]->p11]->x, border, dy) / graphScaley;

            double z = f->getSingleData(vert[sur[id]->p11]->x, border);
            vert.add(new Point3d(vert[sur[id]->p11]->x, border, z));
            vert_derivate.push_back(vert_d(f->getDerivativeX(vert[sur[id]->p11]->x, border, dx), f->getDerivativeY(vert[sur[id]->p11]->x, border, dy)));
            double x = vert[sur[id]->p12]->x; //creat another point
            double z1 = f->getSingleData(x, border);
            vert.add(new Point3d(x, border, z1));
            vert_derivate.push_back(vert_d(f->getDerivativeX(x, border, dx), f->getDerivativeY(x, border, dy)));
            //uint position = netk1 > MAX_D ? sur[id]->p12 : sur[id]->p11;
            if (netk3 > MAX_D) {
                sur.add(new IndexPolygonForSurface(sur[id]->p11, sur[id]->p12, vert.size() - 2, vert.size() - 1, sur[id]->xlineid, sur[id]->ylineid, false , false, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p11 = vert.size() - 2;
                sur[id]->p12 = vert.size() - 1;
                sur[id]->times++;
            } else {
                sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0, sur[id]->isshow, sur[id]->times + 1));
                sur[id]->p21 = vert.size() - 2;
                sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
                sur[id]->times++;
            }

            if (sur[id]->ylineid != -1) {
                yline[sur[id]->ylineid]->add(vert.size() - 1);
            }
            if (sur[id]->y0line) {
                yline[0]->add(vert.size() - 2);
            }
            sur[id]->xlineid = -1;

            if (!(std::abs((*(vert_derivate.end() - 1)).vert_d_y / graphScaley) < MAX_D)) {
                continue;
            } else {
                if (z > zmax) zmax = z;
                if (z < zmin) zmin = z;
                if (z1 > zmax) zmax = z1;
                if (z1 < zmin) zmin = z1;
                //spz = zmax - zmin;
            }
        }
        dis1 = std::sqrt(((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) * ((vert[sur[id]->p11]->x - vert[sur[id]->p21]->x) / spx) +
            ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) * ((vert[sur[id]->p11]->y - vert[sur[id]->p21]->y) / spy) +
            ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz) * ((vert[sur[id]->p11]->z - vert[sur[id]->p21]->z) / spz));
        dis2 = std::sqrt(((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) * ((vert[sur[id]->p12]->x - vert[sur[id]->p22]->x) / spx) +
            ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) * ((vert[sur[id]->p12]->y - vert[sur[id]->p22]->y) / spy) +
            ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz) * ((vert[sur[id]->p12]->z - vert[sur[id]->p22]->z) / spz));
        if (dis1 < MIN_DIS_3D || dis2 < MIN_DIS_3D) {
            return true;
        }
        return false;
    }
}

bool Surface::getbordery(uint id, double &border, double x, double netk1, double netk2, double y1, double y2, int depth) {
    double netkmiddle = f->getDerivativeY(x, border, dy) / graphScaley;
    if (depth <= 0) {
        if (std::abs(netkmiddle) > MAX_D) {
            if (std::abs(netk1) > MAX_D) {
                if (border == y2) {
                    return false;
                } else {
                    border = y2;
                    return true;
                }
            } else {
                if (border == y1) {
                    return false;
                } else {
                    border = y1;
                    return true;
                }
            }
        } else {
            return true;
        }
    }

    //if (depth <= 0) {
    //    if (netkmiddle > MAX_D) {
    //        sur[id]->isshow = false;
    //        return false;
    //    } else {
    //        return true;
    //    }
    //}
    if (std::abs(netkmiddle) > MAX_D) {
        if (std::abs(netk1) > MAX_D) {
            double border_old = border;
            border = (y2 + border) / 2.0;
            return getbordery(id, border, x, netkmiddle, netk2, border_old, y2, --depth);
        } else {
            double border_old = border;
            border = (y1 + border) / 2.0;
            return getbordery(id, border, x, netk1, netkmiddle, y1, border_old, --depth);
        }
    } else {
        if (std::abs(netk1) > MAX_D) {
            double border_old = border;
            border = (y1 + border) / 2.0;
            return getbordery(id, border, x, netk1, netkmiddle, y1, border_old, --depth);
        } else {
            double border_old = border;
            border = (y2 + border) / 2.0;
            return getbordery(id, border, x, netkmiddle, netk2, border_old, y2, --depth);
        }
    }
}

}
}
