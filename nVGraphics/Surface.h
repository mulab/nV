#pragma once

#include "F2P.h"
#include "GraphicsMaker.h"

namespace nV {
namespace Graphics {
//#define ADD_POINT_ANGLE 0.3
#define ADD_POINT_ANGLE 0.1
#define MAX_SPLIT_DEPTH 2
#define LARGE 1000000
#define MAX_D 20
#define MIN_DIS_3D 0.05
#define MAX_SPLIT 3
#define MIN_GRAPHSCALE_MINIMUM 0.1
#define	MAX_DELETE_AMOUNT 5

struct IndexPolygonForSurface {
    int p11, p12, p21, p22;
    int xlineid;
    int ylineid;
    bool x0line;
    bool y0line;
    bool isshow;
    uint times;
    IndexPolygonForSurface(int a, int b, int c, int d, int xid, int yid, bool isx0line, bool isy0line, bool isshow = true, uint times = 0)
            : p11(a), p12(b), p21(c), p22(d), xlineid(xid), ylineid(yid), x0line(isx0line), y0line(isy0line),isshow(isshow), times(times) {}
};

class IndexStripLine {
public:
    ArrayList<int> p;
    ArrayList<Point3d*>* vert;
public:
    IndexStripLine(ArrayList<Point3d*>* vert) {
        this->vert = vert;
    }
};

class IndexStripLineX : public IndexStripLine {
public:
    IndexStripLineX(ArrayList<Point3d*>* vert) : IndexStripLine(vert) {}
    void add(int index) {
        unsigned int i;
        for (i = 0; i < p.size() && ((*vert)[index]->x > (*vert)[p[i]]->x); i++);
        p.add(i, index);
    }
};

class IndexStripLineY : public IndexStripLine {
public:
    IndexStripLineY(ArrayList<Point3d*>* vert) : IndexStripLine(vert) {}
    void add(int index) {
        unsigned int i;
        for (i = 0; i < p.size() && ((*vert)[index]->y > (*vert)[p[i]]->y); i++);
        p.add(i, index);
    }
};

class Surface {
    typedef struct vertderivate{
        double vert_d_x;
        double vert_d_y;
        vertderivate() : vert_d_x(0.0), vert_d_y(0.0) {}
        vertderivate(const double vert_d_x, const  double vert_d_y) : vert_d_x(vert_d_x), vert_d_y(vert_d_y) {}
    } vert_d;

public:
    F2P* f;
    int nx, ny;
    double spx, spy, xmin, xmax, ymin, ymax, spz, zmin, zmax;
    double graphScalex, graphScaley;
    double dx, dy;
    ArrayList<Point3d*> vert;
    ArrayList<IndexPolygonForSurface*> sur;
    IndexStripLineX **xline;
    IndexStripLineY **yline;

    std::vector< std::vector<int> > meshx;
    std::vector< std::vector<int> > meshy;
    std::vector<vert_d> vert_derivate;

public:
    Surface(F2P* f3d, double xstart, double xend, double ystart, double yend);
    ~Surface();

private:
    void initsurface();
    int getNx();
    int getNy();
    void makeplotrange(F2P *f3d_f, double &zmin_f, double &zmax_f, double dx_f, double dy_f,const ArrayList<Point3d*> &vert_f, const std::vector<vert_d> &vert_d_f);
    void trySplitPolygonX(unsigned int id, int depth);
    void trySplitPolygonY(unsigned int id, int depth);

    bool judgmentDX(unsigned int id, double netk1, double netk2, double netk3, double netk4);
    bool getborderx(uint id, double &border, double y, double netk1, double netk2, double x1, double x2, int depth);
    bool judgmentDY(uint id, double netk1, double netk2, double netk3, double netk4);
    bool getbordery(uint id, double &border, double x, double netk1, double netk2, double y1, double y2, int depth);

    void adjustmesh();
};

}
}