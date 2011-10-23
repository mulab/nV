#include <nV/Number.h>
#include <mpfi.h>
#ifdef _MSC_VER
#pragma comment(lib,"mpfi")
#endif

namespace {
inline mpfi_ptr mpfi_new(long prec) {
    mpfi_ptr r = new __mpfi_struct;
    mpfi_init2(r, prec);
    return r;
}
inline int mpfi_set_f(mpfi_ptr r, mpf_srcptr x, mpfr_rnd_t rnd) {
	mpfr_t temp;
	mpfr_init2(temp, mpf_get_prec(x));
	mpfr_set_f(temp, x, rnd);
	int flag = mpfi_set_fr(r, temp);
	mpfr_clear(temp);
	return flag;
}
inline void mpfi_get_f(mpf_ptr r, mpfi_srcptr x, mpfr_rnd_t rnd) {
	mpfr_t temp;
	mpfr_init2(temp, mpfi_get_prec(x));
	mpfi_get_fr(temp, x);
	mpfr_get_f(r, temp, rnd);
	mpfr_clear(temp);
}
inline mpfi_ptr mpfi_new(mpf_srcptr x) {
    mpfi_ptr r = mpfi_new(mpf_get_prec(x));
    mpfi_set_f(r, x, GMP_RNDD);
    return r;
}
inline void mpfi_delete(mpfi_ptr x) {
    mpfi_clear(x);
    delete x;
}
inline void mpfi2mpf(mpf_ptr r, mpfi_srcptr x) {
    mpfi_get_f(r, x, GMP_RNDD);
}
inline void mpfi2Real(nV::Real& r, mpfi_srcptr x) {
	mpfi2mpf(r.mpf, x);
}
}
