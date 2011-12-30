#pragma once

#include "GraphicsMaker2D.h"
#include "PolarCurve.h"

namespace nV {
    namespace Graphics {

class PolarPlot : GraphicsMaker2D {

private:
    int curnum;
    Curve **curs;
    double tmin, tmax;

    //change by options
    bool colorFunctionSet;
    F1P* cf;
    var tparam;

    Kernel &k;

    var expression;

public:
    PolarPlot(Kernel &k, const Tuple &expr);
    ~PolarPlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};

    }
}