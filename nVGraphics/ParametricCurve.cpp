#include "ParametricCurve.h"
#include <cmath>

namespace nV {
    namespace Graphics {

        ParametricCurve::ParametricCurve(F1P *ffx, F1P *ffy, double start, double end) {
            //    LineStrip* lineStrip = new LineStrip;
            //    lineStrips.add(lineStrip);
            arrayTtotal.clear();
            tWithDomain_x = false;
            tWithDomain_y = false;
            tDomains_x.clear();
            tDomains_y.clear();
            fx = ffx;
            fy = ffy;
            tmin = start;
            tmax = end;
            spt = tmax - tmin;
            makeplotrange(fx, tWithDomain_x,tDomains_x, xmax, xmin, tmax, tmin);
            makeplotrange(fy, tWithDomain_y,tDomains_y, ymax, ymin, tmax, tmin);
            if(tWithDomain_x && tWithDomain_y) {
                for(uint i = 0; i < tDomains_x.size(); i++) {
                    for(uint j = 0; j < tDomains_y.size(); j++) {
                        if(!((tDomains_x[i]->min > tDomains_y[j]->max) || (tDomains_x[i]->max < tDomains_y[j]->min))) {
                            domain *d = new domain;
                            d->max = tDomains_x[i]->max < tDomains_y[j]->max ? tDomains_x[i]->max : tDomains_y[j]->max;
                            d->min = tDomains_x[i]->min > tDomains_y[j]->min ? tDomains_x[i]->min : tDomains_y[j]->min;
                            Domains.push_back(d);
                        }
                    }
                }
            } else if(tWithDomain_x) {
                for(uint i = 0; i < tDomains_x.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomains_x[i]->max;
                    d->min = tDomains_x[i]->min;
                    Domains.push_back(d);
                }
            } else if(tWithDomain_y) {
                for(uint i = 0; i < tDomains_y.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomains_y[i]->max;
                    d->min = tDomains_y[i]->min;
                    Domains.push_back(d);
                }
            }
            makePoints();
        }

        ParametricCurve::ParametricCurve(F1P *ffx, F1P *ffy, double start, double end, F1P *cff) {
            //    LineStrip* lineStrip = new LineStrip;
            //    lineStrips.add(lineStrip);
            arrayTtotal.clear();
            //fx = ffx;
            //fy = ffy;
            //tmin = start;
            //tmax = end;
            //spt = tmax - tmin;
            //color function
            cf = cff;
            isColorfulCurve = true;
            //makePoints();
            tWithDomain_x = false;
            tWithDomain_y = false;
            tDomains_x.clear();
            tDomains_y.clear();
            fx = ffx;
            fy = ffy;
            tmin = start;
            tmax = end;
            spt = tmax - tmin;
            makeplotrange(fx, tWithDomain_x,tDomains_x, xmax, xmin, tmax, tmin);
            makeplotrange(fy, tWithDomain_y,tDomains_y, ymax, ymin, tmax, tmin);
            if(tWithDomain_x && tWithDomain_y) {
                for(uint i = 0; i < tDomains_x.size(); i++) {
                    for(uint j = 0; j < tDomains_y.size(); j++) {
                        if(!((tDomains_x[i]->min > tDomains_y[j]->max) || (tDomains_x[i]->max < tDomains_y[j]->min))) {
                            domain *d = new domain;
                            d->max = tDomains_x[i]->max < tDomains_y[j]->max ? tDomains_x[i]->max : tDomains_y[j]->max;
                            d->min = tDomains_x[i]->min > tDomains_y[j]->min ? tDomains_x[i]->min : tDomains_y[j]->min;
                            Domains.push_back(d);
                        }
                    }
                }
            } else if(tWithDomain_x) {
                for(uint i = 0; i < tDomains_x.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomains_x[i]->max;
                    d->min = tDomains_x[i]->min;
                    Domains.push_back(d);
                }
            } else if(tWithDomain_y) {
                for(uint i = 0; i < tDomains_y.size(); i++) {
                    domain *d = new domain;
                    d->max = tDomains_y[i]->max;
                    d->min = tDomains_y[i]->min;
                    Domains.push_back(d);
                }
            }
            makePoints();
            useColorFunction();
        }

        ParametricCurve::~ParametricCurve() {
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
        }

			typedef struct {
				double tmax;
				double tmin;
			} trange;
        void ParametricCurve::makePoints() {
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
                LineStrip *linestrip;
                ArrayList<double> *arrayT;
                int i = 0;
                while(((x[i] < xmin) || (x[i] > xmax) || (y[i] < ymin) || (y[i] > ymax) || isNaN(x[i]) || isNaN(y[i])) && i < dataNum) i++;
                if (i == dataNum) {
                    continue;
                }
                linestrip = new LineStrip;
                arrayT = new ArrayList<double>;
                lineStrips.add(linestrip);
                arrayTtotal.add(arrayT);
                if(i == 0) {
                    linestrip->vert.add(new Point2d(x[i], y[i]));
                    (*arrayT).add(t[i]);
                } else {
                    double newt = getPointOnRangeFromLeft(fx, fy, t[i - 1], t[i], 1);
                    double newx = fx->getSingleData(newt);
                    double newy = fy->getSingleData(newt);
                    linestrip->vert.add(new Point2d(newx, newy));
                    (*arrayT).add(newt);
                    linestrip->vert.add(new Point2d(x[i], y[i]));
                    (*arrayT).add(t[i]);
                }
                i++;
                int state = 0;
                while(i < dataNum) {
                    if((x[i] < xmax) && (x[i] > xmin) && (y[i] > ymin) && (y[i] < ymax) && !isNaN(x[i]) && !isNaN(y[i])) {
                        if(state == 0) {
                            linestrip->vert.add(new Point2d(x[i], y[i]));
                            (*arrayT).add(t[i]);
                        } else {
                            linestrip = new LineStrip;
                            arrayT = new ArrayList<double>;
                            lineStrips.add(linestrip);
                            arrayTtotal.add(arrayT);
                            double newt = getPointOnRangeFromLeft(fx, fy, t[i - 1], t[i], 1);
                            double newx = fx->getSingleData(newt);
                            double newy = fy->getSingleData(newt);
                            linestrip->vert.add(new Point2d(newx, newy));
                            (*arrayT).add(newt);
                            linestrip->vert.add(new Point2d(x[i], y[i]));
                            (*arrayT).add(t[i]);
                            state = 0;
                        }
                    } else {
                        if(state == 0) {
                            double newt = getPointOnRangeFromRight(fx, fy, t[i - 1], t[i], 1);
                            double newx = fx->getSingleData(newt);
                            double newy = fy->getSingleData(newt);
                            linestrip->vert.add(new Point2d(newx, newy));
                            (*arrayT).add(newt);
                            state = 1;
                        } else {}
                    }
                    i++;
                }


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
            delete[] x;
            delete[] y;

        }

