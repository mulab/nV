#include "common.h"
/** \file
\brief 最大公因子及互素检测.
*/
#include <nV/Number.h>

namespace nV {
namespace Number {

/** \brief 判断两个整数是否互素.
\param n1,n2 整数.
\retval 1 是.
\retval 0 否.
*/
bool CoprimeQ(const Integer& x, const Integer& y) {
    return mpz_cmp_ui(GCD(x, y).cast<Integer>().mpz, 1) == 0;
}

/** \brief 判断\f$m\f$是否能整除\f$n\f$.
	\param n 整数.
	\param m 非零整数.
	\retval 1 是.
	\retval 0 否.
	\todo 判断m非零.
*/
bool Divisible(const Integer& x, const Integer& y) {
    if (mpz_fits_uint_p(y.mpz)) {
        return mpz_divisible_ui_p(x.mpz, mpz_get_ui(y.mpz)) != 0;
    } else {
        return mpz_divisible_p(x.mpz, y.mpz) != 0;
    }
}
bool Divisible(const Integer& x, uint y) {
    return mpz_divisible_ui_p(x.mpz, y) != 0;
}

var DivideExact(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_divexact_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz));
    } else {
        mpz_divexact(r->mpz, x.mpz, y.mpz);
    }
    return r;
}
var DivideExact(const Integer& x, uint y) {
    Integer* r = new Integer();
    mpz_divexact_ui(r->mpz, x.mpz, y);
    return r;
}

/** \brief 求\f$m\f$除以\f$n\f$的商.
	\param m 整数.
	\param n 非零整数.
	\return 商\f$\lfloor m/n\rfloor\f$.
	\todo 增加非零判断, 增加非整数处理.
	\todo 需要文档补充.
*/
var Quotient(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_div_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz));
    } else {
        mpz_div(r->mpz, x.mpz, y.mpz);
    }
    return r;
}

/** \brief 求\f$m\f$除以\f$n\f$的商(已知\f$n\f$整除\f$m\f$).
	\param m 整数.
	\param n 非零整数.
	\return \f$m/n\f$.
*/
var ExactQuotient(const Integer& x, uint y) {
    Integer* r = new Integer();
    mpz_divexact_ui(r->mpz, x.mpz, y);
    return r;
}

/** \brief 求\f$m\f$除以\f$n\f$的余数.
	\param m 整数.
	\param n 非零整数.
	\return 余数\f$m \bmod n\f$.
	\note
	\todo 增加非零判断, 增加非整数处理.
	\todo 需要文档补充.
*/
var Mod(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_mod_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz));
    } else {
        mpz_mod(r->mpz, x.mpz, y.mpz);
    }
    return r;
}
	//|r|<=|y|/2
var Mod2(const Integer& x, const Integer& y) {
	var r=Mod(x,y);
	mpz_t y_half;
	mpz_init(y_half);
	mpz_div_ui(y_half,y.mpz,2);
	mpz_abs(y_half,y_half);
	if(mpz_cmp(Z(r).mpz,y_half)>0){
		mpz_sub(Z(r).mpz,Z(r).mpz,y.mpz);
	}
	mpz_clear(y_half);
	return r;
}
/** \brief 求整数的整数次幂.
	\param a 整数.
	\param b 非负整数.
	\return \f$a^b\f$.
	\note 使用自左向右的二进方法.
	\todo a=0
*/
var Power(const Integer& x, uint y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(x.mpz)) {
        mpz_ui_pow_ui(r->mpz, mpz_get_ui(x.mpz), y);
    } else {
        mpz_pow_ui(r->mpz, x.mpz, y);
    }
    return r;
}

/** \brief 求\f$a^b \bmod m\f$.
	\param a,b 整数.
	\param m 非零整数.
	\return \f$a^b \bmod m\f$.
	\note 模幂mpz_powm采用\f$2^k\f$进窗口方法，\f$k\f$根据指数选择.
	\note 对小模使用了Montgomery约化过程(只针对奇数模),
	并且对POWM_THRESHOLD以上模使用带余除法.
	\todo a=0.
*/

var PowerMod(const Integer& x, const Integer& y, const Integer& z) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_powm_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz), z.mpz);
    } else {
        mpz_powm(r->mpz, x.mpz, y.mpz, z.mpz);
    }
    return r;
}

/** \brief 求\f$a\f$模\f$n\f$的逆.
	\param a, n 整数.
	\return \f$a\f$模\f$n\f$的逆.
	\todo 互素判定.
*/
var InverseMod(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    mpz_invert(r->mpz, x.mpz, y.mpz);
    return r;
}

/** \brief 求两个整数的最大公因子.
\param n1, n2 整数.
\return 最大公因子.
\note mpz_gcd对较小数据使用Binary GCD,
对超过GCD_ACCEL_THRESHOLD的数据使用Jebelean-Weber-Sorenson加速算法.
*/
var GCD(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_gcd_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz));
    } else {
        if (mpz_fits_uint_p(x.mpz)) {
            mpz_gcd_ui(r->mpz, y.mpz, mpz_get_ui(x.mpz));
        } else {
            mpz_gcd(r->mpz, x.mpz, y.mpz);
        }
    }
    return r;
}

/** \brief 求两个整数的最小公倍数.
\param n1, n2 整数.
\return 最小公倍数.
*/
var LCM(const Integer& x, const Integer& y) {
    Integer* r = new Integer();
    if (mpz_fits_uint_p(y.mpz)) {
        mpz_lcm_ui(r->mpz, x.mpz, mpz_get_ui(y.mpz));
    } else {
        if (mpz_fits_uint_p(x.mpz)) {
            mpz_lcm_ui(r->mpz, y.mpz, mpz_get_ui(x.mpz));
        } else {
            mpz_lcm(r->mpz, x.mpz, y.mpz);
        }
    }
    return r;
}

/** \brief 求两个整数的扩展最大公因子.
\param n1, n2 整数.
\return 扩展最大公因子\f$\{g,\{r_1,r_2\}\}\f$, 满足\f$g=r_1n_1+r_2n_2\f$.
\note 在GCDEXT_THRESHOLD以上使用Lehmer加速算法.
\todo 扩展到任意个.
*/
var ExtendedGCD(const Integer& x, const Integer& y) {
    var g = GCD(x, y);
    Integer *r1 = new Integer(), *r2 = new Integer();
    mpz_gcdext(g.cast<Integer>().mpz, r1->mpz, r2->mpz, x.mpz, y.mpz);
    return list(g, list(r1, r2));
}
}
}

using namespace nV;

CAPI void CVALUE(System, CoprimeQ)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isObject($.Integer) || !x[2].isObject($.Integer))
        return;
    r = boolean(Number::CoprimeQ(x[1].cast<Integer>(), x[2].cast<Integer>()));
}
CAPI void CVALUE(System, GCD)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isObject($.Integer))
        return;
    r = x[1];
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isObject($.Integer))
            r = Number::GCD(r.cast<Integer>(), x[i].cast<Integer>());
}
CAPI void CVALUE(System, LCM)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isObject($.Integer))
        return;
    r = x[1];
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isObject($.Integer))
            r = Number::LCM(r.cast<Integer>(), x[i].cast<Integer>());
}
CAPI void CVALUE(System, ExtendedGCD)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isObject($.Integer) || !x[2].isObject($.Integer))
        return;
    r = Number::ExtendedGCD(x[1].cast<Integer>(), x[2].cast<Integer>());
}
