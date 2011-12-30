#include "ParametricCurvePlot3D.h"
#include "nV/Number.h"

namespace nV {
    namespace Graphics {

ParametricCurvePlot3D::ParametricCurvePlot3D(Kernel &k, Tuple &cmd) :k(k) {
    spacetype = NORMAL_SPACE;
    var expr = cmd[1];
    var trange = cmd[2];
    var tparam = trange.tuple()[1];
    tmin = toD(N(k, k.eval(trange.tuple()[2])).object());
    tmax = toD(N(k, k.eval(trange.tuple()[3])).object());

    if (cmd[cmd.size - 1].isTuple(SYS(Optional))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }
    this->expression = expr;
    F1P *fx = new F1P(k, expr.tuple()[1], tparam);
    F1P *fy = new F1P(k, expr.tuple()[2], tparam);
    F1P *fz = new F1P(k, expr.tuple()[3], tparam);
    curve = new ParametricCurve3D(fx, fy, fz, tmin, tmax);
    delete fx;
    delete fy;
    delete fz;
    xmin = curve->xmin;
    xmax = curve->xmax;
    ymin = curve->ymin;
    ymax = curve->ymax;
    zmin = curve->zmin;
    zmax = curve->zmax;
}

ParametricCurvePlot3D::~ParametricCurvePlot3D() {
    delete curve;
}

var ParametricCurvePlot3D::exportGraphics() {
    Hue *hue = getHue(0);
    //var shapeexpr = list(makeLine(curve, hue), SYS(Shape));
    var shapeexpr = list(curve->lineStrips.size(), SYS(Shape));
    for (uint i = 0; i < curve->lineStrips.size(); ++i) {
        var v = list(curve->lineStrips[i]->vert.size());
        for (size_t j = 0; j < curve->lineStrips[i]->vert.size(); ++j) {
            v.tuple()[j + 1] = tuple($.List, new Real(curve->lineStrips[i]->vert[j].x, 0), new Real(curve->lineStrips[i]->vert[j].y, 0), new Real(curve->lineStrips[i]->vert[j].z, 0));
        }
        shapeexpr.tuple()[i + 1] = tuple(SYS(Line), v, makeHueOption(hue));
    }
    delete hue;
    return list(shapeexpr, makeGlobalOptions(), SYS(Graphics3D));
    //var shapelist = Vec(1);
    //Hue* hue = getHue(0);
    //At(shapelist, 0) = makeLine(curve, hue);
    //delete hue;
    //var shapeexpr = Ex(Sym(L"Shape"), shapelist);
    //var options = makeGlobalOptions();
    //var graphics = Ex(TAG(Graphics3D), Vec(shapeexpr, options));
    //return graphics;
}

void ParametricCurvePlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}


    }
}