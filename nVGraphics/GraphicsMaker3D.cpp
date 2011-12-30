#include "GraphicsMaker3D.h"

namespace nV {
namespace Graphics {
GraphicsMaker3D::GraphicsMaker3D() {
    spacetype = NORMAL_SPACE;
}

var GraphicsMaker3D::makeGlobalOptions() {
    ArrayList<var> optlist;
    optlist.clear();
    var range = GraphicsMaker::makeRange(xmin, xmax, ymin, ymax, zmin, zmax);
    optlist.add(range);
    var spaceflag;
    if (spacetype == FLAT_SPACE) {
        spaceflag = tuple(SYS(FlatSpace));
    } else if (spacetype == NORMAL_SPACE) {
        spaceflag = tuple(SYS(NormalSpace));
    }
    if (spaceflag != 0)
        optlist.add(spaceflag);
    var opts = list(optlist.size(), SYS(Options));
    for (size_t i = 0; i < optlist.size(); i++) {
        opts.tuple()[i + 1] = optlist[i];
    }
    return opts;
}

var GraphicsMaker3D::makeGraphics(var shape, var opts) {
    return tuple(SYS(Graphics3D), shape, opts);
}

var GraphicsMaker3D::makeGraphics(var shape) {
    var opts = makeGlobalOptions();
    return makeGraphics(shape, opts);
}

void GraphicsMaker3D::setCommonRule(var title, var rule) {
}
}
}