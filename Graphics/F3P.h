#pragma once

#include <nV/Graphics.h>

#define LARGE 1000000

namespace nV {
    namespace Graphics {

class F3P {

    var expre, xparam, yparam, zparam;

    Kernel &k;

public:

    F3P(Kernel &k, var ep, var xpa, var ypa, var zpa);

    void getData(double* x, double* y, double *z, double *ans, int num);

    void getArrayData(double *x, double *y, double *z, double *ans, int num);

    double getSingleData(double inputx, double inputy, double inputz);

    double getDerivativeX(double x, double y, double z, double dx);

    double getDerivativeY(double x, double y, double z, double dy);

    double getDerivativeZ(double x, double y, double z, double dz);
};

    }
}