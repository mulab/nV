//#include "common.h"
#include <nV/Number.h>
#include <nV/utils.h>

namespace nV {
void RealI::print(wostream& o) const {
	mpfr_srcptr left = &(mpfi->left), right = &(mpfi->right);
	Real *a = new Real(precision()), *b = new Real(precision());
	mpfr_get_f(a->mpf, left, GMP_RNDD);
	mpfr_get_f(b->mpf, right, GMP_RNDD);
	o << _W("RealI[");
	a->print(o);
	o << _W(", ");
	b->print(o);
	o << _W(']');
	delete a, b;
}
var realI(const Object& x) {
	var r = Number::cast(x, $.RealI);
	if (r.isObject($.RealI))
		return r;
	if (r.object().type == $.String)
		return new RealI(wcs2mbs(S(x).str).c_str(), 10, 0);
	return 0;
}
var realI(const Object& x, const Object& y) {
	var left = realI(x), right = realI(y);
	if (left == 0)
		return right;
	if (right == 0)
		return left;
	RealI *r = new RealI(std::max(RI(left).precision(), RI(right).precision()));
	mpfr_set(&r->mpfi->left, &RI(left).mpfi->left, GMP_RNDD);
	mpfr_set(&r->mpfi->right, &RI(right).mpfi->right, GMP_RNDU);
	return r;
}
}

using namespace nV;

CAPI void CVALUE(System,RealI)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		if (x[1].isObject())
			r = realI(x[1].object());
	} else if (x.size == 3) {
		if (x[1].isObject() && x[2].isObject())
			r = realI(x[1].object(), x[2].object());
	}
}

namespace nV {
namespace Number {
#define M(name,f) void name(RealI& r) { f(r.mpfi); }
M(ConstPi, mpfi_const_pi)
M(ConstEuler, mpfi_const_euler)
M(ConstCatalan, mpfi_const_catalan)
#undef M
#define M(name,f) void name(RealI& r, const RealI& x) { f(r.mpfi,x.mpfi); }
M(Log, mpfi_log)
M(Log2, mpfi_log2)
M(Log10, mpfi_log10)
M(Exp, mpfi_exp)
M(Exp2, mpfi_exp2)
M(Cos, mpfi_cos)
M(Sin, mpfi_sin)
M(Tan, mpfi_tan)
M(Sec, mpfi_sec)
M(Csc, mpfi_csc)
M(Cot, mpfi_cot)
M(ArcCos, mpfi_acos)
M(ArcSin, mpfi_asin)
M(ArcTan, mpfi_atan)
M(Cosh, mpfi_cosh)
M(Sinh, mpfi_sinh)
M(Tanh, mpfi_tanh)
M(Sech, mpfi_sech)
M(Csch, mpfi_csch)
M(Coth, mpfi_coth)
M(ArcCosh, mpfi_acosh)
M(ArcSinh, mpfi_asinh)
M(ArcTanh, mpfi_atanh)
M(Sqrt, mpfi_sqrt)
#undef M
void Power(RealI& r, const RealI& x, const RealI& y) {
	mpfr_pow(&r.mpfi->left, &x.mpfi->left, &y.mpfi->left, GMP_RNDD);
	mpfr_pow(&r.mpfi->right, &x.mpfi->right, &y.mpfi->right, GMP_RNDD);
}
}
}
