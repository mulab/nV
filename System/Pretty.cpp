#include <nV/System.h>
#include <nV/utils.h>
#include <algorithm>

namespace nV {
var Pretty(Kernel& k, const Integer& x) {
	if (mpz_sgn(x.mpz) < 0) {
		Integer* r = new Integer;
		mpz_neg(r->mpz, x.mpz);
		return nV::tuple($.Minus, r);
	}
	return &x;
}
var Pretty(Kernel& k, const Rational& x) {
	mpq_canonicalize(const_cast<mpq_ptr>(x.mpq));
	Integer *a = new Integer, *b = new Integer;
	mpq_get_num(a->mpz, x.mpq);
	mpq_get_den(b->mpz, x.mpq);
	if(mpz_cmp_ui(b->mpz, 1) == 0)
		return Pretty(k, Z(a));
	if(mpz_sgn(a->mpz) < 0)
	{
		mpz_neg(a->mpz, a->mpz);
		return nV::tuple($.Minus, nV::tuple(SYS(Divide), a, b));
	}
	return nV::tuple(SYS(Divide), a, b);
}
var Pretty(Kernel& k, const Real& x) {
	if (mpf_sgn(x.mpf) < 0) {
		Real* r = new Real(x.precision());
		mpf_neg(r->mpf, x.mpf);
		return nV::tuple($.Minus, r);
	}
	return &x;
}
var Pretty(Kernel& k, const Object& x) {
	if (x.type == $.Integer)
		return Pretty(k, Z(x));
	else if (x.type == $.Rational)
		return Pretty(k, Q(x));
	else if (x.type == $.Real)
		return Pretty(k, R(x));
	sym h = SYM(x.type,Pretty);
	var r = tuple(h, &x);
	r = k.eval(r);
	if (!r.isTuple(h))
		return r;
	return &x;
}
var Pretty(Kernel& k, const Key& x) {
	return &x;
}
var Pretty(Kernel& k, const Tuple& x) {
	var h = x[0], r;
	if (h.isSymbol()) {
		h = SYM(h.symbol(),Pretty);
		r = tuple(h, &x);
		r = k.eval(r);
		if (!r.isTuple(h))
			return r;
	}
	r = x.clone();
	for (uint i = 0; i < x.size; ++i)
		r.tuple()[i] = Pretty(k, x[i]);
	h = r.tuple()[0];
	if (!h.isSymbol())
		return r;
	if (r.tuple().size == 3 && h == $.Power) {
		if (r.tuple()[2].isTuple($.Minus) && r.tuple()[2].tuple().size == 2) {
			if (r.tuple()[2].tuple()[1].isObject() && r.tuple()[2].tuple()[1].object() == 1.0)
				return tuple(SYS(Divide), new Integer(1L), r.tuple()[1]);
			return tuple(SYS(Divide), new Integer(1L), Pretty(k, tuple($.Power, r.tuple()[1], r.tuple()[2].tuple()[1])));
		}
		else if (r.tuple()[2].isTuple(SYS(Divide)) && r.tuple()[2].tuple().size == 3) {
			if (r.tuple()[2].tuple()[1].isObject() && r.tuple()[2].tuple()[1].object() == 1.0 && r.tuple()[2].tuple()[2].isObject($.Integer)) {
				var e = r.tuple()[2].tuple()[2];
				return e.object() == 2
					? tuple($.Sqrt, r.tuple()[1])
					: tuple($.Radical, r.tuple()[1], e);
			}
		}
	}
	else if (h == $.Times) {
		std::vector<var> t[2];
		int sgn = 1;
		for (size_t i = 1; i < r.tuple().size; ++i) {
			var c = r.tuple()[i];
			if (c.isTuple()) {
				h = c.tuple()[0];
				if (h == $.Minus) {
					sgn = -sgn;
					c = c.tuple()[1];
					if (c.isTuple(SYS(Divide))) {
						var a = c.tuple()[1];
						if (a.isTuple($.Minus)) {
							sgn = -sgn;
							t[0].push_back(a.tuple()[1]);
						}
						else
							t[0].push_back(a);
						var b = c.tuple()[2];
						if (b.isTuple($.Minus)) {
							sgn = -sgn;
							t[1].push_back(b.tuple()[1]);
						}
						else
							t[1].push_back(b);
					}
					else
						t[0].push_back(c);
				}
				else if (h == SYS(Divide)) {
					var a = c.tuple()[1];
					if (a.isTuple($.Minus)) {
						sgn = -sgn;
						t[0].push_back(a.tuple()[1]);
					}
					else
						t[0].push_back(a);
					var b = c.tuple()[2];
					if (b.isTuple($.Minus)) {
						sgn = -sgn;
						t[1].push_back(b.tuple()[1]);
					}
					else
						t[1].push_back(b);
				}
				else
					t[0].push_back(c);
			}
			else
				t[0].push_back(c);
		}
		var s[2];
		std::sort(t[0].begin(), t[0].end(), lessPrimary);
		s[0] = nV::list(t[0].size(), t[0].begin(), $.Times);
		s[0] = k.value(s[0].tuple());
		//if (s[0].isTuple($.Times))
		//	std::sort(s[0].tuple().tuple + 1, s[0].tuple().tuple + s[0].tuple().size);
		std::sort(t[1].begin(), t[1].end(), lessPrimary);
		s[1] = nV::list(t[1].size(), t[1].begin(), $.Times);
		s[1] = k.value(s[1].tuple());
		if (s[1].isObject() && s[1].object() == 1.0)
			r = s[0];
		else {
			//if (s[1].isTuple($.Times))
			//	std::sort(s[1].tuple().tuple + 1, s[1].tuple().tuple + s[1].tuple().size);
			r = tuple(SYS(Divide), s[0], s[1]);
		}
		if (sgn == -1)
			r = tuple(SYS(Minus), r);
	}
	return r;
}
var Pretty(Kernel& k, sym x) {
	return x;
}
var Pretty(Kernel& k, const var& x) {
	switch (x.primary()) {
	case Primary::Null:
		return $.Null;
	case Primary::Symbol:
		return Pretty(k, x.symbol());
	case Primary::Object:
		return Pretty(k, x.object());
	case Primary::Tuple:
		return Pretty(k, x.tuple());
	case Primary::Key:
		return Pretty(k, x.key());
	}
	return x;
}
}

using namespace nV;

CAPI void VALUE(Pretty)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = Pretty(k, x[1]);
		return;
	}
}
