#include "nV/Number.h"
#include "PolarPlot.h"

namespace nV {
    namespace Graphics {

PolarPlot::PolarPlot(Kernel &k, const Tuple &cmd) :k(k) {
    colorFunctionSet = false;
    var expr = cmd[1];
    var trange = cmd[2];
    tparam = trange.tuple()[1];
    tmin = toD(N(k, k.eval(trange.tuple()[2])).object());
    tmax = toD(N(k, k.eval(trange.tuple()[3])).object());

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

    curs = new Curve*[curnum];
    for (int i = 0; i < curnum; i++) {
        F1P *f2d = new F1P(k, expr.tuple()[i + 1], tparam);
        if (!colorFunctionSet)
            curs[i] = (Curve*)(new PolarCurve(f2d, tmin, tmax));
        else
            curs[i] = (Curve*)(new PolarCurve(f2d, tmin, tmax, cf));
        delete f2d;
    }

    if (colorFunctionSet)
        delete cf;
    xmin = curs[0]->xmin;
    xmax = curs[0]->xmax;
    ymin = curs[0]->ymin;
    ymax = curs[0]->ymax;
    for (int i = 1; i < curnum; i++) {
        if (curs[i]->ymin < ymin)
            ymin = curs[i]->ymin;
        if (curs[i]->ymax > ymax)
            ymax = curs[i]->ymax;
        if (curs[i]->xmin < xmin)
            xmin = curs[i]->xmin;
        if (curs[i]->xmax > xmax)
            xmax = curs[i]->xmax;
    }
}

PolarPlot::~PolarPlot() {
    for (int i = 0; i < curnum; i++) {
        delete curs[i];
    }
    delete []curs;
}

var PolarPlot::exportGraphics() {
    var shapelist;
    if (!colorFunctionSet) {
        shapelist = list(curnum, SYS(Shape));
        //shapelist = Vec(curnum);
        for (int i = 0; i < curnum; i++) {
            Hue* hue = getHue(i);
            shapelist.tuple()[i + 1] = makeLine(curs[i], hue);
            //At(shapelist, i) = makeLine(curs[i], hue);
            delete hue;
        }
    } else {
        var temp = tuple(curnum);
        uint size_temp = 0;
        for (uint i = 0; i < curnum; i++) {
            temp.tuple()[i] = makeColorfulLines(curs[i]);
            size_temp += (temp.tuple()[i].tuple().size - 1);
        }

        uint totalnum = 1;
        if(size_temp == 0) {
        }else {
            shapelist = list(size_temp, SYS(Shape));
            for(uint i = 0; i < curnum; ++i) {
                for(uint j = 0; j < ((temp.tuple()[i]).tuple()).size - 1; ++j) {
                    shapelist.tuple()[totalnum] = temp.tuple()[i].tuple()[j + 1];
                    ++totalnum;
                }
            }
        }

    }
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shapelist, opts);
    return graphics;
}

void PolarPlot::setRule(var title, var rule) {
    if (title == SYS(ColorFunction)) { //has color function
        colorFunctionSet = true;
        //var fun = rule;
        cf = new F1P(k, rule, tparam);
    }
    setCommonRule(title, rule);
}

    }
}
