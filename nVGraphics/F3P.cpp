#include "nV/Number.h"
#include "F3P.h"
#include <limits>

namespace nV {
    namespace Graphics {

F3P::F3P(Kernel &k, var ep, var xpa, var ypa, var zpa) :k(k) {
    xparam = xpa;
    yparam = ypa;
    zparam = zpa;
    expre = ep;
}

void F3P::getData(double* x, double* y, double *z, double *ans, int num) {
    for (uint i = 0; i < num; ++i) {
        k.assign(xparam, new Real(x[i], 0));
        k.assign(yparam, new Real(y[i], 0));
        k.assign(zparam, new Real(z[i], 0));
        //var temp = k.eval(expre);
        //if (temp.isSymbol()) {
        //    ans[i] = std::numeric_limits<double>().quiet_NaN();
        //    continue;
        //}
        //ans[i] = toD(N(k, temp).object());
		var t = N(k, k.eval(expre));
		ans[i] = t.isObject() ? toD(t.object()) : NaN;
    }
}

void F3P::getArrayData(double *x, double *y, double *z, double *ans, int num) {
    getData(x, y, z, ans, num);
}

double F3P::getSingleData(double inputx, double inputy, double inputz) {
    double x = inputx;
    double y = inputy;
    double z = inputz;
    double ans;
    getData(&x, &y, &z, &ans, 1);
    return ans;
}

double F3P::getDerivativeX(double x, double y, double z, double dx) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x + dx;
    datay[0] = y;
    datay[1] = y;
    dataz[0] = z;
    dataz[1] = z;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dx;
}

double F3P::getDerivativeY(double x, double y, double z, double dy) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y + dy;
    dataz[0] = z;
    dataz[1] = z;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dy;
}

double F3P::getDerivativeZ(double x, double y, double z, double dz) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y;
    dataz[0] = z;
    dataz[1] = z + dz;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dz;
}

    }
}
