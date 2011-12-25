#include "common.h"
#include <nV/Number.h>
#include <gmpxx.h>

namespace nV {
namespace Number {
var cast(const Object& x, sym type) {
    if (x.type == $.Integer) {
        if (type == $.Rational) {
            Rational* r = new Rational();
            mpq_set_z(r->mpq, x.cast<Integer>().mpz);
            return r;
        }
        if (type == $.Real) {
            Real* r = new Real();
            mpf_set_z(r->mpf, x.cast<Integer>().mpz);
            return r;
        }
		if (type == $.RealI) {
			RealI* r = new RealI();
			mpfi_set_z(r->mpfi, x.cast<Integer>().mpz);
			return r;
		}
    }
    if (x.type == $.Rational) {
        if (type == $.Integer) {
            Integer* r = new Integer();
            mpz_set_q(r->mpz, x.cast<Rational>().mpq);
            return r;
        }
        if (type == $.Real) {
            Real* r = new Real();
            mpf_set_q(r->mpf, x.cast<Rational>().mpq);
            return r;
        }
		if (type == $.RealI) {
			RealI* r = new RealI();
			mpfi_set_q(r->mpfi, x.cast<Rational>().mpq);
			return r;
		}
    }
    if (x.type == $.Real) {
        if (type == $.Integer) {
            Integer* r = new Integer();
            mpz_set_f(r->mpz, x.cast<Real>().mpf);
            return r;
        }
        if (type == $.Rational) {
            Rational* r = new Rational();
            mpq_set_f(r->mpq, x.cast<Real>().mpf);
            return r;
        }
		if (type == $.RealI) {
			RealI* r = new RealI(x.cast<Real>().precision());
			mpfi_set_f(r->mpfi, x.cast<Real>().mpf, GMP_RNDD);
			return r;
		}
    }
	if (x.type == $.RealI) {
		if (type == $.Integer) {
			return cast(cast(x, $.Real).object(), type);
		}
		if (type == $.Rational) {
			return cast(cast(x, $.Real).object(), type);
		}
		if (type == $.Real) {
			Real* r = new Real(x.cast<RealI>().precision());
			mpfi_get_f(r->mpf, x.cast<RealI>().mpfi, GMP_RNDD);
			return r;
		}
	}
    return &x;
}
// std::pair<var,var> lift(const Object& x, const Object& y); ?
int cmp(const Object& x, const Object& y) {
    if (x.type == $.Integer) {
        if (y.type == $.Integer)
            return mpz_cmp(Z(x).mpz, Z(y).mpz);
        if (y.type == $.Rational) {
            mpq_class t;
            mpq_set_z(t.get_mpq_t(), Z(x).mpz);
            return mpq_cmp(t.get_mpq_t(), Q(y).mpq);
        }
        if (y.type == $.Real) {
            mpf_class t;
            mpf_set_z(t.get_mpf_t(), Z(x).mpz);
            return mpf_cmp(t.get_mpf_t(), R(y).mpf);
        }
		if (y.type == $.RealI) {
			return cmp(x, cast(y, $.Real).object());
		}
    }
    if (x.type == $.Rational) {
        if (y.type == $.Integer)
            return -cmp(y, x);
        if (y.type == $.Rational)
            return mpq_cmp(Q(x).mpq, Q(y).mpq);
        if (y.type == $.Real) {
            mpf_class t;
            mpf_set_q(t.get_mpf_t(), Q(x).mpq);
            return mpf_cmp(t.get_mpf_t(), R(y).mpf);
        }
		if (y.type == $.RealI) {
			return cmp(x, cast(y, $.Real).object());
		}
    }
    if (x.type == $.Real) {
        if (y.type == $.Integer)
            return -cmp(y, x);
        if (y.type == $.Rational)
            return -cmp(y, x);
        if (y.type == $.Real)
            return mpf_cmp(R(x).mpf, R(y).mpf);
		if (y.type == $.RealI) {
			return cmp(x, cast(y, $.Real).object());
		}
    }
	if (x.type == $.RealI) {
		if (y.type == $.Integer)
			return -cmp(y, x);
		if (y.type == $.Rational)
			return -cmp(y, x);
		if (y.type == $.Real)
			return -cmp(y, x);
		if (y.type == $.RealI) {
			return mpfi_cmp2(x.cast<RealI>().mpfi, y.cast<RealI>().mpfi);
		}
	}
    return x.compare(y);
}
}
}

using namespace nV;

CAPI void VALUE(Cast)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 3 && x[1].isObject() && x[2].isSymbol())
        r = Number::cast(x[1].object(), x[2].symbol());
}
