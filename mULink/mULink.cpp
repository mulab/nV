#include "common.h"
#include <nV/mUObject.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#endif

namespace nV {
mU::var to_mu(sym x) {
    return mU::Sym2(x->toS().c_str());
}
mU::var to_mu(const Key& x) {
	switch (x.kind()) {
// 	case Key::Null:
// 		return mU::Ex(mU::TAG(Slot), mU::Vec(mU::Int(1L)));
//	case Key::Integer:
//		return mU::Ex(mU::TAG(Slot), mU::Vec(mU::Int(x.toUI())));
	case Key::String:
		return mU::Sym(x.toS());
	}
	return new mU::nvobj_t(&x);
}
mU::var to_mu(const Object& x) {
    if (x.type == $.Integer) {
        mU::var r = mU::Int();
        mpz_set(mU::CInt(r), x.cast<Integer>().mpz);
        return r;
    }
    if (x.type == $.Rational) {
        mU::var r = mU::Rat();
        mpq_set(mU::CRat(r), x.cast<Rational>().mpq);
        return r;
    }
    if (x.type == $.Real) {
        mU::var r = mU::Flt();
		mpf_set_prec(mU::CFlt(r), static_cast<uint>(LOG_2_10 * R(x).precision()));
        mpf_set(mU::CFlt(r), R(x).mpf);
        return r;
    }
    if (x.type == $.String) {
        mU::var r = mU::Str();
        mU::CStr(r) = x.cast<String>().str;
        return r;
    }
    if (x.type == mUObject::$mUObject)
        return x.cast<mUObject>().obj;
    return new mU::nvobj_t(&x);
}
mU::var to_mu(const Tuple& x) {
    mU::var b = mU::Vec(x.size - 1);
    for (uint i = 1; i < x.size; ++i)
        mU::At(b, i - 1) = to_mu(x[i]);
    if (x[0].isSymbol() && x[0].symbol() == $.List)
        return b;
    mU::var h = to_mu(x[0]);
    return mU::Ex(h, b);
}
mU::var to_mu(const var& x) {
    switch (x.primary()) {
    case Primary::Symbol:
        return to_mu(x.symbol());
    case Primary::Key:
        return to_mu(x.key());
    case Primary::Object:
        return to_mu(x.object());
    case Primary::Tuple:
        return to_mu(x.tuple());
    }
    return mU::Null;
}
var from_mu(mU::Var x) {
    if (x == mU::Null)
        return null;
    switch (mU::Type(x)) {
    case mU::type_int: {
        Integer* r = new Integer();
        mpz_set(r->mpz, mU::CInt(x));
        return r;
    }
    case mU::type_rat: {
        Rational* r = new Rational();
        mpq_set(r->mpq, mU::CRat(x));
        return r;
    }
    case mU::type_flt: {
        Real* r = new Real(static_cast<uint>(LOG_10_2 * mpf_get_prec(mU::CFlt(x))));
        mpf_set(r->mpf, mU::CFlt(x));
        return r;
    }
    case mU::type_str: {
        String* r = new String();
        r->str = mU::CStr(x);
        return r;
    }
    case mU::type_sym: {
        sym r = root;
        wcs s = mU::ContextName[mU::Context(x)];
        wcs p = s;
        while (*p)
            if (*p == '`') {
                r = r->symbol(wstr(wstring(s, p)));
                s = ++p;
            } else
                ++p;
        r = r->symbol(wstr(mU::Name(x)));
        return r;
    }
    case mU::type_vec: {
        uint n = CVec(x).size();
        Tuple* r = tuple(n + 1);
        r->tuple[0] = $.List;
        for (uint i = 0; i < n; ++i)
            r->tuple[i + 1] = from_mu(mU::At(x, i));
        return r;
    }
    case mU::type_ex: {
        mU::var h = mU::Head(x);
        mU::var b = mU::Body(x);
        uint n = CVec(b).size();
        Tuple* r = tuple(n + 1);
        r->tuple[0] = from_mu(h);
        for (uint i = 0; i < n; ++i)
            r->tuple[i + 1] = from_mu(mU::At(b, i));
        return r;
    }
    }
    // mU::type_obj
	if (mU::Tag(x) == mU::nvobj_t::$nVObj)
		return mU::nvobj(x);
    wostringstream oss;
    mU::Print(x, oss);
    return new String(oss.str());
}
}
