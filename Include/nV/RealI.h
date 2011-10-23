#pragma once
#include <mpfr.h>
#include <mpfi.h>
#ifdef _MSC_VER
#pragma comment(lib, "mpfr")
#pragma comment(lib, "mpfi")
#endif

namespace nV {
#define LOG_2_10 3.3219280948873623478703194294894
#define LOG_10_2 0.3010299956639811952137388947245
inline size_t mpfi_hash(mpfi_srcptr x) {
	return (size_t)x;
}
inline int mpfi_cmp2(mpfi_srcptr x, mpfi_srcptr y) {
	mpfr_t xm, ym;
	mpfr_init2(xm, mpfi_get_prec(x));
	mpfr_init2(ym, mpfi_get_prec(y));
	mpfi_get_fr(xm, x);
	mpfi_get_fr(ym, y);
	int r = mpfr_cmp(xm, ym);
	mpfr_clear(xm);
	mpfr_clear(ym);
	return r;
}
#define mpfi_get_default_prec mpfr_get_default_prec
class RealI : public Object {
public:
    mpfi_t mpfi;
    virtual ~RealI() {
        mpfi_clear(mpfi);
    }
    virtual RealI* clone() const {
        RealI* r = new RealI(precision());
        mpfi_set(r->mpfi, mpfi);
        return r;
    }
	virtual int compare(const Object& x) const {
        return mpfi_cmp2(mpfi, x.cast<RealI>().mpfi);
    }
    virtual size_t hash() const {
        return mpfi_hash(mpfi);
    }
    API virtual void print(wostream&) const;
    RealI(uint n = 0) : Object(SYS(RealI)) {
        mpfi_init2(mpfi, std::max(static_cast<sint>(LOG_2_10 * n + 0.5), mpfi_get_default_prec()));
    }
    RealI(uint x, uint n) : Object($.RealI) {
        mpfi_init2(mpfi, std::max(static_cast<sint>(LOG_2_10 * n + 0.5), mpfi_get_default_prec()));
        mpfi_set_ui(mpfi, x);
    }
    RealI(sint x, uint n) : Object($.RealI) {
        mpfi_init2(mpfi, std::max(static_cast<sint>(LOG_2_10 * n + 0.5), mpfi_get_default_prec()));
        mpfi_set_si(mpfi, x);
    }
    RealI(double x, uint n) : Object($.RealI) {
        mpfi_init2(mpfi, std::max(static_cast<sint>(LOG_2_10 * n + 0.5), mpfi_get_default_prec()));
        mpfi_set_d(mpfi, x);
    }
    RealI(const char* x, int y, uint n) : Object($.RealI) {
        mpfi_init2(mpfi, std::max(static_cast<sint>(LOG_2_10 * n + 0.5), mpfi_get_default_prec()));
        mpfi_set_str(mpfi, x, y);
    }
    uint precision() const {
        return static_cast<uint>(LOG_10_2 * mpfi_get_prec(mpfi) + 0.5);
    }
    double toD() const {
		return mpfi_get_d(mpfi);
	}
};
inline RealI& RI(const var& x) {
	return x.cast<RealI>();
}
inline RealI& RI(const Object& x) {
	return x.cast<RealI>();
}
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
