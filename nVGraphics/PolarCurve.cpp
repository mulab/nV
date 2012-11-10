#include "PolarCurve.h"
#include <nV/utils.h>

namespace nV {
    namespace Graphics {

PolarCurve::PolarCurve(F1P *ff, double start, double end) {
    //LineStrip* lineStrip = new LineStrip;
    //lineStrips.add(lineStrip);
    f = ff;
    tmin = start;
    tmax = end;
    spt = tmax - tmin;
    //timer t;
    //t.start();
    makeplotrange(f, WithDomain, Domains, rmax, rmin, tmax, tmin);
    //t.end();
    //std::cout << "makeplotrange" << std::fixed << t.value << std::endl;
    //t.start();
    spr = rmax - rmin;
    makePoints();
    //t.end();
    //std::cout << "makepoint:" << std::fixed << t.value << std::endl;
}

PolarCurve::PolarCurve(F1P *ff, double start, double end, F1P *cff) {
    //LineStrip* lineStrip = new LineStrip;
    //lineStrips.add(lineStrip);
    f = ff;
    tmin = start;
    tmax = end;
    spt = tmax - tmin;
    //color function
    cf = cff;
    isColorfulCurve = true;
    makeplotrange(f, WithDomain, Domains, rmax, rmin, tmax, tmin);
    spr = rmax - rmin;
    makePoints();
    useColorFunction();
}

PolarCurve::~PolarCurve() {
    for(uint i = 0; i < arrayTtotal.size(); i++) {
        (*(arrayTtotal[i])).clear();
    }
    arrayTtotal.clear();
}


typedef struct {
    double tmax;
    double tmin;
} trange;
void PolarCurve::makePoints() {
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
    double *r = new double[dataNum];
    for(uint countrange = 0; countrange < range.size(); countrange++) {
        double tmin_old = tmin;
        double tmax_old = tmax;
        tmin = range[countrange].tmin;
        tmax = range[countrange].tmax;
        for (int i = 0; i < dataNum; i++) {
            //split the domain
            t[i] = tmin + (tmax - tmin) * i / (dataNum - 1);
        }
        f->getArrayData(t, r, dataNum);
        LineStrip *linestrip;
        ArrayList<double> *arrayT;
        int i = 0;
        while(((r[i] < rmin) || (r[i] > rmax) || isNaN(r[i])) && i < dataNum) i++;
        if (i == dataNum) {
            continue;
        }
        linestrip = new LineStrip;
        arrayT = new ArrayList<double>;
        lineStrips.add(linestrip);
        arrayTtotal.add(arrayT);
        if(i == 0) {
            linestrip->vert.add(new Point2d(r[i] * cos(t[i]), r[i] * sin(t[i])));
            (*arrayT).add(t[i]);
        } else {
            double newt = getPointOnRangeFromLeft(f, t[i - 1], t[i], 1);
            double newr = f->getSingleData(newt);
            linestrip->vert.add(new Point2d(newr * cos(newt), newr * sin(newt)));
            (*arrayT).add(newt);
            linestrip->vert.add(new Point2d(r[i] * cos(t[i]), r[i] * sin(t[i])));
            (*arrayT).add(t[i]);
        }
        i++;
        int state = 0;
        while(i < dataNum) {
            if((r[i] <= rmax) && (r[i] >= rmin) && !isNaN(r[i])) {
                if(state == 0) {
                    linestrip->vert.add(new Point2d(r[i] * cos(t[i]), r[i] * sin(t[i])));
                    (*arrayT).add(t[i]);
                } else {
                    linestrip = new LineStrip;
                    arrayT = new ArrayList<double>;
                    lineStrips.add(linestrip);
                    arrayTtotal.add(arrayT);
                    double newt = getPointOnRangeFromLeft(f, t[i - 1], t[i], 1);
                    double newr = f->getSingleData(newt);
                    linestrip->vert.add(new Point2d(newr * cos(newt), newr * sin(newt)));
                    (*arrayT).add(newt);
                    linestrip->vert.add(new Point2d(r[i] * cos(t[i]), r[i] * cos(t[i])));
                    (*arrayT).add(t[i]);
                    state = 0;
                }
            } else {
                if(state == 0) {
                    double newt = getPointOnRangeFromRight(f, t[i - 1], t[i], 1);
                    double newr = f->getSingleData(newt);
                    linestrip->vert.add(new Point2d(newr * cos(newt), newr * sin(newt)));
                    (*arrayT).add(newt);
                    state = 1;
                } else {}
            }
            i++;
        }

        if(countrange == 0) {
            xmin = lineStrips[0]->vert[0]->x;
            xmax = lineStrips[0]->vert[0]->x;
            ymin = lineStrips[0]->vert[0]->y;
            ymax = lineStrips[0]->vert[0]->y;
        }
        for(uint z = 0; z < lineStrips[countrange]->vert.size(); z++) {
            if(lineStrips[countrange]->vert[z]->x > xmax) xmax = lineStrips[countrange]->vert[z]->x;
            if(lineStrips[countrange]->vert[z]->x < xmin) xmin = lineStrips[countrange]->vert[z]->x;
            if(lineStrips[countrange]->vert[z]->y > ymax) ymax = lineStrips[countrange]->vert[z]->y;
            if(lineStrips[countrange]->vert[z]->y < ymin) ymin = lineStrips[countrange]->vert[z]->y;
        }
        spt = tmax - tmin;
        spr = rmax - rmin;
        spx = xmax - xmin;
        spy = ymax - ymin;
        graphScale = spy / spx;
        dt = spt / LARGE;

        for(uint i = 0; i < lineStrips.size(); i++) {
            addVerts(lineStrips[i], *arrayTtotal[i]);
        }
        tmax = tmax_old;
        tmin = tmin_old;
    }

    delete[] t;
    delete[] r;
}

double PolarCurve::getPointOnRangeFromLeft(F1P *f_x, double t1, double t2, int depth) {
    if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
        return t2;
    double t = (t1 + t2) / 2;
    double x = f_x->getSingleData(t);
    if (x < xmin || x > xmax || isNaN(x))
        return getPointOnRangeFromLeft(f_x, t, t2, depth + 1);
    else
        return getPointOnRangeFromLeft(f_x, t1, t, depth + 1);
}

double PolarCurve::getPointOnRangeFromRight(F1P *f_x, double t1, double t2, int depth) {
    if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
        return t1;
    double t = (t1 + t2) / 2;
    double x = f_x->getSingleData(t);
    if (x < xmin || x > xmax || isNaN(x))
        return getPointOnRangeFromRight(f_x, t1, t, depth + 1);
    else
        return getPointOnRangeFromRight(f_x, t, t2, depth + 1);
}

int PolarCurve::getBaseNum() {
    return 20;
}

void PolarCurve::addVerts(LineStrip* lineStrip, ArrayList<double> &arrayT) {
    ArrayList<double> kcaled;
    if(graphScale == 0) {
        return;
    }
    double graphScalet = spr / spt;
    kcaled.add(getK(arrayT[0], dt) / graphScale);
    //double discoe = 0;
    unsigned int i = 0;
    while (i < lineStrip->vert.size() - 1) {
        double dis = std::sqrt(((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) * ((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) + ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy) * ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy));
        //double disx=arrayT[i + 1] - arrayT[i];
        double disx = lineStrip->vert[i + 1]->x - lineStrip->vert[i]->x;
        if (dis < (MIN_DELTA_DIS_POLAR_CURVE)  || std::abs(disx)<=MIN_X_DIS) {
            i++;
            //discoe = 0.0;
            kcaled.add(getK(arrayT[i], dt) / graphScale);
            continue;
        }
        double netk1 = kcaled[i];
        double netk2 = getK(arrayT[i+1], -dt) / graphScale;
        double dangle = std::acos((1 + netk1 * netk2) / std::sqrt(1 + netk1 * netk1) / std::sqrt(1 + netk2 * netk2));
        if (dangle > ADD_POINT_ANGLE_POLAR_CURVE) {
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            double r = f->getSingleData(t);
             if(!isNaN(r)) {
                lineStrip->vert.add(i + 1, new Point2d(r * cos(t), r * sin(t)));
                arrayT.add(i + 1, t);
                //discoe += 0.01 * graphScale;
                if (r > rmax) rmax = r;
                if (r < rmin) rmin = r;
                if (r * cos(t) > xmax) xmax = r * cos(t);
                if (r * cos(t) < xmin) xmin = r * cos(t);
                if (r * sin(t) > ymax) ymax = r * sin(t);
                if (r * sin(t) < ymin) ymin = r * sin(t);
            } else {
                i++;
                //discoe = 0.0;
            }
        } else {
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            double netk3 = getK(t, -dt) / graphScale;
            if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) {
                double t = (arrayT[i] + arrayT[i+1]) / 2;
                double r = f->getSingleData(t);
                if(!isNaN(r)) {
                    lineStrip->vert.add(i + 1, new Point2d(r * cos(t), r * sin(t)));
                    arrayT.add(i + 1, t);
                    //discoe += 0.05 * graphScale;
                    if (r > rmax) rmax = r;
                    if (r < rmin) rmin = r;
                    if (r * cos(t) > xmax) xmax = r * cos(t);
                    if (r * cos(t) < xmin) xmin = r * cos(t);
                    if (r * sin(t) > ymax) ymax = r * sin(t);
                    if (r * sin(t) < ymin) ymin = r * sin(t);
                } else {
                    i++;
                    //discoe = 0.0;
                }
            } else {
                i++;
                //discoe = 0.0;
                kcaled.add(getK(arrayT[i], dt) / graphScale);
            }
        }
    }
    spt = tmax - tmin;
    spr = rmax - rmin;
    spx = xmax - xmin;
    spy = ymax - ymin;
    graphScale = spy / spx;
}

void PolarCurve::useColorFunction() {
    double cfmin, cfmax;
    cfmin = cf->getSingleData((*arrayTtotal[0])[0]);
    cfmax = cfmin;
    for (unsigned int i = 0; i < lineStrips.size(); i++) {
        LineStrip* lineStrip = lineStrips[i];
        for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
            double tmp = cf->getSingleData((*arrayTtotal[i])[j]);
            if (tmp > cfmax)
                cfmax = tmp;
            if (tmp < cfmin)
                cfmin = tmp;
        }
    }
    for (unsigned int i = 0; i < lineStrips.size(); i++) {
        LineStrip* lineStrip = lineStrips[i];
        for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
            double tmp = cf->getSingleData((*arrayTtotal[i])[j]);
            if(isNaN(cfmax) || isNaN(cfmin) || isNaN(tmp) || (cfmax == cfmin)) {
                lineStrip->color.add(j, getRainbowColor(1));
                continue;
            }
            lineStrip->color.add(j, getRainbowColor((tmp - cfmin) / (cfmax - cfmin)));
        }
    }
}

