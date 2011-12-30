#include "common.h"
#include <nV/Number.h>

namespace nV {
namespace Number {
var Divide(const Object& x) {
	var r;
    if (x.type == $.Integer) {
        sint sgn = mpz_sgn(x.cast<Integer>().mpz);
        if (sgn == 0) {
            r = $.Infinity;
            return r;
        }
        var a = new Integer(sgn), b = &x;
        if (sgn < 0) {
            b = new Integer();
            mpz_neg(b.cast<Integer>().mpz, x.cast<Integer>().mpz);
        }
        r = new Rational();
        mpq_set_num(r.cast<Rational>().mpq, a.cast<Integer>().mpz);
        mpq_set_den(r.cast<Rational>().mpq, b.cast<Integer>().mpz);
        return r;
    }
    if (x.type == $.Rational) {
        r = new Rational();
        mpq_inv(r.cast<Rational>().mpq, x.cast<Rational>().mpq);
        return r;
    }
    if (x.type == $.Real) {
        if (!mpf_sgn(x.cast<Real>().mpf)) {
            r = $.Infinity;
            return r;
        }
        r = new Real(x.cast<Real>().precision());
        mpf_ui_div(r.cast<Real>().mpf, 1L, x.cast<Real>().mpf);
        return r;
    }
    r = tuple($.Power, &x, new Integer(-1L));
	return r;
}
var Divide(const Object& x, const Object& y) {
    var r;
    if (y.type == $.Integer) {
        if (!mpz_sgn(y.cast<Integer>().mpz)) {
            r = $.Infinity;
            return r;
        }
        if (x.type == $.Integer) {
            r = new Rational();
            mpq_set_num(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
            mpq_set_den(r.cast<Rational>().mpq, y.cast<Integer>().mpz);
            mpq_canonicalize(r.cast<Rational>().mpq);
            Integer* t = new Integer();
            mpq_get_den(t->mpz, r.cast<Rational>().mpq);
            if (!mpz_cmp_ui(t->mpz, 1)) {
                mpq_get_num(t->mpz, r.cast<Rational>().mpq);
                r = t;
            }
            return r;
        }
        if (x.type == $.Rational) {
            r = new Rational();
            mpq_set_z(Q(r).mpq, Z(y).mpz);
            mpq_div(Q(r).mpq, Q(x).mpq, Q(r).mpq);
            return r;
        }
        if (x.type == $.Real) {
            r = new Real(R(x).precision());
            mpf_set_z(R(r).mpf, Z(y).mpz);
            mpf_div(R(r).mpf, R(x).mpf, R(r).mpf);
            return r;
        }
    }
    if (y.type == $.Rational) {
        if (x.type == $.Integer) {
            r = new Rational();
            mpq_set_z(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
            mpq_div(r.cast<Rational>().mpq, r.cast<Rational>().mpq, y.cast<Rational>().mpq);
            return r;
        }
        if (x.type == $.Rational) {
            r = new Rational();
            mpq_div(r.cast<Rational>().mpq, x.cast<Rational>().mpq,
                    y.cast<Rational>().mpq);
            return r;
        }
        if (x.type == $.Real) {
            r = new Real(x.cast<Real>().precision());
            mpf_set_q(r.cast<Real>().mpf, y.cast<Rational>().mpq);
            mpf_div(r.cast<Real>().mpf, x.cast<Real>().mpf, r.cast<Real>().mpf);
            return r;
        }
    }
    if (y.type == $.Real) {
        if (!mpf_sgn(y.cast<Real>().mpf)) {
            r = $.Infinity;
            return r;
        }
        if (x.type == $.Integer) {
            r = new Real(y.cast<Real>().precision());
            mpf_set_z(r.cast<Real>().mpf, x.cast<Integer>().mpz);
            mpf_div(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
            return r;
        }
        if (x.type == $.Rational) {
            r = new Real(y.cast<Real>().precision());
            mpf_set_q(r.cast<Real>().mpf, x.cast<Rational>().mpq);
            mpf_div(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
            return r;
        }
        if (x.type == $.Real) {
            r = new Real(std::min(x.cast<Real>().precision(),
                                      y.cast<Real>().precision()));
            mpf_div(r.cast<Real>().mpf, x.cast<Real>().mpf,
                    y.cast<Real>().mpf);
            return r;
        }
    }
    if (x.compare(y) == 0)
        r = new Integer(1L);
    else {
        r = Divide(y);
        r = tuple($.Times, &x, r);
    }
	return r;
}
}
}

using namespace nV;

CAPI void VALUE(Divide)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2)
        if (isNumber(x[1])) {
            r = Number::Divide(x[1].object());
            return;
        }
    if (x.size == 3)
        if (isNumber(x[1]) && isNumber(x[2])) {
            r = Number::Divide(x[1].object(), x[2].object());
            return;
        }
}
CAPI void CMETHOD(System_Integer, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpz_div(self.cast<Integer>().mpz, self.cast<Integer>().mpz, x[1].cast<Integer>().mpz);
    r = self;
}
CAPI void CMETHOD(System_Rational, div, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpq_inv(self.cast<Rational>().mpq, self.cast<Rational>().mpq);
    r = self;
}
CAPI void CMETHOD(System_Rational, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpq_div(self.cast<Rational>().mpq, self.cast<Rational>().mpq, x[1].cast<Rational>().mpq);
    r = self;
}
CAPI void CMETHOD(System_Real, div, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpf_ui_div(self.cast<Real>().mpf, 1, self.cast<Real>().mpf);
    r = self;
}
CAPI void CMETHOD(System_Real, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    mpf_div(self.cast<Real>().mpf, self.cast<Real>().mpf, x[1].cast<Real>().mpf);
    r = self;
}
