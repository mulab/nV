#pragma once

#include "Curve.h"
#include "F1P.h"

namespace nV{
namespace Graphics {

#define ADD_POINT_ANGLE_PARAM_CURVE ADD_POINT_ANGLE_CURVE
#define MAX_DERIVATIVE 20 //before 20
#define MAX_GRAPHSCALE 3
#define MIN_GRAPHSCALE 0.1

#define	MAX_DELETE_AMOUNT 5
#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MIN_X_DIS 0.001

class ParametricCurve : public Curve {

public:
    double tmin, tmax, spt;
    double dt, graphScale;
    ArrayList<ArrayList<double> *> arrayTtotal;
    //ArrayList<double> arrayT; //存每个点的t值，与verts一一对应
    F1P *fx, *fy;

    bool tWithDomain_x;
    std::vector<domain *> tDomains_x;

    bool tWithDomain_y;
    std::vector<domain *> tDomains_y;

    //about color functions
    F1P *cf;

public:
    ParametricCurve(F1P *fx, F1P *fy, double start, double end);
    ParametricCurve(F1P *fx, F1P *fy, double start, double end, F1P *cf);
    ~ParametricCurve();

protected:
    int getBaseNum();
    void makePoints();
    void useColorFunction();
    void addVerts(LineStrip* lineStrip,  ArrayList<double> &arrayT);
    inline double getK(double t, double dt);
    void makeplotrange(F1P *f, bool havananrange, std::vector<domain *> &nanrangeset, double &ymax_f, double &ymin_f, double xmax_f, double xmin_f);

private:
    void getyrange(double &ymax, double &ymin, double &graphScale, double &spx, double &spy, 
        const std::vector<Point2d *> &vert, const std::vector<bool> &flag, 
        const std::vector<bool> &nanflag);

    void detectnanborder(F1P *f, std::vector<domain *> &nanrangeset,
        std::vector<uint> &nannodesite, const std::vector<bool> &nanflag, 
        const std::vector<bool> &derivativeflag,
        const std::vector<Point2d *> &vert);

    double getrightborder(F1P *f, double lx, double rx, uint deepth);

    double getPointOnRangeFromLeft(F1P *f_x, F1P *f_y, double x1, double x2, int depth);

    double getPointOnRangeFromRight(F1P *f_x, F1P * f_y, double x1, double x2, int depth);


};

inline double ParametricCurve::getK(double t, double dt) {
    double deritivate = fy->getDerivative(t, dt) / fx->getDerivative(t, dt);
    if(!isNaN(deritivate)) {
        return deritivate;
    } else {
        return MAX_DERIVATIVE;
    }
}

}
}
