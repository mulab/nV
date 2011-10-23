#include "ContourPlot3D.h"
#include "nV/Number.h"

namespace nV {
    namespace Graphics {

ContourPlot3D::ContourPlot3D(Kernel &k, const Tuple &cmd) : k(k) {
    spacetype = NORMAL_SPACE;
    var expr = cmd[1];
    var xrange = cmd[2];
    var xparam = xrange.tuple()[1];
    xmin = toD(N(k, k.eval(xrange.tuple()[2])).object());
    xmax = toD(N(k, k.eval(xrange.tuple()[3])).object());
    var yrange = cmd[3];
    var yparam = yrange.tuple()[1];
    ymin = toD(N(k, k.eval(yrange.tuple()[2])).object());
    ymax = toD(N(k, k.eval(yrange.tuple()[3])).object());
    var zrange = cmd[4];
    var zparam = zrange.tuple()[1];
    zmin = toD(N(k, k.eval(zrange.tuple()[2])).object());
    zmax = toD(N(k, k.eval(zrange.tuple()[3])).object());
    if (cmd[cmd.size - 1].isTuple(SYS(Optional))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }

    //if(expr.isTuple($.List)) {
    //    surnum = expr.tuple().size - 1;
    //}else {
    //    surnum = 1;
    //    expr = list(expr);
    //}
    this->expression = expr;
    F3P *f = new F3P(k, expr, xparam, yparam, zparam);
    sur = new ImplicitSurface(f, xmin, xmax, ymin, ymax, zmin,zmax);
    delete f;
}

ContourPlot3D::~ContourPlot3D() {
    delete sur;
}

//make a meshed surface
var ContourPlot3D::exportGraphics() {
    var v = list(sur->m_vertices.size());
    for (uint i = 0; i < sur->m_vertices.size(); ++i) {
        v.tuple()[i + 1] = list(new Real(sur->m_vertices[i]->position.x, 0), new Real(sur->m_vertices[i]->position.y, 0), new Real(sur->m_vertices[i]->position.z, 0));
    }
    var tri = list(sur->m_triangleindices.size());
    for (uint i = 0; i < sur->m_triangleindices.size(); ++i) {
        tri.tuple()[i + 1] = new Integer((long)sur->m_triangleindices[i]);
    }
    var suroptions = list(list(0,SYS(LightedSurface)), SYS(Options));
    var triangle = list(tri, suroptions, SYS(TriangleArray));
    var directive = list(triangle, SYS(Directive));
    var complex = list(v, directive, SYS(GraphicsComplex));
    var shape = list(complex, SYS(Shape));
    var graphics = list(shape, makeGlobalOptions(), SYS(Graphics3D));
    return graphics;
}

void ContourPlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}

    }
}