#include "ContourPlot.h"
#include <nV/Number.h>

namespace nV {
    namespace Graphics {
ContourPlot::ContourPlot(Kernel &k, const Tuple &cmd) :k(k) {
    var expr = cmd[1];
    //expr.print();
    var xrange = cmd[2];
    var yrange = cmd[3];
    var xparam = xrange.tuple()[1];
    //xparam.print();
    xmin = toD(N(k, k.eval(xrange.tuple()[2])).object());
    xmax = toD(N(k, k.eval(xrange.tuple()[3])).object());
    var yparam = yrange.tuple()[1];
    //yparam.print();
    ymin = toD(N(k, k.eval(yrange.tuple()[2])).object());
    ymax = toD(N(k, k.eval(xrange.tuple()[3])).object());
    if (cmd[cmd.size - 1].isTuple(SYS(Options))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }

    if(expr.isTuple($.List)) {
        curnum = expr.tuple().size - 1;
    }else {
        curnum = 1;
        expr = list(expr);
    }
    this->expression = expr;

    for(uint i = 0; i < curnum; i++) {
        F2P *f = new F2P(k, expr.tuple()[i + 1], xparam, yparam);
        cur = new ImplicitCurve(f, xmin, xmax, ymin, ymax);
        delete f;
    }
}

ContourPlot::~ContourPlot() {
    delete cur;
}

var ContourPlot::exportGraphics() {
    //vert list
    var v = list(cur->m_vertices.size());
    for(uint i = 0; i < cur->m_vertices.size(); i++) {
        v.tuple()[i + 1] = list(new Real(cur->m_vertices[i]->position.x, 0), new Real(cur->m_vertices[i]->position.y, 0));
    }
    var lines = list(cur->m_line.size() /  2 + 1, SYS(Line));
    for(uint i = 0; i < cur->m_line.size() / 2; i++) {
        lines.tuple()[i + 1] = list(new Integer((long)cur->m_line[2 * i]), new Integer((long)cur->m_line[2 * i + 1]));
    }
    Hue *hue = getHue(0);
    var opt = makeHueOption(hue);
    lines.tuple()[cur->m_line.size() / 2 + 1] = opt;
    var shape = list(list(v, list(lines, SYS(Directive)), SYS(GraphicsComplex)), SYS(Shape));
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ContourPlot::setRule(var title, var rule) {
    setCommonRule(title, rule);
}

    }
}
