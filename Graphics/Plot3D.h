#pragma once

#include "GraphicsMaker3D.h"
#include "Surface.h"

namespace nV {
namespace Graphics {
class Plot3D : GraphicsMaker3D {
private:
    int surnum;
    Surface **sur;

    Kernel &k;

public:
    Plot3D(Kernel &k, const Tuple &expr);
    ~Plot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};

}
}
