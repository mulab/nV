#include <nV/Number.h>
#include "Plot3D.h"

namespace nV {
namespace Graphics {

Plot3D::Plot3D(Kernel &k, const Tuple &cmd) : k(k) {
    spacetype = FLAT_SPACE;
    var expr = cmd[1];
    var xrange = cmd[2];
    var yrange = cmd[3];
    var xparam = xrange.tuple()[1];
    var yparam = yrange.tuple()[1];
    xmin = toD(N(k, k.eval(xrange.tuple()[2])).object());
    xmax = toD(N(k, k.eval(xrange.tuple()[3])).object());
    ymin = toD(N(k, k.eval(yrange.tuple()[2])).object());
    ymax = toD(N(k, k.eval(yrange.tuple()[3])).object());

    if (cmd[cmd.size - 1].isTuple(SYS(Optional))) {
        var opts = cmd[cmd.size - 1];
        readOptions(opts.tuple());
    }

    sur = new Surface*[1];
    surnum = 1;
    F2P *f3d = new F2P(k, expr, xparam, yparam);
    sur[0] = new Surface(f3d, xmin, xmax, ymin, ymax);
    delete f3d;
    zmin = sur[0]->zmin;
    zmax = sur[0]->zmax;
    for (int i = 1; i < surnum; i++) {
        if (sur[i]->zmin < zmin)
            zmin = sur[i]->zmin;
        if (sur[i]->zmax > zmax)
            zmax = sur[i]->zmax;
    }
}

Plot3D::~Plot3D() {
    for (int i = 0; i < surnum; i++) {
        delete sur[i];
    }
    delete[] sur;
}

//make a meshed surface
var Plot3D::exportGraphics() {
    //vert list
    var v = list(sur[0]->vert.size());
    for(uint i = 0; i < sur[0]->vert.size(); i++) {
        v.tuple()[i + 1] = list(new Real(sur[0]->vert[i]->x, 0), new Real(sur[0]->vert[i]->y, 0), new Real(sur[0]->vert[i]->z, 0));
    }
    uint total = 0;
    for(uint i = 0; i < sur[0]->sur.size(); i++) {
        if (sur[0]->sur[i]->isshow) {
            total++;
        }
    }
    var quad = list(total * 4);
    uint count = 1;
    for(uint i = 0; i < sur[0]->sur.size(); i++) {
        if (sur[0]->sur[i]->isshow) {
        quad.tuple()[count++] = new Integer((long)sur[0]->sur[i]->p11);
        quad.tuple()[count++] = new Integer((long)sur[0]->sur[i]->p12);
        quad.tuple()[count++] = new Integer((long)sur[0]->sur[i]->p22);
        quad.tuple()[count++] = new Integer((long)sur[0]->sur[i]->p21);
        }
    }
    var qoptions = list(list(0,SYS(DoubleSurface)), SYS(Options));
    var equad = list(quad,qoptions,SYS(QuadArray));
    
    //var linepak = list(sur[0]->ny + sur[0]->nx + 1, SYS(Line));
    //for(uint i = 0; i < sur[0]->ny; i++) {
    //    var lv = list(sur[0]->xline[i]->p.size());
    //    for(uint j = 0; j < sur[0]->xline[i]->p.size(); j++) {
    //        lv.tuple()[j + 1] = new Integer((long)sur[0]->xline[i]->p[j]);
    //    }
    //    linepak.tuple()[i + 1] = lv;
    //}
    //for(uint i = 0; i < sur[0]->nx; i++) {
    //    var lv = list(sur[0]->yline[i]->p.size());
    //    for(uint j = 0; j < sur[0]->yline[i]->p.size(); j++) {
    //        lv.tuple()[j + 1] = new Integer((long)sur[0]->yline[i]->p[j]);
    //    }
    //    linepak.tuple()[sur[0]->ny + i + 1] = lv;
    //}
    var linepak = list(sur[0]->meshx.size() + sur[0]->meshy.size() + 1, SYS(Line));
    for (uint i = 0; i < sur[0]->meshx.size(); ++i) {
        var lv = list(sur[0]->meshx[i].size());
        for (uint j = 0; j < sur[0]->meshx[i].size(); ++j) {
            lv.tuple()[j + 1] = new Integer((long)sur[0]->meshx[i][j]);
        }
        linepak.tuple()[i + 1] = lv;
    }
    for (uint i = 0; i < sur[0]->meshy.size(); ++i) {
        var lv = list(sur[0]->meshy[i].size());
        for (uint j = 0; j < sur[0]->meshy[i].size(); ++j) {
            lv.tuple()[j + 1] = new Integer((long)sur[0]->meshy[i][j]);
        }
        linepak.tuple()[sur[0]->meshx.size() + i + 1] = lv;
    }
    var color = list(new Real(0), new Real(0), new Real(0), SYS(Color3f));
    var loptions = list(list(0,SYS(Mesh)), color, SYS(Options));
    linepak.tuple()[sur[0]->meshx.size() + sur[0]->meshy.size() + 1] = loptions;
    
    var directive = list(equad, linepak, SYS(Directive));
    var complex = list(v, directive, SYS(GraphicsComplex));
    var shape = list(complex, SYS(Shape));
    var options = makeGlobalOptions();
    var graphics = list(shape, options, SYS(Graphics3D));

    return graphics;
}

void Plot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}

}
}

