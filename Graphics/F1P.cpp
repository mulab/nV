#include <nV/Number.h>
#include <nV/utils.h>
#include "F1P.h"
#include <limits>

namespace nV{
    namespace Graphics{
        //uint times = 0;
        //double timetotal = 0.0;
		F1P::F1P(Kernel& k, var ep, var pa) : kernel(k) {
			param = pa;
			expre = ep;
		}

		void F1P::getData(double* x, double* y, int num) {
            //times++;
            //timer t;
            //t.start();
			kernel.beginLocal();
            //var expretemp = kernel.eval(expre);
            //expretemp.print();
			for (int i = 0; i < num; ++i) {
                kernel.assign(param, new Real(x[i], 0));
                //var temp = kernel.eval(expre);
                //if(temp.isSymbol()) {
                //    y[i] = std::numeric_limits<double>().quiet_NaN();
                //    continue;
                //}
                //if (temp.isObject($.Real) || temp.isObject($.Integer)) {
                //    y[i] = toD(N(kernel, temp).object());
                //} else {
                //    y[i] = 0.0;
                //}
                //y[i] = toD(N(kernel, kernel.eval(expretemp)).object());
				var t = N(kernel, kernel.eval(expre));
				y[i] = t.isObject() ? toD(t.object()) : NaN;
            }
			kernel.endLocal();
            //t.end();
            //timetotal += t.value;
		}

		void F1P::getArrayData(double *x, double *y, int num) {
			getData(x, y, num);
		}

		double F1P::getSingleData(double input) {
			double x = input;
			double y;
			getData(&x, &y, 1);
			return y;
		}

		double F1P::getDerivative(double x, double dx) {
			double datay[2];
			double datax[2];
			datax[0] = x;
			datax[1] = x + dx;
			getData(datax, datay, 2);
			return (datay[1] - datay[0]) / dx;
		}

		double F1P::getSecondDerivative(double x, double dx) {
			double datay[3];
			double datax[3];
			datax[0] = x;
			datax[1] = x + dx;
			datax[2] = x + 2 * dx;
			getData(datax, datay, 3);
			return (datay[2] - 2*datay[1] + datay[0]) / (dx*dx);
		}

	}
}