void PolarCurve::makeplotrange(F1P *f, bool havenanrange, std::vector<domain *> &nanrangeset, double &ymax_f, double &ymin_f, double xmax_f, double xmin_f) {
    int bigDataNum = 200;
    double *x = new double[bigDataNum];
    double *y = new double[bigDataNum];
    for (int i = 0; i < bigDataNum; i++) {
        x[i] = xmin_f + (xmax_f - xmin_f) * i / (bigDataNum - 1);
    }
    f->getArrayData(x, y, bigDataNum);
    ArrayList<Point2d*> vert;
    //ArrayList<Point2d*> vert_polor;
    ArrayList<double> d;
    for (int i = 0; i < bigDataNum; i++) {
        vert.add(new Point2d(x[i], y[i]));
        //vert_polor.add(new Point2d(y[i] * cos(x[i]), y[i] * sin(x[i])));
        //vert_polor.add(new Point2d(x[i], y[i]));
    }

    uint i = 0;
    ymin_f = ymax_f = 0L;
    for(; i < vert.size(); i++) {
        if(!isNaN(vert[i]->y)) {
            ymin_f = vert[i]->y;
            ymax_f = vert[i]->y;
            ymax = vert[i]->y * cos(vert[i]->x);
            ymin = vert[i]->y * cos(vert[i]->x);
            break;
        }
    }
    for (; i < vert.size(); i++) {
        if(!isNaN(vert[i]->y)) {
            if (vert[i]->y > ymax_f) ymax_f = vert[i]->y;
            if (vert[i]->y < ymin_f) ymin_f = vert[i]->y;
            if (vert[i]->y * cos(vert[i]->x) > ymax) ymax = vert[i]->y * cos(vert[i]->x);
            if (vert[i]->y * cos(vert[i]->x) < ymin) ymin = vert[i]->y * cos(vert[i]->x);
        }
    }
    double spx_f = xmax_f - xmin_f;
    double spy_f = ymax_f - ymin_f;
    xmax = y[bigDataNum - 1] * sin(x[bigDataNum - 1]);
    xmin = y[0] * sin(x[0]);
    spy = ymax - ymin;
    spx = xmax - xmin;
    graphScale = spy / spx;
    double dx_f = spx_f / LARGE;
    for (unsigned int i = 0; i < vert.size(); i++) {
        d.add(getNaNk(x[i], dx_f));
    }
    delete [] x;
    delete [] y;
    //
    //去除斜率太大的点
    //
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
                    if(graphScale != 0) {
                        if (std::abs(d[i]) / graphScale > MAX_DERIVATIVE) {
                            //vert.remove(i);
                            //d.remove(i);
                            singularity = true;
                            flag[i] = true;
                            vrm = true;
                        }
                    }
                }
            }
        }

        getyrange(ymax_f, ymin_f, graphScale, spx_f, spy_f, vert, flag, nanflag);

        if(graphScale < MIN_GRAPHSCALE_MINIMUM_CURVE) {
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
    /*for(ArrayList<Point2d *>::iterator it = vert_polor.begin(); it != vert_polor.end(); it++) {
        delete (*it);
    }*/
}

