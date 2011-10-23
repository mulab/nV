#include "singularityWithPlotRange.h"

namespace nV {
	namespace Graphics {
		sigularityWithPlotRange::sigularityWithPlotRange(F1P *f1, double xmin, double xmax, double ymin, double ymax) {
			singularity = true;
			Curve::uninitsingularityset();
			f = f1;		
			this->xmin = xmin;
			this->xmax = xmax;
			spx = xmax - xmin;
			makePlotRange();
			this->ymin = ymin;
			this->ymax = ymax;
			spy = ymax - ymin;
			CommonCurveWithPlotRange::makePoints();
		}

		sigularityWithPlotRange::sigularityWithPlotRange(F1P *f1, double xmin, double xmax, double ymin, double ymax, F1P *cf1) {
			singularity = true;
			Curve::uninitsingularityset();
			f = f1;
			this->xmin = xmin;
			this->xmax = xmax;
			spx = xmax - xmin;
			makePlotRange();
			this->ymin = ymin;
			this->ymax = ymax;
			spy = ymax - ymin;
			CommonCurveWithPlotRange::makePoints();
			isColorfulCurve = true;
			cf = cf1;
			useColorFunction();
		}

		sigularityWithPlotRange::sigularityWithPlotRange(F1P *f1, double xmin, double xmax) {
			singularity = true;
			Curve::uninitsingularityset();
			f = f1;
			this->xmin = xmin;
			this->xmax = xmax;
			spx = xmax - xmin;
			makePlotRange();
			CommonCurveWithPlotRange::makePoints();
		}

		sigularityWithPlotRange::sigularityWithPlotRange(F1P *f1, double xmin, double xmax, F1P *cf1) {
			singularity = true;
			Curve::uninitsingularityset();
			f = f1;
			this->xmin = xmin;
			this->xmax = xmax;
			spx = xmax - xmin;
			makePlotRange();
			CommonCurveWithPlotRange::makePoints();
			isColorfulCurve = true;
			cf = cf1;
			useColorFunction();
		}

		sigularityWithPlotRange::~sigularityWithPlotRange() {
		}


