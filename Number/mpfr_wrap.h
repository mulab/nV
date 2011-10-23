#include <nV/Number.h>

namespace nV {
inline mpfr_ptr mpfr_new(long prec) {
    mpfr_ptr r = new __mpfr_struct;
    mpfr_init2(r, prec);
    return r;
}
inline mpfr_ptr mpfr_new(mpf_srcptr x) {
    mpfr_ptr r = mpfr_new(mpf_get_prec(x));
    mpfr_set_f(r, x, GMP_RNDD);
    return r;
}
inline void mpfr_delete(mpfr_ptr x) {
    mpfr_clear(x);
    delete x;
}
inline void mpfr2mpf(mpf_ptr r, mpfr_srcptr x) {
    mpfr_get_f(r, x, GMP_RNDD);
}
inline void mpfr2Real(nV::Real& r, mpfr_srcptr x) {
// 	if (mpfr_nan_p(x)) {
// 		r.digit = mpfr_sgn(x) > 0 ? nV::Real::NaN : nV::Real::NNaN;
// 	} else if (mpfr_inf_p(x)) {
// 		r.digit = mpfr_sgn(x) > 0 ? nV::Real::Inf : nV::Real::NInf;
// 	} else
// 		r.digit = nV::Real::Normal;
	mpfr2mpf(r.mpf, x);
}
}
