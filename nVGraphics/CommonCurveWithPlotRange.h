#pragma once

#include "CommonCurve.h"

namespace nV {
	namespace Graphics{
#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MAX_DERIVATIVE 20
#define MAX_GRAPHSCALE 3
#define MIN_GRAPHSCALE 0.1

		class CommonCurveWithPlotRange : public CommonCurve {

		public:
			CommonCurveWithPlotRange() {};
			CommonCurveWithPlotRange(F1P *f, double xmin, double xmax); //auto plot range
			CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax);
			CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, F1P *cf);
			CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax, F1P *cf);
            CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax, std::vector<singularitynode *> singularityset);
            CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax, std::vector<singularitynode *> singularityset, F1P *cf);
			virtual ~CommonCurveWithPlotRange();

		protected:
			int getBaseNum(); //Override
			virtual void makePlotRange();
			void makePoints();
			double getPointOnRangeFromLeft(double x1, double x2, int depth);
			double getPointOnRangeFromRight(double x1, double x2, int depth);
		};
	}
}
