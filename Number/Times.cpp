#include "StdAfx.h"
#include <nV/Number.h>

namespace nV {
namespace Number {
var Times(const Object& x, const Object& y) {
    var r;
    if (x.type == $.Integer) {
        if (y.type == $.Integer) {
            r = new Integer();
            mpz_mul(r.cast<Integer>().mpz, x.cast<Integer>().mpz,
                    y.cast<Integer>().mpz);
            return r;
        }
        if (y.type == $.Rational) {
            r = new Rational();
            mpq_set_z(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
            mpq_mul(r.cast<Rational>().mpq, r.cast<Rational>().mpq, y.cast<Rational>().mpq);
			mpq_canonicalize(Q(r).mpq);
            return r;
        }
        if (y.type == $.Real) {
            r = new Real(y.cast<Real>().precision());
            mpf_set_z(r.cast<Real>().mpf, x.cast<Integer>().mpz);
            mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
            return r;
        }
		if (y.type == $.RealI) {
			r = new RealI(y.cast<RealI>().precision());
			mpfi_set_z(r.cast<RealI>().mpfi, x.cast<Integer>().mpz);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, y.cast<RealI>().mpfi);
			return r;
		}
    }
    if (x.type == $.Rational) {
        if (y.type == $.Integer) {
            r = new Rational();
            mpq_set_z(r.cast<Rational>().mpq, y.cast<Integer>().mpz);
            mpq_mul(r.cast<Rational>().mpq, r.cast<Rational>().mpq, x.cast<Rational>().mpq);
			mpq_canonicalize(Q(r).mpq);
            return r;
        }
        if (y.type == $.Rational) {
            r = new Rational();
            mpq_mul(r.cast<Rational>().mpq, x.cast<Rational>().mpq,
                    y.cast<Rational>().mpq);
			mpq_canonicalize(Q(r).mpq);
            return r;
        }
        if (y.type == $.Real) {
            r = new Real(y.cast<Real>().precision());
            mpf_set_q(r.cast<Real>().mpf, x.cast<Rational>().mpq);
            mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
            return r;
        }
		if (y.type == $.RealI) {
			r = new RealI(y.cast<RealI>().precision());
			mpfi_set_q(r.cast<RealI>().mpfi, x.cast<Rational>().mpq);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, y.cast<RealI>().mpfi);
			return r;
		}
    }
    if (x.type == $.Real) {
        if (y.type == $.Integer) {
            r = new Real(x.cast<Real>().precision());
            mpf_set_z(r.cast<Real>().mpf, y.cast<Integer>().mpz);
            mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, x.cast<Real>().mpf);
            return r;
        }
        if (y.type == $.Rational) {
            r = new Real(x.cast<Real>().precision());;
            mpf_set_q(r.cast<Real>().mpf, y.cast<Rational>().mpq);
            mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, x.cast<Real>().mpf);
            return r;
        }
        if (y.type == $.Real) {
            r = new Real(std::min(x.cast<Real>().precision(), y.cast<Real>().precision()));
            mpf_mul(r.cast<Real>().mpf, x.cast<Real>().mpf, y.cast<Real>().mpf);
            return r;
        }
		if (y.type == $.RealI) {
			r = new RealI(y.cast<RealI>().precision());
			mpfi_set_f(r.cast<RealI>().mpfi, x.cast<Real>().mpf, GMP_RNDD);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, y.cast<RealI>().mpfi);
			return r;
		}
    }
	if (x.type == $.RealI) {
		if (y.type == $.Integer) {
			r = new RealI(x.cast<RealI>().precision());
			mpfi_set_z(r.cast<RealI>().mpfi, y.cast<Integer>().mpz);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, x.cast<RealI>().mpfi);
			return r;
		}
		if (y.type == $.Rational) {
			r = new RealI(x.cast<RealI>().precision());;
			mpfi_set_q(r.cast<RealI>().mpfi, y.cast<Rational>().mpq);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, x.cast<RealI>().mpfi);
			return r;
		}
		if (y.type == $.Real) {
			r = new RealI(x.cast<RealI>().precision());;
			mpfi_set_f(r.cast<RealI>().mpfi, y.cast<Real>().mpf, GMP_RNDD);
			mpfi_mul(r.cast<RealI>().mpfi, r.cast<RealI>().mpfi, x.cast<RealI>().mpfi);
			return r;
		}
		if (y.type == $.RealI) {
			r = new RealI(std::min(x.cast<RealI>().precision(), y.cast<RealI>().precision()));
			mpfi_mul(r.cast<RealI>().mpfi, x.cast<RealI>().mpfi, y.cast<RealI>().mpfi);
			return r;
		}
	}
    if (x.compare(y) == 0)
        r = tuple($.Power, &x, new Integer(2L));
    else
        r = tuple($.Times, &x, &y);
	return r;
}
}
}

using namespace nV;

CAPI void VALUE(Times)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1 || !isNumber(x[1]))
		return;
	r = x[1].clone();
    for (uint i = 2; i < x.size; ++i)
        if (isNumber(x[i]))
            r = Number::Times(r.object(), x[i].object());
}
CAPI void CMETHOD(System_Integer, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpz_mul(self.cast<Integer>().mpz, self.cast<Integer>().mpz, x[1].cast<Integer>().mpz);
    r = self;
}
CAPI void CMETHOD(System_Rational, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpq_mul(self.cast<Rational>().mpq, self.cast<Rational>().mpq, x[1].cast<Rational>().mpq);
    r = self;
}
CAPI void CMETHOD(System_Real, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpf_mul(self.cast<Real>().mpf, self.cast<Real>().mpf, x[1].cast<Real>().mpf);
    r = self;
}