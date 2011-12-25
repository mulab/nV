#include "StdAfx.h"
/** \file
\brief 基础数论函数.
*/
#include <nV/Number.h>

namespace nV {
namespace Number {
/** \brief 数论模块

包含各种数论函数.
*/

gmp_randstate_t rstate; /**< 随机状态变量. */

/** \brief 随机数初始化.
	\note 需在调用::RandomInteger之前执行.
*/
void Randomize() {
    gmp_randinit_default(rstate);
    gmp_randseed_ui(rstate, static_cast<uint>(time(NULL)));
}

/** \brief 使用\f$n\f$作为随机数种子重置随机数生成器.
	\param n 整数.
*/
void SeedRandom(const Integer& x) {
    gmp_randseed(rstate, x.mpz);
}

/** \brief 使用系统时间作为随机数种子重置随机数生成器.
*/
void SeedRandom() {
    gmp_randseed_ui(rstate, static_cast<uint>(time(NULL)));
}

/** \brief 产生随机整数.
	\param n 正整数.
	\return 0到n之间一致分布的随机数.
	\param {a,b} 正整数对.
	\return a到b之间一致分布的随机数.
	\note 需先执行::Randomize.
	\todo 判断界.
*/
var RandomInteger(const Integer& x) {
    Integer* r = new Integer();
    mpz_urandomm(r->mpz, rstate, x.mpz);
    return r;
}
var RandomInteger(uint x) {
    mpz_t $x;
    mpz_init_set_ui($x, x);
    Integer* r = new Integer();
    mpz_urandomm(r->mpz, rstate, $x);
    mpz_clear($x);
    return r;
}
var RandomInteger(const Integer& x, const Integer& y) {
    mpz_t diff;
    mpz_init(diff);
    mpz_sub(diff, y.mpz, x.mpz);
    Integer* r = new Integer();
    mpz_urandomm(r->mpz, rstate, diff);
    mpz_add(r->mpz, r->mpz, x.mpz);
    return r;
}

/** \brief 产生随机素数.
\param n 大于1的整数.
\return 0到n之间一致分布的随机素数.
\param {a,b} 包含素数的正整数对.
\return a到b之间一致分布的随机素数.
\note 需先执行::Randomize.
\todo 判断界.
*/
var RandomPrime(const Integer& x) {
    var r = RandomInteger(x);
    Integer& m = Z(r);
    if (EvenQ(m) && mpz_cmp_ui(m.mpz, 2))
        mpz_sub_ui(m.mpz, m.mpz, 1);
    while (mpz_cmp(m.mpz, x.mpz) <= 0 ) {
        if (PrimeQ(m))
            return r;
        else
            mpz_add_ui(m.mpz, m.mpz, 2);
    }
    return $.Fail;
}
var RandomPrime(const Integer& x, const Integer& y) {
    var r = RandomInteger(x, y);
    Integer& m = Z(r);
    if (EvenQ(m) && mpz_cmp_ui(m.mpz, 2))
        mpz_sub_ui(m.mpz, m.mpz, 1);
    while (mpz_cmp(x.mpz, m.mpz) <= 0 && mpz_cmp(y.mpz, m.mpz) >= 0) {
        if (PrimeQ(m))
            return r;
        else
            mpz_add_ui(m.mpz, m.mpz, 2);
    }
    return $.Fail;
}
}
}

using namespace nV;

CAPI void CVALUE(System, RandomInteger)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2 && x[1].isObject($.Integer)) {
        r = Number::RandomInteger(Z(x[1]));
        return;
    }
    if (x.size == 3 && x[1].isObject($.Integer) && x[2].isObject($.Integer)) {
        r = Number::RandomInteger(Z(x[1]), Z(x[2]));
        return;
    }
}
CAPI void CVALUE(System, RandomPrime)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2 && x[1].isObject($.Integer)) {
        r = Number::RandomPrime(Z(x[1]));
        return;
    }
    if (x.size == 3 && x[1].isObject($.Integer) && x[2].isObject($.Integer)) {
        r = Number::RandomPrime(Z(x[1]), Z(x[2]));
        return;
    }
}