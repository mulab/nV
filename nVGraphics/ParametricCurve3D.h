#pragma once

#include "nV/Graphics.h"
#include "Curve3D.h"
#include "F1P.h"

namespace nV {
    namespace Graphics {

#define ADD_POINT_ANGLE_PARAM_CURVE_3D ADD_POINT_ANGLE_CURVE_3D
#define MAX_DERIVATIVE 20 //before 20
#define MAX_GRAPHSCALE 3
#define MIN_GRAPHSCALE 0.1

#define	MAX_DELETE_AMOUNT 5
#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MIN_X_DIS 0.001


class ParametricCurve3D : public Curve3D {
    typedef struct {
        double min;
        double max;
    }domain;

    typedef struct {
        std::vector<Point3d> vert;
        std::vector<Color> color;
    }LineStrip;

public:
    double tmin, tmax, spt;
    double dt;
    ArrayList<ArrayList<double> *> arrayTtotal; //存每个点的t值，与verts一一对应
    F1P *fx, *fy, *fz;

    ArrayList<LineStrip*> lineStrips;

    std::vector<domain *> Domains;

    bool tWithDomain_x;
    std::vector<domain *> tDomains_x;

    bool tWithDomain_y;
    std::vector<domain *> tDomains_y;

    bool tWithDomain_z;
    std::vector<domain *> tDomains_z;
public:
    ParametricCurve3D(F1P *fx, F1P *fy, F1P *fz, double start, double end);
    ~ParametricCurve3D();
    void creatCurve();

private:
    int getBaseNum();
    void makePoints();
    void addVerts(LineStrip* lineStrip, ArrayList<double> &arrayT);
    Vector3D* getVector(double t, double dt);

    void makeplotrange(F1P *f, bool havananrange, std::vector<domain *> &nanrangeset, double &ymax_f, double &ymin_f, double xmax_f, double xmin_f);

    void getyrange(double &ymax, double &ymin, double &graphScale, double &spx, double &spy, 
        const std::vector<Point2d *> &vert, const std::vector<bool> &flag, 
        const std::vector<bool> &nanflag);

    void detectnanborder(F1P *f, std::vector<domain *> &nanrangeset,
        std::vector<uint> &nannodesite, const std::vector<bool> &nanflag, 
        const std::vector<bool> &derivativeflag,
        const std::vector<Point2d *> &vert);

    double getrightborder(F1P *f, double lx, double rx, uint deepth);

    double getPointOnRangeFromLeft(F1P *f_x, F1P *f_y, F1P *f_z, double x1, double x2, int depth);

    double getPointOnRangeFromRight(F1P *f_x, F1P *f_y, F1P *f_z, double x1, double x2, int depth);

    void mergedomain(bool tWithDomain_first_f, std::vector<domain *> tDomain_first_f,
        bool tWithDomain_second_f, std::vector<domain *> tDomain_second_f,
        std::vector<domain *> &Domains_f);
};

    }
}
