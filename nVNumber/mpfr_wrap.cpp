#include "common.h"
#include "mpfr_wrap.h"

namespace nV {
namespace Number {
#define M(name,f)\
void name(Real& r)\
{\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,GMP_RNDD);\
	mpfr2Real(r, rr);\
	mpfr_delete(rr);\
}

M(ConstPi, mpfr_const_pi)
M(ConstEuler, mpfr_const_euler)
M(ConstCatalan, mpfr_const_catalan)
#undef M
#define M(name,f)\
void name(Real& r, const Real& x)\
{\
	mpfr_ptr xr = mpfr_new(x.mpf);\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,xr,GMP_RNDD);\
	mpfr2Real(r, rr);\
	mpfr_delete(rr);\
	mpfr_delete(xr);\
}

M(Log, mpfr_log)
M(Log2, mpfr_log2)
M(Log10, mpfr_log10)
M(Exp, mpfr_exp)
M(Exp2, mpfr_exp2)
M(Exp10, mpfr_exp10)
M(Cos, mpfr_cos)
M(Sin, mpfr_sin)
M(Tan, mpfr_tan)
M(Sec, mpfr_sec)
M(Csc, mpfr_csc)
M(Cot, mpfr_cot)
M(ArcCos, mpfr_acos)
M(ArcSin, mpfr_asin)
M(ArcTan, mpfr_atan)
M(Cosh, mpfr_cosh)
M(Sinh, mpfr_sinh)
M(Tanh, mpfr_tanh)
M(Sech, mpfr_sech)
M(Csch, mpfr_csch)
M(Coth, mpfr_coth)
M(ArcCosh, mpfr_acosh)
M(ArcSinh, mpfr_asinh)
M(ArcTanh, mpfr_atanh)
M(Gamma, mpfr_gamma)
M(LogGamma, mpfr_lngamma)
M(Zeta, mpfr_zeta)
M(Erf, mpfr_erf)
M(Erfc, mpfr_erfc)
M(Sqrt, mpfr_sqrt)
#undef M
#define M(name,f)\
void name(Real& r, const Real& x, const Real& y)\
{\
	mpfr_ptr xr = mpfr_new(x.mpf);\
	mpfr_ptr yr = mpfr_new(y.mpf);\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,xr,yr,GMP_RNDD);\
	mpfr2Real(r, rr);\
	mpfr_delete(rr);\
	mpfr_delete(yr);\
	mpfr_delete(xr);\
}
M(Power, mpfr_pow)
#undef M
}
}
