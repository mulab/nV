#pragma once

#include "GraphicsMaker3D.h"
#include "F3P.h"
#include "ImplicitSurface.h"

namespace nV {
    namespace Graphics {

class ContourPlot3D : GraphicsMaker3D {
private:
    //int surnum;
    ImplicitSurface *sur;

    Kernel &k;
    var expression;

public:
    ContourPlot3D(Kernel &k, const Tuple &expr);
    ~ContourPlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};

    }
}