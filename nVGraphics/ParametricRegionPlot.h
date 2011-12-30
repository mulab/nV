#pragma once

#include "GraphicsMaker2D.h"
#include "ParametricRegion.h"
namespace nV {
namespace Graphics {
#define MAX_SCALE 
	#define MAX_DERIVATIVE 20
class ParametricRegionPlot :GraphicsMaker2D {

private:
    int polynum;
    IndexedQuadArray** polys;

    double umin, umax, vmin, vmax;

	//kernel
	Kernel &k;
public:
    ParametricRegionPlot(Kernel& k, const Tuple& expr);
    ~ParametricRegionPlot();
    var exportGraphics();
	void makePlotRange();
protected:
    void setRule(var title, var rule);
};
}
}