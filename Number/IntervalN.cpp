//#include "StdAfx.h"
#include <nV/Number.h>
//#include <limits>

namespace nV {
var IntervalN(Kernel& k, const var& x, uint n) {
    switch (x.primary()) {
    case Primary::Object: {
	RealI* ri;
	if (x.isObject($.Real)) {
		ri = new RealI(x.cast<Real>().precision());
		mpfi_set_f(ri->mpfi, x.cast<Real>().mpf, GMP_RNDD);
		return ri;
	}
	if (x.isObject($.Integer)) {
		ri = new RealI(n);
		mpfi_set_z(ri->mpfi, x.cast<Integer>().mpz);
		return ri;
	}
	if (x.isObject($.Rational)) {
		ri = new RealI(n);
		mpfi_set_q(ri->mpfi, x.cast<Rational>().mpq);
		return ri;
	}
	return x;
    }
    case Primary::Symbol:
		if (x == SYS(Pi)) {
			RealI* r = new RealI(n);
			Number::ConstPi(*r);
			return r;
		} else if (x == SYS(E)) {
			RealI* r = new RealI(1L, n);
			Number::Exp(*r, *r);
			return r;
		} else if (x == SYS(Euler)) {
			RealI* r = new RealI(n);
			Number::ConstEuler(*r);
			return r;
		} else if (x == SYS(Catalan)) {
			RealI* r = new RealI(n);
			Number::ConstCatalan(*r);
			return r;
		}
        return x;
    case Primary::Tuple: {
        var r;
        Tuple* t = x.tuple().clone();										//t: the expression
//        t->tuple[0] = IntervalN(k, t->tuple[0], n);
        if (t->tuple[0].isSymbol()) {
            std::unordered_map<sym, Kernel::Attribute>::const_iterator
            iter = k.attributes.find(t->tuple[0].symbol());
            if (iter != k.attributes.end()) {
                if (!iter->second.count($.NHoldAll)) {
                    if (t->size > 1 && !iter->second.count($.NHoldFirst))
                        t->tuple[1] = IntervalN(k, t->tuple[1], n);
                    if (t->size > 2 && !iter->second.count($.NHoldRest)) {
                        for (uint i = 2; i < t->size; ++i)
                            t->tuple[i] = IntervalN(k, t->tuple[i], n);
                    }
                }
                r = t;
            }
        }
        if (r == 0) {
            for (uint i = 1; i < t->size; ++i)
                t->tuple[i] = IntervalN(k, t->tuple[i], n);
            r = t;
        };
        return k.eval(r);
    }
    }
    return x;
}
}

using namespace nV;

CAPI void CVALUE(System, IntervalN)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = IntervalN(k, x[1]);
	if (x.size == 3)
		if (isNumber(x[2])) {
			uint n = static_cast<uint>(toD(x[2].object()));
			r = IntervalN(k, x[1], n);
		}
}
CAPI void CVALUE(System, NI)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
		var t = IntervalN(k, x[1]);
		if (t.isObject($.RealI)) {
			r = new Real(t.cast<RealI>().precision());
			mpfi2Real(r.cast<Real>(), t.cast<RealI>().mpfi);
		} else
			r = t;
	} else if (x.size == 3 && isNumber(x[2])) {
        uint n = static_cast<uint>(toD(x[2].object()));
		var t = IntervalN(k, x[1], n);
		if (t.isObject($.RealI)) {
			r = new Real(std::min(n, t.cast<RealI>().precision()));
			mpfi2Real(r.cast<Real>(), t.cast<RealI>().mpfi);
		} else
			r = t;
	}
}