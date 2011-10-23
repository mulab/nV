#pragma once

#include "GraphicsMaker2D.h"
#include "ParametricCurve.h"
namespace nV {
namespace Graphics {
class ParametricPlot : GraphicsMaker2D {

private:
    int curnum;
    Curve **curs;
    double tmin, tmax;

    //change by options
    bool colorFunctionSet;
    F1P* cf;
    var tparam;

    Kernel &k;

    //expression
    var expression;
public:
    ParametricPlot(Kernel &k, const Tuple &expr);
    ~ParametricPlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};

}
}