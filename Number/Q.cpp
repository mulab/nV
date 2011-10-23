#include "StdAfx.h"
#include <nV/Number.h>

namespace nV {
#define M(name,f)\
var operator name(const Rational& x)\
{\
	Rational* r = new Rational();\
	mpq_##f(r->mpq, x.mpq);\
	return r;\
}

M(-, neg)
#undef M
#define M(name,f)\
var operator name(const Rational& x, const Rational& y)\
{\
	Rational* r = new Rational();\
	mpq_##f(r->mpq, x.mpq, y.mpq);\
	return r;\
}

M(+, add)
M(-, sub)
M(*, mul)
M( / , div)

#undef M
var denominator(const Rational& x){
	Integer* r=new Integer();
	mpq_get_den(r->mpz,x.mpq);
	return r;
}
var denominator(const Object& x){
	if(x.type==$.Rational)
		return denominator(x.cast<Rational>());
	else
		return new Integer(1L);
}
var numerator(const Rational& x){
	Integer* r=new Integer();
	mpq_get_num(r->mpz,x.mpq);
	return r;
}
var numerator(const Object& x){
	if(x.type==$.Rational)
		return numerator(x.cast<Rational>());
	else
		return x.clone();
}
}