        double ParametricCurve::getPointOnRangeFromLeft(F1P *f_x, F1P *f_y, double t1, double t2, int depth) {
            if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
                return t2;
            double t = (t1 + t2) / 2;
            double x = f_x->getSingleData(t);
            double y = f_y->getSingleData(t);
            if (x < xmin || x > xmax || y < ymin || y > ymax || isNaN(y) || isNaN(x))
                return getPointOnRangeFromLeft(f_x, f_y, t, t2, depth + 1);
            else
                return getPointOnRangeFromLeft(f_x, f_y, t1, t, depth + 1);
        }

        double ParametricCurve::getPointOnRangeFromRight(F1P *f_x, F1P *f_y, double t1, double t2, int depth) {
            if (depth > MAX_GET_POINT_ON_RANGE_DEPTH)
                return t1;
            double t = (t1 + t2) / 2;
            double x = f_x->getSingleData(t);
            double y = f_y->getSingleData(t);
            if (x < xmin || x > xmax || y < ymin || y > ymax || isNaN(y) || isNaN(x))
                return getPointOnRangeFromRight(f_x, f_y, t1, t, depth + 1);
            else
                return getPointOnRangeFromRight(f_x, f_y, t, t2, depth + 1);
        }
        int ParametricCurve::getBaseNum() {
            return 20;
        }

        void ParametricCurve::addVerts(LineStrip* lineStrip, ArrayList<double> &arrayT) {
            ArrayList<double> kcaled;
            kcaled.add(getK(arrayT[0], dt) / graphScale);
            unsigned int i = 0;
            while (i < lineStrip->vert.size() - 1) {
                double dis = std::sqrt(((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) * ((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) + ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy) * ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy));
                double disx=arrayT[i + 1] - arrayT[i];
                if (dis < MIN_DELTA_DIS  || std::abs(disx)<=MIN_X_DIS) {
                    i++;
                    kcaled.add(getK(arrayT[i], dt) / graphScale);
                    continue;
                }
                double netk1 = kcaled[i];
                double netk2 = getK(arrayT[i+1], -dt) / graphScale;
                double dangle = std::acos((1 + netk1 * netk2) / std::sqrt(1 + netk1 * netk1) / std::sqrt(1 + netk2 * netk2));
                if (dangle > ADD_POINT_ANGLE) {
                    double t = (arrayT[i] + arrayT[i+1]) / 2;
                    double x = fx->getSingleData(t);
                    double y = fy->getSingleData(t);
                    if(!isNaN(x) && !isNaN(y)) {
                        lineStrip->vert.add(i + 1, new Point2d(x, y));
                        arrayT.add(i + 1, t);
                        if (x > xmax) xmax = x;
                        if (x < xmin) xmin = x;
                        if (y > ymax) ymax = y;
                        if (y < ymin) ymin = y;
                    } else {
                        i++;
                    }
                } else {
                    double t = (arrayT[i] + arrayT[i+1]) / 2;
                    double netk3 = getK(t, -dt) / graphScale;
                    if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) {
                        double t = (arrayT[i] + arrayT[i+1]) / 2;
                        double x = fx->getSingleData(t);
                        double y = fy->getSingleData(t);
                        if(!isNaN(x) && !isNaN(y)) {
                            lineStrip->vert.add(i + 1, new Point2d(x, y));
                            arrayT.add(i + 1, t);
                            if (x > xmax) xmax = x;
                            if (x < xmin) xmin = x;
                            if (y > ymax) ymax = y;
                            if (y < ymin) ymin = y;
                        } else {
                            i++;
                        }
                    } else {
                        i++;
                        kcaled.add(getK(arrayT[i], dt) / graphScale);
                    }
                }
            }
            spx = xmax - xmin;
            spy = ymax - ymin;
            graphScale = spy / spx;
        }

        void ParametricCurve::useColorFunction() {
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

        void ParametricCurve::makeplotrange(F1P *f, bool havenanrange, std::vector<domain *> &nanrangeset, double &ymax_f, double &ymin_f, double xmax_f, double xmin_f) {
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

        void ParametricCurve::detectnanborder(F1P *f, std::vector<domain *> &nanrangeset, std::vector<uint> &nannodesite,
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

        double ParametricCurve::getrightborder(F1P *f, double lx, double rx, uint deepth) {
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

        void ParametricCurve::getyrange(double &ymax_f, double &ymin_f, double &graphScale_f, double &spx_f, double &spy_f, 
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


    }
}
