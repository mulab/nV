#pragma once

#include "GraphicsMaker3D.h"
#include "ParametricSurface.h"

namespace nV {
namespace Graphics {
class ParametricSurfacePlot3D : GraphicsMaker3D {

private:
    int polynum;
    IndexedQuadArray3D** polys;
    double umin, umax, vmin, vmax, wmin, wmax;
	//kernel
	Kernel &k;
public:
    ParametricSurfacePlot3D(Kernel& k, const Tuple& expr);
    ~ParametricSurfacePlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
}
}