void PolarCurve::detectnanborder(F1P *f, std::vector<domain *> &nanrangeset, std::vector<uint> &nannodesite,
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

double PolarCurve::getrightborder(F1P *f, double lx, double rx, uint deepth) {
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

void PolarCurve::getyrange(double &ymax_f, double &ymin_f, double &graphScale_f, double &spx_f, double &spy_f, 
    const std::vector<Point2d *> &vert, const std::vector<bool> &flag, const std::vector<bool> &nanflag) {
        for(uint i = 0; i < vert.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                ymin_f = vert[i]->y;
                ymax_f = vert[i]->y;
                ymax = vert[i]->y * cos(vert[i]->x);
                ymin = vert[i]->y * cos(vert[i]->x);
                break;
            }
        }
        for (uint i = 0; i < vert.size(); i++) {
            if(!flag[i] && !nanflag[i]) {
                if (vert[i]->y > ymax_f) ymax_f = vert[i]->y;
                if (vert[i]->y < ymin_f) ymin_f = vert[i]->y;
                if (vert[i]->y * cos(vert[i]->x) > ymax) ymax = vert[i]->y * cos(vert[i]->x);
                if (vert[i]->y * cos(vert[i]->x) < ymin) ymin = vert[i]->y * cos(vert[i]->x);
            }
        }
        spy_f = ymax_f - ymin_f;
        graphScale_f = spy_f / spx_f;
        spy = ymax - ymin;
        graphScale = spy / spx;
}

}
}
