#pragma once

#include "Curve.h"
#include "F1P.h"
#include <cmath>




#define ADD_POINT_ANGLE 0.05
#define MIN_DELTA_DIS 0.001
#define MIN_X_DIS 0.001
#define MAX_DERIVATIVE 20

namespace nV{
	namespace Graphics{


		class CommonCurve : public Curve {

		public:
			double dx, graphScale;
			F1P *f;

			//about color functions
			F1P *cf;

		public:
			CommonCurve();
			CommonCurve(F1P *f, double start, double end);
			CommonCurve(F1P *f, double start, double end, F1P *cf); //with color function
			virtual ~CommonCurve();

		protected:
			int getBaseNum();
			void makePoints();
			void addVerts(LineStrip* lineStrip);
			void useColorFunction();
		};


	}
}
