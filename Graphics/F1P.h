#pragma once

#include "nV/Graphics.h"

#define LARGE 1000000

namespace nV{
	namespace Graphics{
//extern uint times;
//extern double timetotal;

		class F1P {

			Kernel& kernel;
			var expre, param;

		public:

			F1P(Kernel&, var ep, var pa);

			void getData(double* x, double* y, int num);

			void getArrayData(double *x, double *y, int num);

			double getSingleData(double input);

			double getDerivative(double x, double dx);

			double getSecondDerivative(double x, double dx);
		};

	}
}