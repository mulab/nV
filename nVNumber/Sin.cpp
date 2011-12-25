#include "common.h"
#include <nV/Number.h>

using namespace nV;

#undef M
#define M(f)\
CAPI void VALUE(f)(Kernel& k, var& r, Tuple& x) {\
	if (x.size != 2)\
		return;\
	if (x[1].isObject($.Real)) {\
		r = new Real(x[1].cast<Real>().precision());\
		Number::f(r.cast<Real>(), x[1].cast<Real>());\
	} else if (x[1].isObject($.RealI)) {\
		r = new RealI(x[1].cast<RealI>().precision());\
		Number::f(r.cast<RealI>(), x[1].cast<RealI>());\
	}\
}

M(Log)
M(Log2)
M(Log10)
M(Exp)
M(Exp2)
//M(Exp10)
M(Cos)
M(Sin)
M(Tan)
M(Sec)
M(Csc)
M(Cot)
M(ArcCos)
M(ArcSin)
M(ArcTan)
M(Cosh)
M(Sinh)
M(Tanh)
M(Sech)
M(Csch)
M(Coth)
M(ArcCosh)
M(ArcSinh)
M(ArcTanh)
//M(Gamma)
//M(LogGamma)
//M(Zeta)
//M(Erf)
//M(Erfc)
M(Sqrt)


/*
using namespace nV;

CAPI void VALUE(Pi)(Kernel& k, var& r, Tuple& x) {
	r = Pi();
}
CAPI void VALUE(EulerGamma)(Kernel& k, var& r, Tuple& x) {
	r = Euler();
}
CAPI void VALUE(Catalan)(Kernel& k, var& r, Tuple& x) {
	r = Catalan();
}
CAPI void VALUE(E)(Kernel& k, var& r, Tuple& x) {
	r = new Real(1L);
	r = Exp(r.cast<Real>());
}
*/

#undef M
#define M(f)\
CAPI void VALUE(f)(Kernel& k, var& r, Tuple& x) {\
	if (x.size != 2)\
		return;\
	if (x[1].isObject($.Real)) {\
		r = new Real(x[1].cast<Real>().precision());\
		Number::f(r.cast<Real>(), x[1].cast<Real>());\
	}\
}
M(Exp10)
M(Gamma)
M(LogGamma)
M(Zeta)
M(Erf)
M(Erfc)
