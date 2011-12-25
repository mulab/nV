#pragma once

#include "CommonCurveWithPlotRange.h"
#include "F1P.h"

namespace nV {
	namespace Graphics{
#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MAX_DERIVATIVE 20
#define	MAX_DELETE_AMOUNT 5

		class sigularityWithPlotRange : public CommonCurveWithPlotRange {

		public:
			sigularityWithPlotRange() {};
			sigularityWithPlotRange(F1P *f, double xmin, double xmax); //auto plot range
			sigularityWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax);
			sigularityWithPlotRange(F1P *f, double xmin, double xmax, F1P *cf);
			sigularityWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax, F1P *cf);
			virtual ~sigularityWithPlotRange();

		protected:
			void makePlotRange();
		};
	}
}