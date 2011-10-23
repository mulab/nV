#include "ParametricCurve3D.h"
#include <cmath>

namespace nV {
    namespace Graphics {

ParametricCurve3D::ParametricCurve3D(F1P *ffx, F1P *ffy, F1P *ffz, double start, double end) {
    arrayTtotal.clear();
    Domains.clear();
    tWithDomain_x = false;
    tDomains_x.clear();
    tWithDomain_y = false;
    tDomains_y.clear();
    tWithDomain_z = false;
    tDomains_z.clear();
    fx = ffx;
    fy = ffy;
    fz = ffz;
    tmin = start;
    tmax = end;
    spt = tmax - tmin;
    makeplotrange(fx, tWithDomain_x, tDomains_x, xmax, xmin, tmax, tmin);
    makeplotrange(fy, tWithDomain_y, tDomains_y, ymax, ymin, tmax, tmin);
    makeplotrange(fz, tWithDomain_z, tDomains_z, zmax, zmin, tmax, tmin);
    bool tempwithdomain = false;
    std::vector<domain *> temp;
    temp.clear();
    mergedomain(tWithDomain_x, tDomains_x, tWithDomain_y, tDomains_y, temp);
    tempwithdomain = temp.size() != 0 ? true : false;
    mergedomain(tempwithdomain, temp, tWithDomain_z, tDomains_z, Domains);
    makePoints();
}

void ParametricCurve3D::mergedomain(bool tWithDomain_first_f, std::vector<domain *> tDomain_first_f,
    bool tWithDomain_second_f, std::vector<domain *> tDomain_second_f,
    std::vector<domain *> &Domains_f) {
                if(tWithDomain_first_f && tWithDomain_second_f) {
                for(uint i = 0; i < tDomain_first_f.size(); i++) {
                    for(uint j = 0; j < tDomain_second_f.size(); j++) {
                        if(!((tDomain_first_f[i]->min > tDomain_second_f[j]->max) || (tDomain_first_f[i]->max < tDomain_second_f[j]->min))) {
                            domain *d = new domain;
                            d->max = tDomain_first_f[i]->max < tDomain_second_f[j]->max ? tDomain_first_f[i]->max : tDomain_second_f[j]->max;
                            d->min = tDomain_first_f[i]->min > tDomain_second_f[j]->min ? tDomain_first_f[i]->min : tDomain_second_f[j]->min;
                            Domains_f.push_back(d);
                        }
                    }
                }
            } else if(tWithDomain_first_f) {
                for(uint i = 0; i < tDomain_first_f.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomain_first_f[i]->max;
                    d->min = tDomain_first_f[i]->min;
                    Domains_f.push_back(d);
                }
            } else if(tWithDomain_second_f) {
                for(uint i = 0; i < tDomain_second_f.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomain_second_f[i]->max;
                    d->min = tDomain_second_f[i]->min;
                    Domains_f.push_back(d);
                }
            }
}

typedef struct {
    double tmax;
    double tmin;
} trange;

void ParametricCurve3D::makePoints() {
    std::vector<trange> range;
    if(Domains.size() == 0) {
        trange tr;
        tr.tmin = tmin;
        tr.tmax = tmax;
        range.push_back(tr);
    } else {
        for(uint i = 0; i < Domains.size(); i++) {
            trange tr;
            tr.tmax = Domains[i]->max;
            tr.tmin = Domains[i]->min;
            range.push_back(tr);
        }
    }
    dataNum = getBaseNum();
    double *t = new double[dataNum];
    double *x = new double[dataNum];
    double *y = new double[dataNum];
    double *z = new double[dataNum];
    for(uint countrange = 0; countrange < range.size(); countrange++) {
        double tmin_old = tmin;
        double tmax_old = tmax;
        tmin = range[countrange].tmin;
        tmax = range[countrange].tmax;
        for (int i = 0; i < dataNum; i++) {
            //split the domain
            t[i] = tmin + (tmax - tmin) * i / (dataNum - 1);
        }
        fx->getArrayData(t, x, dataNum);
        fy->getArrayData(t, y, dataNum);
        fz->getArrayData(t, z, dataNum);
        LineStrip *linestrip;
        ArrayList<double> *arrayT;
        int i = 0;
        while(((x[i] < xmin) || (x[i] > xmax)
            || (y[i] < ymin) || (y[i] > ymax)
            || (z[i] < zmin) || (z[i] > zmax)
            || isNaN(x[i]) || isNaN(y[i]) || isNaN(z[i]))
            && i < dataNum) i++;
        linestrip = new LineStrip;
        arrayT = new ArrayList<double>;
        lineStrips.add(linestrip);
        arrayTtotal.add(arrayT);
        if(i == 0) {    //线中
            linestrip->vert.push_back(Point3d(x[i], y[i], z[i]));
            (*arrayT).add(t[i]);
        } else {    //线尾
            double newt = getPointOnRangeFromLeft(fx, fy, fz, t[i - 1], t[i], 1);
            double newx = fx->getSingleData(newt);
            double newy = fy->getSingleData(newt);
            double newz = fz->getSingleData(newt);
            linestrip->vert.push_back(Point3d(newx, newy, newz));
            (*arrayT).add(newt);
            linestrip->vert.push_back(Point3d(x[i], y[i], z[i]));
            (*arrayT).add(t[i]);
        }
        i++;
        int state = 0;
        while(i < dataNum) {
            if((x[i] < xmax) && (x[i] > xmin) &&
               (y[i] > ymin) && (y[i] < ymax) &&
               (z[i] > zmin) && (z[i] < zmax) &&
               !isNaN(x[i]) && !isNaN(y[i]) && !isNaN(z[i])) {
                if(state == 0) {
                    linestrip->vert.push_back(Point3d(x[i], y[i], z[i]));
                    (*arrayT).add(t[i]);
                } else {
                    linestrip = new LineStrip;
                    arrayT = new ArrayList<double>;
                    lineStrips.add(linestrip);
                    arrayTtotal.add(arrayT);
                    double newt = getPointOnRangeFromLeft(fx, fy, fz, t[i - 1], t[i], 1);
                    double newx = fx->getSingleData(newt);
                    double newy = fy->getSingleData(newt);
                    double newz = fz->getSingleData(newt);
                    linestrip->vert.push_back(Point3d(newx, newy, newz));
                    (*arrayT).add(newt);
                    linestrip->vert.push_back(Point3d(x[i], y[i], z[i]));
                    (*arrayT).add(t[i]);
                    state = 0;
                }
            } else {
                if(state == 0) {
                    double newt = getPointOnRangeFromRight(fx, fy, fz, t[i - 1], t[i], 1);
                    double newx = fx->getSingleData(newt);
                    double newy = fy->getSingleData(newt);
                    double newz = fz->getSingleData(newt);
                    linestrip->vert.push_back(Point3d(newx, newy, newz));
                    (*arrayT).add(newt);
                    state = 1;
                } else {}
            }
            i++;
        }


        spx = xmax - xmin;
        spy = ymax - ymin;
        spz = zmax - zmin;
        //graphScale = spy / spx;
        dt = spt / LARGE;

        for(uint i = 0; i < lineStrips.size(); i++) {
            addVerts(lineStrips[i], *arrayTtotal[i]);
        }
        tmax = tmax_old;
        tmin = tmin_old;
    }
    delete[] t;
    delete[] x;
    delete[] y;
    delete[] z;

}

double ParametricCurve3D::getPointOnRangeFromLeft(F1P *f_x, F1P *f_y, F1P *f_z, double t1, double t2, int depth) {
    if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
        return t2;
    double t = (t1 + t2) / 2;
    double x = f_x->getSingleData(t);
    double y = f_y->getSingleData(t);
    double z = f_z->getSingleData(t);
    if (x < xmin || x > xmax ||
        y < ymin || y > ymax ||
        z < zmin || z > zmax ||
        isNaN(y) || isNaN(x) || isNaN(z))
        return getPointOnRangeFromLeft(f_x, f_y, f_z,t, t2, depth + 1);
    else
        return getPointOnRangeFromLeft(f_x, f_y, f_z,t1, t, depth + 1);
}

double ParametricCurve3D::getPointOnRangeFromRight(F1P *f_x, F1P *f_y, F1P *f_z, double t1, double t2, int depth) {
    if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
        return t1;
    double t = (t1 + t2) / 2;
    double x = f_x->getSingleData(t);
    double y = f_y->getSingleData(t);
    double z = f_z->getSingleData(t);
    if (x < xmin || x > xmax ||
        y < ymin || y > ymax ||
        z < zmin || z > zmax ||
        isNaN(y) || isNaN(x) || isNaN(z))
        return getPointOnRangeFromRight(f_x, f_y, f_z, t1, t, depth + 1);
    else
        return getPointOnRangeFromRight(f_x, f_y, f_z, t, t2, depth + 1);
}
void ParametricCurve3D::creatCurve() {
}

ParametricCurve3D::~ParametricCurve3D() {
    for (unsigned int i = 0; i < vert.size(); i++) {
        delete vert[i];
    }
    vert.clear();

    for (uint i = 0; i < lineStrips.size(); i++) {
        delete lineStrips[i];
    }
    lineStrips.clear();
    for(uint i = 0; i < arrayTtotal.size(); i++) {
        delete arrayTtotal[i];
    }
    arrayTtotal.clear();
    for(uint i = 0; i < tDomains_x.size(); i++) {
        delete tDomains_x[i];
    }
    tDomains_x.clear();
    for(uint i = 0; i < tDomains_y.size(); i++) {
        delete tDomains_y[i];
    }
    tDomains_y.clear();
    for (uint i = 0; i < tDomains_z.size(); i++) {
        delete tDomains_z[i];
    }
    tDomains_z.clear();
    for (uint i = 0; i < Domains.size(); i++) {
        delete Domains[i];
    }
    Domains.clear();
}

int ParametricCurve3D::getBaseNum() {
    return 20;
}

void ParametricCurve3D::addVerts(LineStrip* lineStrip, ArrayList<double> &arrayT) {
    ArrayList<Vector3D*> vcaled; //save the vector calculated
    vcaled.add(getVector(arrayT[0], dt));
    unsigned int i = 0;
    while (i < (*lineStrip).vert.size() - 1) {
        double dis = std::sqrt(((lineStrip->vert[i+1].x - lineStrip->vert[i].x) / spx) * ((lineStrip->vert[i+1].x - lineStrip->vert[i].x) / spx) + ((lineStrip->vert[i+1].y - lineStrip->vert[i].y) / spy) * ((lineStrip->vert[i+1].y - lineStrip->vert[i].y) / spy) + ((lineStrip->vert[i+1].z - lineStrip->vert[i].z) / spz) * ((lineStrip->vert[i+1].z - lineStrip->vert[i].z) / spz));
        if (dis < MIN_DELTA_DIS_3D) {
            i++;
            vcaled.add(getVector(arrayT[i], dt));
            continue;
        }
        Vector3D v1 = *(vcaled[i]);
        Vector3D v2 = *(getVector(arrayT[i+1], -dt));
        double dangle = std::acos((v1 * v2) / std::sqrt(v1 * v1) / std::sqrt(v2 * v2));
        if (dangle > ADD_POINT_ANGLE_3D) {
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            double x = fx->getSingleData(t);
            double y = fy->getSingleData(t);
            double z = fz->getSingleData(t);
            //vert.add(i + 1, new Point3d(x, y, z));
            if (!isNaN(x) && !isNaN(y) && !isNaN(z)) {
                lineStrip->vert.insert(lineStrip->vert.begin() + i + 1, Point3d(x, y, z));
                arrayT.add(i + 1, t);
                if (x > xmax) xmax = x;
                if (x < xmin) xmin = x;
                if (y > ymax) ymax = y;
                if (y < ymin) ymin = y;
                if (z > zmax) zmax = z;
                if (z < zmin) zmin = z;
            } else {
                i++;
            }
        } else { //尝试添加
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            Vector3D v3 = *(getVector(t, -dt));
            if ((v1*v3)*(v2*v3) < 0) {
                double t = (arrayT[i] + arrayT[i+1]) / 2;
                double x = fx->getSingleData(t);
                double y = fy->getSingleData(t);
                double z = fz->getSingleData(t);
                if (!isNaN(x) && !isNaN(y) && !isNaN(z)) {
                    //vert.add(i + 1, new Point3d(x, y, z));
                    lineStrip->vert.insert(lineStrip->vert.begin() + i + 1, Point3d(x, y, z));
                    arrayT.add(i + 1, t);
                    if (x > xmax) xmax = x;
                    if (x < xmin) xmin = x;
                    if (y > ymax) ymax = y;
                    if (y < ymin) ymin = y;
                    if (z > zmax) zmax = z;
                    if (z < zmin) zmin = z;
                } else {
                    i++;
                }
            } else {
                i++;
                vcaled.add(getVector(arrayT[i], dt));
            }
        }
    }
    spx = xmax - xmin;
    spy = ymax - ymin;
    spz = zmax - zmin;
    for (unsigned int i = 0; i < vcaled.size(); i++) {
        delete vcaled[i];
    }
    vcaled.clear();
}

void ParametricCurve3D::makeplotrange(F1P *f, bool havenanrange, std::vector<domain *> &nanrangeset, double &ymax_f, double &ymin_f, double xmax_f, double xmin_f) {
    int bigDataNum = 500;
    double *x = new double[bigDataNum];
    double *y = new double[bigDataNum];
    for (int i = 0; i < bigDataNum; i++) {
        x[i] = xmin_f + (xmax_f - xmin_f) * i / (bigDataNum - 1);
    }
    f->getArrayData(x, y, bigDataNum);
    ArrayList<Point2d*> vert;
    ArrayList<double> d;
    for (int i = 0; i < bigDataNum; i++) {
        vert.add(new Point2d(x[i], y[i]));
    }
    delete [] x;
    delete [] y;
    uint i = 0;
    ymin_f = ymax_f = 0L;
    for(; i < vert.size(); i++) {
        if(!isNaN(vert[i]->y)) {
            ymin_f = vert[i]->y;
            ymax_f = vert[i]->y;
            break;
        }
    }
    for (; i < vert.size(); i++) {
        if(!isNaN(vert[i]->y)) {
            if (vert[i]->y > ymax_f) ymax_f = vert[i]->y;
            if (vert[i]->y < ymin_f) ymin_f = vert[i]->y;
        }
    }
    double spx_f = xmax_f - xmin_f;
    double spy_f = ymax_f - ymin_f;
    double graphScale_f = spy_f / spx_f;
    double dx_f = spx_f / LARGE;
    for (unsigned int i = 0; i < vert.size(); i++) {
        d.add(f->getDerivative(vert[i]->x, dx_f));
    }

    //
    //去除斜率太大的点
    //
    bool singularity = false;
    bool vrm = true; //vert removed
    bool isnanfunction = false;
    std::vector<bool> flag(vert.size(), false); //if true, the vert's derivative is too big
    std::vector<bool> nanflag(vert.size(), false);  //if true, the vert's value is NaN
    //std::vector<domain> nanrangeset;    //the set of one curve's range
    std::vector<uint> nannodesite;  //the site of the node which is the border

    int count = 0;
    while (vrm) {
        vrm = false;
        for (unsigned int i = 0; i < vert.size(); i++) {
            if(isNaN(d[i])) {
                flag[i] = true;
                nanflag[i] = true;
                isnanfunction = true;
            } else {
                if(!flag[i]) {
                    if (std::abs(d[i]) / graphScale_f > MAX_DERIVATIVE) {
                        //vert.remove(i);
                        //d.remove(i);
                        singularity = true;
                        flag[i] = true;
                        vrm = true;
                    }
                }
            }
        }

        getyrange(ymax_f, ymin_f, graphScale_f, spx_f, spy_f, vert, flag, nanflag);

        if(graphScale_f < MIN_GRAPHSCALE_MINIMUM) {
            break;
        }
        if(count >= MAX_DELETE_AMOUNT) {
            break;
        }
        count++;
    }

    if(isnanfunction) {
        detectnanborder(f, nanrangeset, nannodesite, nanflag, flag, vert);
    }

    havenanrange = isnanfunction;

    for(ArrayList<Point2d*>::iterator it = vert.begin(); it != vert.end(); it++) {
        delete (*it);
    }
}

void ParametricCurve3D::detectnanborder(F1P *f, std::vector<domain *> &nanrangeset, std::vector<uint> &nannodesite,
    const std::vector<bool> &nanflag, const std::vector<bool> &derivativeflag,
    const std::vector<Point2d *> &vert) {
        double xright;
        uint deepth = 5;
        domain *node;
        bool locationflag = false;
        for(uint i = 0; i < nanflag.size() - 1; i++) {
            if(nanflag[i] && !nanflag[i + 1]) {
                double x = (vert[i]->x + vert[i + 1]->x) / 2;
                double y = f->getSingleData(x);
                if(isNaN(y)) {
                    xright = getrightborder(f, x, vert[i + 1]->x, deepth);
                } else {
                    xright = getrightborder(f, vert[i]->x, x, deepth);
                }
                locationflag = true;
                node = new domain;
                node->min = xright;
                nannodesite.push_back(i + 1);
            } else if(!nanflag[i] && nanflag[i + 1]) {
                double x = (vert[i]->x + vert[i + 1]->x) / 2;
                double y = f->getSingleData(x);
                if(isNaN(y)) {
                    xright = getrightborder(f, x, vert[i]->x, deepth);
                } else {
                    xright = getrightborder(f, vert[i + 1]->x, x, deepth);
                }
                if(!locationflag) {
                    node = new domain;
                    node->min = vert[0]->x;
                    nannodesite.push_back(0);
                }
                locationflag = false;
                node->max = xright;
                nannodesite.push_back(i);
                nanrangeset.push_back(node);
            }
        }

        if(locationflag) {
            node->max = vert[vert.size() - 1]->x;
            nannodesite.push_back(vert.size() - 1);
            nanrangeset.push_back(node);
        }
}

double ParametricCurve3D::getrightborder(F1P *f, double lx, double rx, uint deepth) {
    double x = (lx + rx) / 2.0;
    double y = f->getSingleData(x);
    double xright;
    if(deepth <= 0) {
        if(!isNaN(y)) {
            return x;
        } else {
            return rx;
        }
    }
    if(isNaN(y)) {
        xright = getrightborder(f, x, rx, --deepth);
    } else {
        xright = getrightborder(f, lx, x, --deepth);
    }
    return xright;
}

void ParametricCurve3D::getyrange(double &ymax_f, double &ymin_f, double &graphScale_f, double &spx_f, double &spy_f, 
    const std::vector<Point2d *> &vert, const std::vector<bool> &flag, const std::vector<bool> &nanflag) {
        for(uint i = 0; i < vert.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                ymin_f = vert[i]->y;
                ymax_f = vert[i]->y;
                break;
            }
        }
        for (uint i = 0; i < vert.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                if (vert[i]->y > ymax_f) ymax_f = vert[i]->y;
                if (vert[i]->y < ymin_f) ymin_f = vert[i]->y;
            }
        }
        spy_f = ymax_f - ymin_f;
        graphScale_f = spy_f / spx_f;
}
//获得切向量
Vector3D* ParametricCurve3D::getVector(double t, double dt) {
    return new Vector3D(fx->getDerivative(t, dt) / spx, fy->getDerivative(t, dt) / spy, fz->getDerivative(t, dt) / spz);
}

}
}
