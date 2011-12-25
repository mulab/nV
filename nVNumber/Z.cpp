#include "StdAfx.h"
#include <nV/Number.h>

namespace nV {
#define M(name,f)\
var operator name(const Integer& x)\
{\
	Integer* r = new Integer();\
	mpz_##f(r->mpz, x.mpz);\
	return r;\
}

M(-, neg)
M(~, com)
#undef M
#define M(name,f)\
var operator name(const Integer& x, const Integer& y)\
{\
	Integer* r = new Integer();\
	mpz_##f(r->mpz, x.mpz, y.mpz);\
	return r;\
}

M(+, add)
M(-, sub)
M(*, mul)
M( / , div)
M( % , mod)
M(&, and)
M( | , ior)
M( ^ , xor)

#undef M
#define M(name,f)\
var operator name(const Integer& x, uint y)\
{\
	Integer* r = new Integer();\
	mpz_##f##_ui(r->mpz, x.mpz, y);\
	return r;\
}

M(+, add)
M(-, sub)
M(*, mul)
M( % , mod)

#undef M
#define M(name,f)\
var operator name(const Integer& x, sint y)\
{\
	Integer* r = new Integer();\
	mpz_##f##_si(r->mpz, x.mpz, y);\
	return r;\
}

M(*, mul)

#undef M
#define M(name,f)\
var operator name(uint x, const Integer& y)\
{\
	Integer* r = new Integer();\
	mpz_ui_##f(r->mpz, x, y.mpz);\
	return r;\
}

M(-, sub)

#undef M

var operator<<(const Integer& x, int y) {
    Integer* r = new Integer();
    if (y < 0)
        mpz_tdiv_q_2exp(r->mpz, x.mpz, -y);
    else
        mpz_mul_2exp(r->mpz, x.mpz, y);
    return r;
}
var operator>>(const Integer& x, int y) {
    Integer* r = new Integer();
    if (y < 0)
        mpz_mul_2exp(r->mpz, x.mpz, -y);
    else
        mpz_tdiv_q_2exp(r->mpz, x.mpz, y);
    return r;
}
}

using namespace nV;

/** \brief 位与运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位与运算.
*/
CAPI void CVALUE(System, BitAnd)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isObject($.Integer))
        return;
    r = x[1];
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isObject($.Integer))
            r = Z(r) & Z(x[i]);
}

/** \brief 位或运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位或运算.
*/
CAPI void CVALUE(System, BitOr)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isObject($.Integer))
        return;
    r = x[1];
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isObject($.Integer))
            r = Z(r) | Z(x[i]);
}

/** \brief 位异或运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位异或运算.
*/
CAPI void CVALUE(System, BitXor)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isObject($.Integer))
        return;
    r = x[1];
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isObject($.Integer))
            r = Z(r) ^ Z(x[i]);
}

/** \brief 位非运算.
	\param n 整数.
	\return \f$n\f$的位非运算.
*/
CAPI void CVALUE(System, BitNot)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    // !? ~Z(x[1]) - 1?
    r = ~Z(x[1]);
}

/** \brief 将\f$n\f$左移\f$k\f$位.
	\param n, k 整数. 默认\f$k=1\f$.
	\return 将\f$n\f$左移\f$k\f$位. 当\f$k\f$为负数时为右移.
*/
CAPI void CVALUE(System, BitShiftLeft)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isObject($.Integer) || !x[2].isObject($.Integer))
        return;
    r = Z(x[1]) << Z(x[2]).toSI();
}

/** \brief 将\f$n\f$右移\f$k\f$位.
	\param n, k 整数. 默认\f$k=1\f$.
	\return 将\f$n\f$右移\f$k\f$位. 当\f$k\f$为负数时为左移.
*/
CAPI void CVALUE(System, BitShiftRight)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isObject($.Integer) || !x[2].isObject($.Integer))
        return;
    r = Z(x[1]) >> Z(x[2]).toSI();
}