#include "common.h"
#include <nV/Number.h>
#include <limits>

namespace nV {
var N(Kernel& k, const var& x, uint n) {
    switch (x.primary()) {
    case Primary::Object: {
        Real* r = new Real(n);
        if (x.object().type == $.Integer) {
            mpf_set_z(r->mpf, x.cast<Integer>().mpz);
            return r;
        }
        if (x.object().type == $.Rational) {
            mpf_set_q(r->mpf, x.cast<Rational>().mpq);
            return r;
        }
        if (x.object().type == $.Real && x.cast<Real>().precision() > n) {
            mpf_set(r->mpf, x.cast<Real>().mpf);
            return r;
        }
        delete r;
        return x;
    }
    case Primary::Symbol:
        if (x == SYS(Pi)) {
			return Number::Pi(n);
		} else if (x == SYS(E)) {
			Real* r = new Real(1L, n);
			Number::Exp(*r, *r);
			return r;
		}  else if (x == SYS(Euler)) {
			Real* r = new Real(n);
			Number::ConstEuler(*r);
			return r;
		} else if (x == SYS(Catalan)) {
			Real* r = new Real(n);
			Number::ConstCatalan(*r);
			return r;
		}
        return x;
    case Primary::Tuple: {
        var r;
        Tuple* t = x.tuple().clone();
        t->tuple[0] = N(k, t->tuple[0], n);
        if (t->tuple[0].isSymbol()) {
            boost::unordered_map<sym, Kernel::Attribute>::const_iterator
            iter = k.attributes.find(t->tuple[0].symbol());
            if (iter != k.attributes.end()) {
                if (!iter->second.count($.NHoldAll)) {
                    if (t->size > 1 && !iter->second.count($.NHoldFirst))
                        t->tuple[1] = N(k, t->tuple[1], n);
                    if (t->size > 2 && !iter->second.count($.NHoldRest)) {
                        for (uint i = 2; i < t->size; ++i)
                            t->tuple[i] = N(k, t->tuple[i], n);
                    }
                }
                r = t;
            }
        }
        if (r == 0) {
            for (uint i = 1; i < t->size; ++i)
                t->tuple[i] = N(k, t->tuple[i], n);
            r = t;
        }
        return k.eval(r);
    }
    }
    return x;
}
double toD(Kernel& k, const var& x) {
	switch (x.primary()) {
	case Primary::Symbol:
		if (x == $.Infinity)
			return std::numeric_limits<double>().infinity();
		if (x.isTuple($.Times) && x.tuple().size == 3 && x.tuple()[2] == $.Infinity)
			return -std::numeric_limits<double>().infinity();
		if (x == $.NaN)
			return std::numeric_limits<double>().quiet_NaN();
		break;
	case Primary::Object:
		return toD(x.object());
	}
	k.logging(_W("Not a double number!"));
	return 0.0;
}
}

using namespace nV;

CAPI void CVALUE(System, N)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
        r = N(k, x[1]);
    else if (x.size == 3)
        if (isNumber(x[2])) {
            uint n = static_cast<uint>(toD(x[2].object()));
            r = N(k, x[1], n);
        }
}
