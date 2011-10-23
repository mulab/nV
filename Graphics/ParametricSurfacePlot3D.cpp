#include <nV/Number.h>
#include "ParametricSurfacePlot3D.h"

namespace nV {
namespace Graphics {

ParametricSurfacePlot3D::ParametricSurfacePlot3D(Kernel& k, const Tuple& cmd) : k(k) {
    var expr = cmd[1];
    var urange = cmd[2];
    var vrange = cmd[3];
    if (cmd[cmd.size - 1].isTuple(SYS(Optional))) {
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
        polys = new IndexedQuadArray3D*[polynum];
        for (int i = 0; i < polynum; i++) {
			F2P *fx = new F2P(k, expr.tuple()[i + 1].tuple()[1], uparam,vparam);
            F2P *fy = new F2P(k, expr.tuple()[i + 1].tuple()[2], uparam,vparam);
			F2P *fz = new F2P(k, expr.tuple()[i + 1].tuple()[3], uparam,vparam);
            polys[i] = (IndexedQuadArray3D*)(new ParametricSurface(fx, fy, fz, umin, umax, vmin, vmax));
            delete fx;
            delete fy;
			delete fz;
        }
    } else {
        polynum = 1;
        polys = new IndexedQuadArray3D*[polynum];
        var funs = expr;
        F2P *fx = new F2P(k, expr.tuple()[1], uparam,vparam);
        F2P *fy = new F2P(k, expr.tuple()[2], uparam,vparam);
		F2P *fz = new F2P(k, expr.tuple()[3], uparam,vparam);
        polys[0] = (IndexedQuadArray3D*)(new ParametricSurface(fx, fy, fz, umin, umax, vmin, vmax));
        delete fx;
        delete fy;
		delete fz;
    }
   
    xmin = polys[0]->xmin;
    xmax = polys[0]->xmax;
    ymin = polys[0]->ymin;
    ymax = polys[0]->ymax;
    zmin = polys[0]->zmin;
    zmax = polys[0]->zmax;
    for (int i = 1; i < polynum; i++) {
        if (polys[i]->xmin < xmin)
            xmin = polys[i]->xmin;
        if (polys[i]->xmax > xmax)
            xmax = polys[i]->xmax;
        if (polys[i]->ymin < ymin)
            ymin = polys[i]->ymin;
        if (polys[i]->ymax > ymax)
            ymax = polys[i]->ymax;
        if (polys[i]->zmin < zmin)
            zmin = polys[i]->zmin;
        if (polys[i]->zmax > zmax)
            zmax = polys[i]->zmax;
    }
}

ParametricSurfacePlot3D::~ParametricSurfacePlot3D() {
    for (int i = 0; i < polynum; i++) {
        delete polys[i];
    }
    delete polys;
}

var ParametricSurfacePlot3D::exportGraphics() {
    var shapelist = list(polynum,SYS(Shape));
    for (int k = 0; k < polynum; k++) {
        IndexedQuadArray3D* poly = polys[k];
        var v = list(poly->verts.size());
        for (size_t i = 0; i < poly->verts.size(); ++i) {
			v.tuple()[i+1] = list(new Real(poly->verts[i]->x,0), new Real(poly->verts[i]->y,0), 
				             new Real(poly->verts[i]->z,0));
        }
        //quad list
        var quads = list(poly->quads.size() * 4);
        for (size_t i = 0; i < poly->quads.size(); i++) {
			quads.tuple()[4*i+1] = new Integer((long)poly->quads[i]->p11);
            quads.tuple()[4*i + 2] = new Integer((long)poly->quads[i]->p12);
			quads.tuple()[4*i + 3] = new Integer((long)poly->quads[i]->p22);
			quads.tuple()[4*i + 4] = new Integer((long)poly->quads[i]->p21);
        }
        var surfflag = tuple(SYS(Surface));
        var qopt = tuple(SYS(Options),surfflag);
        var equad = tuple(SYS(QuadArray),quads,qopt);
        //mesh
        var lines = list(poly->meshs.size()+1,SYS(Line));
        for (size_t i = 0; i < poly->meshs.size(); i++) {
            var line = list(poly->meshs[i]->p.size());
            for (size_t j = 0; j < poly->meshs[i]->p.size(); j++)
				line.tuple()[j+1] = new Integer((long)poly->meshs[i]->p[j]);
			lines.tuple()[i+1] = line;
        }
        var color = list(3,SYS(Color3f));// Vec(Flt(0.0), Flt(0.0), Flt(0.0)));
		color.tuple()[1]=new Real(0.0,0);
		color.tuple()[2]=new Real(0.0,0);
		color.tuple()[3]=new Real(0.0,0);
		var meshflag = tuple(SYS(Mesh));
        var lopts = list(2,SYS(Options));
		lopts.tuple()[1]=meshflag;
		lopts.tuple()[2]=color;
		lines.tuple()[lines.tuple().size-1]=lopts;
        
        //graphics complex
        var directive =tuple(SYS(Directive),equad,lines);
        var complex = tuple(SYS(GraphicsComplex),v,directive);
		shapelist.tuple()[k+1]=complex;
	}
    
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shapelist, opts);
    return graphics;
}

void ParametricSurfacePlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
}
}

