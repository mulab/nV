#include <nV/Number.h>
#include "F2P.h"
#include <limits>

namespace nV {
namespace Graphics {

F2P::F2P(Kernel &k, var ep, var xpa, var ypa) : k(k) {
    xparam = xpa;
    yparam = ypa;
    expre = ep;
}

void F2P::getData(double* x, double* y, double *z, int num) {
    k.beginLocal();
    for(uint i = 0; i < num; i++) {
        k.assign(xparam, new Real(x[i], 0));
        k.assign(yparam, new Real(y[i], 0));
        //var temp = k.eval(expre);
        //if(temp.isSymbol()) {
        //    z[i] = std::numeric_limits<double>().quiet_NaN();
        //    continue;
        //}
        //z[i] = toD(N(k, temp).object());
		var t = N(k, k.eval(expre));
		z[i] = t.isObject() ? toD(t.object()) : NaN;
    }
    k.endLocal();
}

void F2P::getArrayData(double *x, double *y, double *z, int num) {
    getData(x, y, z, num);
}

double F2P::getSingleData(double inputx, double inputy) {
    double x = inputx;
    double y = inputy;
    double z;
    getData(&x, &y, &z, 1);
    return z;
}

double F2P::getDerivativeX(double x, double y, double dx) {
    double datax[2];
    double datay[2];
    double dataz[2];
    datax[0] = x;
    datax[1] = x + dx;
    datay[0] = y;
    datay[1] = y;
    getData(datax, datay, dataz, 2);
    return (dataz[1] - dataz[0]) / dx;
}

double F2P::getDerivativeY(double x, double y, double dy) {
    double datax[2];
    double datay[2];
    double dataz[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y + dy;
    getData(datax, datay, dataz, 2);
    return (dataz[1] - dataz[0]) / dy;
}

}
}
