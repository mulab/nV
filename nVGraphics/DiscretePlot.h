#pragma once

#include "GraphicsMaker2D.h"
#include "F1P.h"

namespace nV {
	namespace Graphics {
#define FILL_NULL 0
#define FILL_AXIS 2
#define FILL_CURVE 1
#define MAX_DERIVATIVE 20
#define MIN_STEP 0.00000001
#define MIN_DISTANCE 1
#define MAX_YRANGE 20
#define MAX_GRAPHSCALE 3
#define MIN_GRAPHSCALE 0.1
#define	MAX_DELETE_AMOUNT 5
#define MIN_GRAPHSCALE_MINIMUM 0.01
class DiscretePlot : public GraphicsMaker2D {

private:
	/*typedef struct 
	{
		double min;
		double max;
	}x_domain;*/
	uint curnum;
	uint maxpnum;
    uint *pnum;//the number of point in each line
	bool* singular_flag;//每条线是否为奇点函数
    int filling;
	int fillcur1,fillcur2;
	double XMin,XMax;
	double XStep;
	int colorFunctionSet;
	bool joined;
	F1P *cf;
    var varplist;
	//expression
	var expression;
    ArrayList<Color*> color;
	//std::vector<x_domain *> x_domain;
	var xparam;
	//kernel
	Kernel &k;
public:
    DiscretePlot(Kernel& k, const Tuple& cmd);
    ~DiscretePlot();
    var exportGraphics();

private:
    void makePointList();
    void makeColorOption();
	void makeyrange();
	int getNoNaNsum();
	int getNoNaN_fillCurve();
	int getDomainNumber();
protected:
    void setRule(var title, var rule);
};
	}
}