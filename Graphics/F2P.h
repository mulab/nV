#pragma once

#include "nV/Graphics.h"

#define LARGE 1000000

namespace nV {
namespace Graphics {

class F2P {

    var expre, xparam, yparam;

    Kernel &k;

public:

    F2P(Kernel &k,var ep, var xpa, var ypa);

    void getData(double* x, double* y, double *z, int num);

    void getArrayData(double *x, double *y, double *z, int num);

    double getSingleData(double inputx, double inputy);

    double getDerivativeX(double x, double y, double dx);

    double getDerivativeY(double x, double y, double dy);
};

}
}