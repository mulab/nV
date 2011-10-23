#include "PlotWithDomain.h"
namespace nV {
namespace Graphics {
	PlotWithDomain::PlotWithDomain(F1P *f1 ,std::vector<domain *> domains, double ymin, double ymax)
	{
		f=f1;
		this->Domains=domains;
		this->WithDomain=true;
        this->ymax = ymax;
        this->ymin = ymin;
		makePoints(domains);
	}
	PlotWithDomain::PlotWithDomain(F1P *f1 ,std::vector<domain *> domains,F1P *cf, double ymin, double ymax)
	{
		f=f1;
		this->Domains=domains;
		this->WithDomain=true;
        this->ymax = ymax;
        this->ymin = ymin;
		makePoints(domains);
		isColorfulCurve = true;
        this->cf = cf;
		useColorFunction();
	}
    void PlotWithDomain::makePoints(std::vector<domain *> domains)
    {
        dataNum = getBaseNum();
        double *x = new double[dataNum];
        double *y = new double[dataNum];
        for(std::vector<domain *>::iterator dt=domains.begin();dt!=domains.end();dt++)
        {
            double xmin=(*dt)->min;
            double xmax=(*dt)->max;
            //dataNum = getBaseNum() * ((xmax - xmin) / ((*(domains.end() - 1))->max - (*domains.begin())->min));

            for (int i = 0; i < dataNum; i++) {
                x[i] = xmin + (xmax - xmin) * i / (dataNum - 1);
            }
            f->getArrayData(x, y, dataNum);
            LineStrip* lineStrip;
            //add the first in-range point
            int i = 0;
            while (((y[i] < ymin) || (y[i] > ymax) || isNaN(y[i])) && i < dataNum)
                i++;
            if (i == dataNum) {
                return;
            }
            lineStrip = new LineStrip;
            lineStrips.add(lineStrip);
            if (i == 0) {
                lineStrip->vert.add(new Point2d(x[i], y[i]));
            } else { //xmin point was excluded
                double newx = getPointOnRangeFromLeft(x[i-1], x[i], 1);
                double newy = f->getSingleData(newx);
                lineStrip->vert.add(new Point2d(newx, newy));
                lineStrip->vert.add(new Point2d(x[i], y[i]));
            }
            i++;
            int state = 0;
            while (i < dataNum) {
                if ((y[i] > ymin) && (y[i] < ymax) && !isNaN(y[i])) {
                    if (state == 0) {
                        //line strip still going
                        lineStrip->vert.add(new Point2d(x[i], y[i]));
                    } else {
                        //make a new line strip
                        lineStrip = new LineStrip;
                        lineStrips.add(lineStrip);
                        double newx = getPointOnRangeFromLeft(x[i-1], x[i], 1);
                        double newy = f->getSingleData(newx);
                        lineStrip->vert.add(new Point2d(newx, newy));
                        lineStrip->vert.add(new Point2d(x[i], y[i]));
                        state = 0;
                    }
                } else {
                    if (state == 0) {
                        //a line strip ends
                        double newx = getPointOnRangeFromRight(x[i-1], x[i], 1);
                        double newy = f->getSingleData(newx);
                        lineStrip->vert.add(new Point2d(newx, newy));
                        state = 1;
                    } else {
                        //do nothing
                    }
                }
                i++;
            }

            spx = xmax - xmin;
            spy = ymax - ymin;
            graphScale = spy / spx;
            dx = spx / LARGE;
            for (unsigned int i = 0; i < lineStrips.size(); i++)
                addVerts(lineStrips[i]);
        }
        delete []x;
        delete []y;


    }
	PlotWithDomain::~PlotWithDomain()
	{}
}
}