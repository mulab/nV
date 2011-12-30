#pragma once

#include "CommonCurveWithPlotRange.h"

namespace nV {
	namespace Graphics{
#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MAX_DERIVATIVE 20
#define MAX_GRAPHSCALE 3
#define MIN_GRAPHSCALE 0.1
#define MIN_GRAPHSCALE_MINIMUM 0.01


	class PlotWithDomain : public CommonCurveWithPlotRange {

	public:
		PlotWithDomain(F1P *f ,std::vector<domain *> domains,F1P *cf, double ymin, double ymax);
		PlotWithDomain(F1P *f ,std::vector<domain *> domains, double ymin, double ymax);
		virtual ~PlotWithDomain();

	protected:
		//int getBaseNum(); //Override
		//void makePlotRange();
		void makePoints(std::vector<domain *>);
	};
}
}