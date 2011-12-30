#include "common.h"
#include <nV/Number.h>

namespace nV {
namespace Number {
var Power(const Object& x, const Object& y) {
	var r;
	if (x == 0.0) {
		r = new Integer(0L);
		return r;
	}
	if (x == 1.0) {
		r = &x;
		return r;
	}
	if (y == 0.0) {
		r = new Integer(1L);
		return r;
	}
	if (y == 1.0) {
		r = &x;
		return r;
	}
	if (y.type == $.Integer) {
		sint se = Z(y).toSI();
		uint e = se > 0 ? se : -se;
		if (x.type == $.Integer) {
			if (se > 0) {
				r = new Integer;
				mpz_pow_ui(Z(r).mpz, Z(x).mpz, e);
			} else {
				r = new Rational;
				mpz_set_ui(mpq_numref(Q(r).mpq), 1L);
				mpz_pow_ui(mpq_denref(Q(r).mpq), Z(x).mpz, e);
				mpq_canonicalize(Q(r).mpq);
			}
			return r;
		} else if (x.type == $.Rational) {
			r = new Rational;
			if(se > 0) {
				mpz_pow_ui(mpq_numref(Q(r).mpq), mpq_numref(Q(x).mpq), e);
				mpz_pow_ui(mpq_denref(Q(r).mpq), mpq_denref(Q(x).mpq), e);
			}
			else {
				mpz_pow_ui(mpq_numref(Q(r).mpq), mpq_denref(Q(x).mpq), e);
				mpz_pow_ui(mpq_denref(Q(r).mpq), mpq_numref(Q(x).mpq), e);
				mpq_canonicalize(Q(r).mpq);
			}
			return r;
		} else if (x.type == $.Real) {
			r = new Real(R(x).precision());
			mpf_pow_ui(R(r).mpf, R(x).mpf, e);
			if(se < 0)
				mpf_ui_div(R(r).mpf, 1L, R(r).mpf);
			return r;
		} else if (x.type == $.RealI) {
			r = new RealI(RI(x).precision());
			mpfr_pow_ui(&RI(r).mpfi->left, &RI(x).mpfi->left, e, GMP_RNDD);
			mpfr_pow_ui(&RI(r).mpfi->right, &RI(x).mpfi->right, e, GMP_RNDD);
			if(se < 0)
				mpfi_ui_div(RI(r).mpfi, 1L, RI(r).mpfi);
			return r;
		}
	} else if (y.type == $.Rational) {
		if (x.type == $.Integer) {
			sint se = mpz_get_si(mpq_numref(Q(y).mpq));
			uint e = se > 0 ? se : -se;
			r = new Integer;
			mpz_pow_ui(Z(r).mpz, Z(x).mpz, e);
			r = list(r, new Integer((se > 0 ? 1 : -1) * mpz_get_si(mpq_denref(Q(y).mpq))), $.Radical);
			return r;
		} else if (x.type == $.Rational) {
			var a = new Integer, b = new Integer;
			mpz_set(Z(a).mpz, mpq_numref(Q(x).mpq));
			mpz_set(Z(b).mpz, mpq_denref(Q(x).mpq));
			a = Power(a.object(), y);
			b = Power(b.object(), y);
			r = list(a, list(b, new Integer(-1L), $.Power), $.Times);
			return r;
		} else if (x.type == $.Real) {
			r = new Real(R(x).precision());
			mpf_set_q(R(r).mpf, Q(y).mpq);
			Power(R(r), R(x), R(r));
			return r;
		} else if (x.type == $.RealI) {
			r = new RealI(RI(x).precision());
			mpfi_set_q(RI(r).mpfi, Q(y).mpq);
			Power(RI(r), RI(x), RI(r));
			return r;
		}
	} else if (y.type == $.Real) {
		if (x.type == $.Integer) {
			r = new Real(R(y).precision());
			mpf_set_z(R(r).mpf, Z(x).mpz);
			Power(R(r), R(r), R(y));
			return r;
		} else if (x.type == $.Rational) {
			r = new Real(R(y).precision());
			mpf_set_q(R(r).mpf, Q(x).mpq);
			Power(R(r), R(r), R(y));
			return r;
		} else if (x.type == $.Real) {
			r = new Real(std::min(R(x).precision(), R(y).precision()));
			Power(R(r), R(x), R(y));
			return r;
		} else if (x.type == $.RealI) {
			r = new RealI(R(y).precision());
			mpfi_set_f(RI(r).mpfi, R(y).mpf, GMP_RNDD);
			Power(RI(r), RI(x), RI(r));
			return r;
		}
	} else if (y.type == $.RealI) {
		if (x.type == $.Integer) {
			r = new RealI(RI(y).precision());
			mpfi_set_z(RI(r).mpfi, Z(x).mpz);
			Power(RI(r), RI(r), RI(y));
			return r;
		} else if (x.type == $.Rational) {
			r = new RealI(RI(y).precision());
			mpfi_set_q(RI(r).mpfi, Q(x).mpq);
			Power(RI(r), RI(r), RI(y));
			return r;
		} else if (x.type == $.Real) {
			r = new RealI(R(x).precision());
			mpfi_set_f(RI(r).mpfi, R(x).mpf, GMP_RNDD);
			Power(RI(r), RI(r), RI(y));
			return r;
		} else if (x.type == $.RealI) {
			r = new RealI(std::min(RI(x).precision(), RI(y).precision()));
			Power(RI(r), RI(x), RI(y));
			return r;
		}
	}
	r = list(&x, &y, $.Power);
	return r;
}
}
}


using namespace nV;

CAPI void VALUE(Power)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3)
        return;
    if (isNumber(x[1]) && isNumber(x[2]))
		r = Number::Power(x[1].object(), x[2].object());
}
CAPI void CMETHOD(System_Integer, pow, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpz_pow_ui(Z(self).mpz, Z(self).mpz, Z(x[1]).toUI());
    r = self;
}
CAPI void CMETHOD(System_Rational, pow, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpz_pow_ui(mpq_numref(Q(self).mpq), mpq_numref(Q(self).mpq), Z(x[1]).toUI());
	mpz_pow_ui(mpq_denref(Q(self).mpq), mpq_denref(Q(self).mpq), Z(x[1]).toUI());
    r = self;
}
CAPI void CMETHOD(System_Real, pow, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpf_pow_ui(R(self).mpf, R(self).mpf, Z(x[1]).toUI());
    r = self;
}
