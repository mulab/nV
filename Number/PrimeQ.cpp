#include "StdAfx.h"
/** \file
\brief 素性检测.
*/
#include <nV/Number.h>

namespace nV {
namespace Number {

/** \brief 判断\f$n\f$是否为素数.
	\param n 整数.
	\retval >0 是.
	\retval 0 否.
	\note 使用Rabin-Miller强伪素数检测.
*/
bool PrimeQ(const Integer& x) {
    return mpz_probab_prime_p(x.mpz, 10) > 0;
}

/** \brief 求大于\f$n\f$的第一个素数.
	\param n 整数.
	\return 大于\f$n\f$的第一个素数.
	\todo 非整数判断.
*/
var NextPrime(const Integer& x) {
    /*if (mpz_cmp_ui(x.mpz, 1) <= 0 && mpz_cmp_si(x.mpz, -2) >= 0)
    	return new Integer(2L);
    if (mpz_cmp_ui(x.mpz, -3) == 0)
    	return new Integer(-3L);
    Integer* r = new Integer();
    if (EvenQ(x))
    	mpz_add_ui(r->mpz, x.mpz, 1);
    while (1) {
    	if (PrimeQ(*r))
    		return r;
    	else
    		mpz_add_ui(r->mpz, r->mpz, 1);
    }
    */
    Integer* r = new Integer();
    mpz_nextprime(r->mpz, x.mpz);
    return r;
}

/** \brief 求整数\f$n\f$在\f$b\f$进制表示下的长度.
	\param n 整数.
	\param b 2 - 62.
	\return 整数在\f$b\f$进制表示下的长度.
*/
uint IntegerLength(const Integer& x, uint y) {
    return mpz_sizeinbase(x.mpz, y);
}

/** \brief 设置\f$n\f$的\f$2^k\f$位为1.
	\param n 整数.
	\param k 非负整数.
	\return 设置\f$n\f$的\f$2^k\f$位为1.
*/
var BitSet(const Integer& x, uint y) {
    Integer* r = new Integer(x);
    mpz_setbit(r->mpz, y);
    return r;
}

/** \brief 设置\f$n\f$的\f$2^k\f$位为0.
\param n 整数.
\param k 非负整数.
\return 设置\f$n\f$的\f$2^k\f$位为0.
*/
var BitClear(const Integer& x, uint y) {
    Integer* r = new Integer(x);
    mpz_clrbit(r->mpz, y);
    return r;
}

/** \brief 求\f$n\f$的\f$2^k\f$位.
\param n, k 正整数.
\return \f$n\f$的\f$2^k\f$位.
*/
bool BitGet(const Integer& x, uint y) {
    return mpz_tstbit(x.mpz, y) != 0;
}

/** \brief 判断一个整数是否为奇数.
	\param n 整数.
	\retval 1 是.
	\retval 0 否.
*/
bool OddQ(const Integer& x) {
    return mpz_odd_p(x.mpz) != 0;
}

/** \brief 判断一个整数是否为偶数.
	\param n 整数.
	\retval 1 是.
	\retval 0 否.
*/
bool EvenQ(const Integer& x) {
    return mpz_even_p(x.mpz) != 0;
}

/** \brief 求整数的绝对值.
	\param n 整数
	\return n的绝对值.
*/
var Abs(const Integer& x) {
    Integer* r = new Integer();
    mpz_abs(r->mpz, x.mpz);
    return r;
}

/** \brief 整数开平方.
	\param n 整数.
	\return 平方根的整数部分\f$\lfloor\sqrt{n}\rfloor\f$.
	\note 使用Karatsuba平方根算法.
	\todo 补充文档.
*/
var Sqrt(const Integer& x) {
    Integer* r = new Integer();
    mpz_sqrt(r->mpz, x.mpz);
    return r;
}

/** \brief 判断一个整数是否为完全平方数.
	\param n 输入整数.
	\retval 1 是.
	\retval 0 否.
	\note 使用\f$\mathbb{Z}/k\mathbb{Z}\f$上平方数方法.
	以256，45，17，13，7为模, 比率99.25%, 使用汇编写的mpn_mod_34lsub1加速.
*/
bool SquareQ(const Integer& x) {
    return mpz_perfect_square_p(x.mpz) != 0;
}
}
}

using namespace nV;

CAPI void CVALUE(System, PrimeQ)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    r = boolean(Number::PrimeQ(x[1].cast<Integer>()));
}
CAPI void CVALUE(System, OddQ)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    r = boolean(Number::OddQ(x[1].cast<Integer>()));
}
CAPI void CVALUE(System, EvenQ)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    r = boolean(Number::EvenQ(x[1].cast<Integer>()));
}
CAPI void CVALUE(System, SquareQ)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    r = boolean(Number::SquareQ(x[1].cast<Integer>()));
}
CAPI void CVALUE(System, NextPrime)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isObject($.Integer))
        return;
    r = Number::NextPrime(x[1].cast<Integer>());
}
CAPI void CVALUE(System, IntegerLength)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isObject($.Integer) || !x[2].isObject($.Integer))
        return;
    r = new Integer(Number::IntegerLength(x[1].cast<Integer>(), mpz_get_ui(x[2].cast<Integer>().mpz)));
}