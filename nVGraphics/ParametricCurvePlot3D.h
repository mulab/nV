#pragma once

#include "GraphicsMaker3D.h"
#include "ParametricCurve3D.h"

namespace nV {
    namespace Graphics {

class ParametricCurvePlot3D : GraphicsMaker3D {

private:
    ParametricCurve3D *curve;
    double tmin, tmax;

    Kernel &k;
    var expression;

public:
    ParametricCurvePlot3D(Kernel &k, Tuple &expr);
    ~ParametricCurvePlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};

    }
}