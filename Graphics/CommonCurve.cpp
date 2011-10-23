#include "CommonCurve.h"

namespace nV{
	namespace Graphics{

		CommonCurve::CommonCurve() {
		}

		CommonCurve::CommonCurve(F1P *f1, double start, double end) {
			LineStrip* lineStrip = new LineStrip;
			lineStrips.add(lineStrip);
			f = f1;
			xmin = start;
			xmax = end;
			spx = xmax - xmin;
			makePoints();
		}

		CommonCurve::CommonCurve(F1P *f1, double start, double end, F1P *cf1) {
			LineStrip* lineStrip = new LineStrip;
			lineStrips.add(lineStrip);
			f = f1;
			xmin = start;
			xmax = end;
			spx = xmax - xmin;
			cf = cf1;
			isColorfulCurve = true;
			makePoints();
			useColorFunction();
		}

		CommonCurve::~CommonCurve() {
		}

		void CommonCurve::makePoints() {
			dataNum = getBaseNum();
			double *x = new double[dataNum];
			double *y = new double[dataNum];
			for (int i = 0; i < dataNum; i++) {
				//split the domain into dataNum-1 parts
				x[i] = xmin + (xmax - xmin) * i / (dataNum - 1);
			}
			f->getArrayData(x, y, dataNum);
			for (int i = 0; i < dataNum; i++) {
				lineStrips[0]->vert.add(new Point2d(x[i], y[i]));
			}
			delete x;
			delete y;
			ymin = lineStrips[0]->vert[0]->y;
			ymax = lineStrips[0]->vert[0]->y;
			for (int i = 0; i < dataNum; i++) {
				if (lineStrips[0]->vert[i]->y > ymax) ymax = lineStrips[0]->vert[i]->y;
				if (lineStrips[0]->vert[i]->y < ymin) ymin = lineStrips[0]->vert[i]->y;
			}
			spx = xmax - xmin;
			spy = ymax - ymin;
			graphScale = spy / spx;
			dx = spx / LARGE;
			addVerts(lineStrips[0]);
		}

		void CommonCurve::addVerts(LineStrip* lineStrip) {
			//add points where the angle between two lines is too large
			ArrayList<double> kcaled; //save the k calculated
            double derivative = f->getDerivative(lineStrip->vert[0]->x, dx);
            if(!isNaN(derivative)) {
                kcaled.add(derivative / graphScale); //get k0, kcaled是一阶导数
            } else {
                kcaled.add(MAX_DERIVATIVE);
            }
			unsigned int i = 0;
			while (i < lineStrip->vert.size() - 1) {
				double dis = std::sqrt(((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) * ((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) + ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy) * ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy));
                double disx=lineStrip->vert[i+1]->x - lineStrip->vert[i]->x;
				if (dis < MIN_DELTA_DIS || std::abs(disx)<=MIN_X_DIS) {
					i++;
                    derivative = f->getDerivative(lineStrip->vert[i]->x, dx);
                    if(!isNaN(derivative)) {
                        kcaled.add(derivative / graphScale); //get ki, kcaled是一阶导数
                    } else {
                        kcaled.add(MAX_DERIVATIVE);
                    }
					continue;  //line is too short
				}
				double netk1 = kcaled[i];
                derivative = f->getDerivative(lineStrip->vert[i + 1]->x, -dx);
                double netk2;
                if(!isNaN(derivative)) {
                    netk2 = f->getDerivative(lineStrip->vert[i+1]->x, -dx) / graphScale; //always divided by graph scale
                } else {
                    netk2 = MAX_DERIVATIVE;
                }
				double dangle = std::acos((1 + netk1 * netk2) / std::sqrt(1 + netk1 * netk1) / std::sqrt(1 + netk2 * netk2));
				if (dangle > ADD_POINT_ANGLE) {	//need to add points
					double x = (lineStrip->vert[i]->x + lineStrip->vert[i+1]->x) / 2;
					double y = f->getSingleData(x);
                    if(!isNaN(y)) {
                        lineStrip->vert.add(i + 1, new Point2d(x, y));
                        if (y > ymax) ymax = y;
                        if (y < ymin) ymin = y;
                    } else {
                        i++;
                    }
				} else { //try adding points
                    double x = (lineStrip->vert[i]->x + lineStrip->vert[i+1]->x) / 2;
                    double net3y=f->getSingleData(x);
                    if(isNaN(net3y)) {
                        i++;
                        continue;
                    }
                    double dis1 = std::sqrt(((x - lineStrip->vert[i]->x) / spx) * ((x - lineStrip->vert[i]->x) / spx) + ((net3y - lineStrip->vert[i]->y) / spy) * ((net3y - lineStrip->vert[i]->y) / spy));
                    double dis2 = std::sqrt(((lineStrip->vert[i+1]->x - x) / spx) * ((lineStrip->vert[i+1]->x - x) / spx) + ((lineStrip->vert[i+1]->y - net3y) / spy) * ((lineStrip->vert[i+1]->y - net3y) / spy));
                    double netk3 = f->getDerivative(x, dx) / graphScale;
                    if(isNaN(netk3)) {
                        netk3 = MAX_DERIVATIVE;
                    }
                    double disx1=std::abs(x - lineStrip->vert[i]->x);
                    double disx2=std::abs(x - lineStrip->vert[i+1]->x);

					if (((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) && (dis1>MIN_DELTA_DIS&&dis2>MIN_DELTA_DIS) && (disx1>MIN_X_DIS&&disx2>MIN_X_DIS)) {
						double y = f->getSingleData(x);
                        if(!isNaN(y)) {
                            lineStrip->vert.add(i + 1, new Point2d(x, y));
                            if (y > ymax) ymax = y;
                            if (y < ymin) ymin = y;
                        } else {
                            i++;
                        }
					} else {
						i++;
                        derivative = f->getDerivative(lineStrip->vert[i]->x, dx);
                        if(!isNaN(derivative)) {
                            kcaled.add(derivative / graphScale);
                        } else {
                            kcaled.add(MAX_DERIVATIVE);
                        }
					}
				}
			}
			spx = xmax - xmin;
			spy = ymax - ymin;
			graphScale = spy / spx;
		}

		int CommonCurve::getBaseNum() {
			//should not be to large
			return 20;
		}

		void CommonCurve::useColorFunction() {
			double cfmin, cfmax;
			cfmin = cf->getSingleData(lineStrips[0]->vert[0]->x);
			cfmax = cfmin;
			for (unsigned int i = 0; i < lineStrips.size(); i++) {
				LineStrip* lineStrip = lineStrips[i];
				for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
					double tmp = cf->getSingleData(lineStrip->vert[j]->x);
					if (tmp > cfmax)
						cfmax = tmp;
					if (tmp < cfmin)
						cfmin = tmp;
				}
			}
			for (unsigned int i = 0; i < lineStrips.size(); i++) {
				LineStrip* lineStrip = lineStrips[i];
				for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
					double tmp = cf->getSingleData(lineStrip->vert[j]->x);
                    if(isNaN(cfmax) || isNaN(cfmin) || isNaN(tmp) || (cfmax == cfmin)) {
                        lineStrip->color.add(j, getRainbowColor(1));
                        continue;
                    }
					lineStrip->color.add(j, getRainbowColor((tmp - cfmin) / (cfmax - cfmin)));
				}
			}
		}

	}
}