		void sigularityWithPlotRange::makePlotRange() {
			int bigDataNum = 500;
			double *x = new double[bigDataNum];
			double *y = new double[bigDataNum];
			for (int i = 0; i < bigDataNum; i++) {
				x[i] = xmin + (xmax - xmin) * i / (bigDataNum - 1);
			}
			f->getArrayData(x, y, bigDataNum);
			ArrayList<Point2d*> vert;
			ArrayList<double> d;
			for (int i = 0; i < bigDataNum; i++) {
				vert.add(new Point2d(x[i], y[i]));
			}
			delete x;
			delete y;
			ymin = vert[0]->y;
			ymax = vert[0]->y;
			for (uint i = 0; i < vert.size(); i++) {
				if (vert[i]->y > ymax) ymax = vert[i]->y;
				if (vert[i]->y < ymin) ymin = vert[i]->y;
			}
			spx = xmax - xmin;
			spy = ymax - ymin;
			graphScale = spy / spx;
			dx = spx / LARGE;
			for (unsigned int i = 0; i < vert.size(); i++) {
				d.add(f->getDerivative(vert[i]->x, dx));
			}

			bool vrm = true; //vert removed
			bool *flag = new bool[vert.size()];
			for(uint i = 0; i < vert.size(); i++) {
				flag[i] = false;
			}
			bool *pflag = flag;

			int count = 0;
			while (vrm) {
				vrm = false;
				for (unsigned int i = 0; i < vert.size(); i++) {
					if(!flag[i]) {
						if (std::abs(d[i]) / graphScale > MAX_DERIVATIVE) {
							//vert.remove(i);
							//d.remove(i);
							flag[i] = true;
							vrm = true;
						}
					}
				}
				for(uint i = 0; i < vert.size(); i++) {
					if(!flag[i]) {
						ymin = vert[i]->y;
						ymax = vert[i]->y;
						break;
					}
				}
				for (uint i = 0; i < vert.size(); i++) {
					if(!flag[i]) {
						if (vert[i]->y > ymax) ymax = vert[i]->y;
						if (vert[i]->y < ymin) ymin = vert[i]->y;
					}
				}
				spy = ymax - ymin;
				graphScale = spy / spx;
                if(graphScale < MIN_GRAPHSCALE_MINIMUM) {
                    break;
                }
				if(count >= MAX_DELETE_AMOUNT) {
					break;
				}
				count++;
			}

			singularitynode *temp;
			if(flag[0]) {
				temp = new singularitynode;
				temp->x = xmin;
				uint m = 1;
				for(; m < vert.size() - 1 && flag[m]; m++) {
				}

				if(m != vert.size() - 1) {
					temp->lefty = vert[m-1]->y;
					temp->leftd = d[m-1];
					temp->righty = vert[m-1]->y;
					temp->rightd = d[m-1];
				}else {
					temp->lefty = vert[0]->y;
					temp->leftd = d[0];
					temp->righty = vert[0]->y;
					temp->rightd = d[0];
				}
				singularityset.push_back(temp);
			}

			uint k = 0;

			for(; k < vert.size() - 2; k++) {
				if(!flag[k] && flag[k + 1]) {
					uint j = k + 1;
					for(; j < vert.size() - 1; j++) {
						if(flag[j] && !flag[j + 1]) {
							temp = new singularitynode;
							temp->x = (vert[k]->x + vert[j + 1]->x) / 2.0;
							temp->lefty = vert[k]->y;
							temp->leftd = d[k];
							temp->righty = vert[j + 1]->y;
							temp->rightd = d[j + 1];
							singularityset.push_back(temp);
							k = j;
							break;
						}
					}
					if(j == (vert.size() - 2) && flag[vert.size() - 1]) {
						temp = new singularitynode;
						temp->x = vert[vert.size() - 1]->x;
						temp->lefty = vert[k]->y;
						temp->leftd = d[k];
						temp->righty = vert[k]->y;
						temp->rightd = d[k];
						singularityset.push_back(temp);
					}
				}
			}

			delete [] flag;
			for(uint i = 0; i < singularityset.size(); i++) {
				std::cout << singularityset[i]->x << ' ' << singularityset[i]->leftd << ' ' << singularityset[i]->rightd << std::endl;
			}

            if(graphScale > MAX_GRAPHSCALE) {
                double y_temp = (ymax + ymin) / 2.0;
                spy = MAX_GRAPHSCALE * spx;
                ymax = y_temp + std::abs(spy / 2);
                ymin = y_temp - std::abs(spy / 2);
            } else if(graphScale < MIN_GRAPHSCALE) {
                double y_temp = (ymax + ymin) / 2.0;
                spy = MIN_GRAPHSCALE * spx;
                ymax = y_temp + std::abs(spy / 2);
                ymin = y_temp - std::abs(spy / 2);
            }
			//if(vert[0]->x != xmin) {
			//		singularityset.push_back(xmin);
			//}

			//for(uint i = 0; i < vert.size() - 1; i++) {
			//	if(std::abs(vert[i + 1]->x - vert[i]->x) != ((xmax - xmin) / (bigDataNum - 1))) {
			//		singularityset.push_back((vert[i]->x + vert[i + 1]->x) / 2.0);
			//	}
			//}

			//if(vert[vert.size() - 1]->x != (xmin + (xmax - xmin) * (bigDataNum - 1) / (bigDataNum - 1))) {
			//		singularityset.push_back(xmax);
			//}
			//uint z = 0;
			//if(std::abs(d[z]) / graphScale > MAX_DERIVATIVE) {
			//	singularityset.push_back(xmin);
			//	while(z < vert.size()) {
			//		if(std::abs(d[z]) / graphScale > MAX_DERIVATIVE) {
			//			vert.remove(z);
			//			d.remove(z);

			//			ymin = vert[0]->y;
			//			ymax = vert[0]->y;
			//			for (uint i = 0; i < vert.size(); i++) {
			//				if (vert[i]->y > ymax) ymax = vert[i]->y;
			//				if (vert[i]->y < ymin) ymin = vert[i]->y;
			//			}
			//			spy = ymax - ymin;
			//			graphScale = spy / spx;

			//		}else {
			//			break;
			//		}
			//	}
			//}
			//
			//


			//for (; z < vert.size() - 1; z++) {
			//	if (std::abs(d[z]) / graphScale <= MAX_DERIVATIVE && std::abs(d[z + 1]) / graphScale > MAX_DERIVATIVE) {
			//		uint j = z + 1;
			//		while(j < vert.size()) {
			//			if(std::abs(d[j]) / graphScale > MAX_DERIVATIVE) {
			//				vert.remove(j);
			//				d.remove(j);

			//				ymin = vert[0]->y;
			//				ymax = vert[0]->y;
			//				for (uint i = 0; i < vert.size(); i++) {
			//					if (vert[i]->y > ymax) ymax = vert[i]->y;
			//					if (vert[i]->y < ymin) ymin = vert[i]->y;
			//				}
			//				spy = ymax - ymin;
			//				graphScale = spy / spx;

			//			}else {
			//				singularityset.push_back((vert[z]->x + vert[j]->x) / 2.0);
			//				break;
			//			}
			//		}
			//		if(j == vert.size()) {
			//			singularityset.push_back(xmax);
			//		}
			//		//for(uint j = i; j > 0; j--) {
			//		//	if(std::abs(d[j]) / graphScale > MAX_DERIVATIVE) {
			//		//	}
			//		//}
			//	}	
			//}
			//ymin = vert[0]->y;
			//ymax = vert[0]->y;
			//for (uint i = 0; i < vert.size(); i++) {
			//	if (vert[i]->y > ymax) ymax = vert[i]->y;
			//	if (vert[i]->y < ymin) ymin = vert[i]->y;
			//}
			//spy = ymax - ymin;
			//graphScale = spy / spx;

		}

	}
}