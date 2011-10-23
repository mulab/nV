#include <nV/Number.h>
#include "ParametricRegionPlot.h"

namespace nV {
namespace Graphics {
ParametricRegionPlot::ParametricRegionPlot(Kernel& k, const Tuple& cmd) : k(k) {
	//cmd.print();
    var expr = cmd[1];
    var urange = cmd[2];
    var vrange = cmd[3];
    if (cmd[cmd.size - 1].isTuple(SYS(Options))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }
	var uparam = urange.tuple()[1];
    umin =toD(N(k, k.eval(urange.tuple()[2])).object());
    umax =toD(N(k, k.eval(urange.tuple()[3])).object());
	//uparam.print();
	//cout<<umin<<" "<<umax;
    var vparam = vrange.tuple()[1];
    vmin = toD(N(k, k.eval(vrange.tuple()[2])).object());
    vmax =toD(N(k, k.eval(vrange.tuple()[3])).object());
	//vparam.print();
	//cout<<vmin<<" "<<vmax;
	if (expr.isTuple($.List)&&expr.tuple()[expr.tuple().size-1].isTuple($.List)) {
		polynum = expr.tuple().size-1;
        polys = new IndexedQuadArray*[polynum];
        for (int i = 0; i < polynum; i++) {
            //var funs = At(expr, i);
			//expr.tuple()[i+1].tuple()[1].print();
			//cout<<endl;
			//expr.tuple()[i+1].tuple()[2].print();
			//cout<<endl;
			F2P *fx = new F2P(k, expr.tuple()[i + 1].tuple()[1], uparam,vparam);
            F2P *fy = new F2P(k, expr.tuple()[i + 1].tuple()[2], uparam,vparam);
            polys[i] = (IndexedQuadArray*)(new ParametricRegion(fx, fy, umin, umax, vmin, vmax));
            delete fx;
            delete fy;
        }
    } else {
        polynum = 1;
        polys = new IndexedQuadArray*[1];
        var funs = expr;
		//expr.print();
        F2P *fx = new F2P(k, expr.tuple()[1], uparam,vparam);
		
		//expr.tuple()[1].print();
		//cout<<endl;
        F2P *fy = new F2P(k, expr.tuple()[2], uparam,vparam);
		//expr.tuple()[2].print();
		//cout<<endl;
        polys[0] = (IndexedQuadArray*)(new ParametricRegion(fx, fy, umin, umax, vmin, vmax));
        delete fx;
        delete fy;
    }
    xmin = polys[0]->xmin;
    xmax = polys[0]->xmax;
    ymin = polys[0]->ymin;
    ymax = polys[0]->ymax;
    for (int i = 1; i < polynum; i++) {
        if (polys[i]->ymin < ymin)
            ymin = polys[i]->ymin;
        if (polys[i]->ymax > ymax)
            ymax = polys[i]->ymax;
        if (polys[i]->xmin < xmin)
            xmin = polys[i]->xmin;
        if (polys[i]->xmax > xmax)
            xmax = polys[i]->xmax;
    }
}

ParametricRegionPlot::~ParametricRegionPlot() {
    for (int i = 0; i < polynum; i++) {
        delete polys[i];
    }
    delete polys;
}

var ParametricRegionPlot::exportGraphics() {
    var shapelist= list(polynum,SYS(Shape));
    for (int i = 0; i < polynum; i++) {
		shapelist.tuple()[i+1] = makeIndexedPolygon(polys[i], i);
    }
    //var shape = tuple(SYS(Shape),shapelist);
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shapelist, opts);
    return graphics;
	return shapelist;
}

void ParametricRegionPlot::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
void ParametricRegionPlot::makePlotRange()
{
	double graphscale_xu=(xmax-xmin)/(umax-umin);
	double graphscale_xv=(xmax-xmin)/(vmax-vmin);
	double graphscale_yu=(ymax-ymin)/(umax-umin);
	double graphscale_yv=(ymax-ymin)/(vmax-vmin);
}
}
}