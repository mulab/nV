#include "common.h"
#include "objects/integer.h"
#include "objects/interface.h"
#include <gmp.h>

namespace maTHmU {
//////////////////////////////////////
namespace
{
inline mpz_ptr PTR(void *x) { return (mpz_ptr)x; }
inline mpz_ptr PTR(VAR x) { return PTR(x.ptr); }
inline Z& CAST(VAR x) { return *(Z*)(&x); }
}
using namespace Objects::Integer;
//////////////////////////////////////
bool Z::type(VAR x) { return GetType(x) == INTEGER; }
Z& Z::cast(VAR x) { return CAST(x); }
int32_t Z::i(VAR x) { return Int(x); }
Z::Z() : var(Zero()) {}
Z::Z(int32_t x) : var(New(x)) {}
Z::Z(uint32_t x) : var(New(x)) {}
Z::Z(double x) : var(New(x)) {}
Z::Z(const char *x, uint32_t base) : var(New(x,base)) {}
int32_t Z::sgn() const { return Sgn(*this); }
void Z::print() const { Print(*this); printf("\n"); }
Z Z::copy() const { return CAST(New(*this)); }
Z operator ~ (const Z& x) { return CAST(Com(x)); }
Z operator + (const Z& x) { return CAST(Abs(x)); }
Z operator - (const Z& x) { return CAST(Neg(x)); }
Z operator & (const Z& x, const Z& y) { return CAST(And(x,y)); }
Z operator | (const Z& x, const Z& y) { return CAST(Ior(x,y)); }
Z operator ^ (const Z& x, const Z& y) { return CAST(Xor(x,y)); }
Z operator + (const Z& x, const Z& y) { return CAST(Add(x,y)); }
Z operator + (const Z& x, uint32_t y) { return CAST(Add(x,y)); }
Z operator - (const Z& x, const Z& y) { return CAST(Sub(x,y)); }
Z operator - (const Z& x, uint32_t y) { return CAST(Sub(x,y)); }
Z operator * (const Z& x, const Z& y) { return CAST(Mul(x,y)); }
Z operator * (const Z& x, uint32_t y) { return CAST(Mul(x,y)); }
Z operator / (const Z& x, const Z& y) { return CAST(Div(x,y)); }
Z operator / (const Z& x, uint32_t y) { return CAST(Div(x,y)); }
Z operator % (const Z& x, const Z& y) { return CAST(Mod(x,y)); }
uint32_t operator % (const Z& x, uint32_t y) { return Mod(x,y); }
#define Z_DEF_CMP(op)\
	bool operator op (const Z& x,const Z& y) { return Cmp(x,y) op 0; }

Z_DEF_CMP(>)
Z_DEF_CMP(<)
Z_DEF_CMP(!=)
Z_DEF_CMP(>=)
Z_DEF_CMP(<=)
Z_DEF_CMP(==)
//////////////////////////////////////
namespace Objects {
namespace Integer {
//////////////////////////////////////
uint32_t INTEGER;
#define MPZ_DEF_1(name,f,type1,x1)\
var name(type1 x)\
{\
var r = Zero();\
f(PTR(r),x1);\
return r;\
}

#define MPZ_DEF_2(name,f,type1,x1,type2,x2)\
var name(type1 x, type2 y)\
{\
var r = Zero();\
f(PTR(r),x1,x2);\
return r;\
}

#define MPZ_DEF_3(name,f,type1,x1,type2,x2,type3,x3)\
var name(type1 x, type2 y, type3 z)\
{\
	var r = Zero();\
	f(PTR(r),x1,x2,x3);\
	return r;\
}

MPZ_DEF_1(New,mpz_set,VAR,PTR(x))
MPZ_DEF_1(New,mpz_set_ui,uint32_t,x)
MPZ_DEF_1(New,mpz_set_si,int32_t,x)
MPZ_DEF_1(New,mpz_set_d,double,x)
MPZ_DEF_1(NewQ,mpz_set_q,VAR,(mpq_ptr)PTR(x))
MPZ_DEF_1(NewF,mpz_set_f,VAR,(mpf_ptr)PTR(x))
MPZ_DEF_1(Abs,mpz_abs,VAR,PTR(x))
MPZ_DEF_1(Com,mpz_com,VAR,PTR(x))
MPZ_DEF_1(Neg,mpz_neg,VAR,PTR(x))
MPZ_DEF_1(Sqrt,mpz_sqrt,VAR,PTR(x))
MPZ_DEF_2(And,mpz_and,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Ior,mpz_ior,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Xor,mpz_xor,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Add,mpz_add,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Add,mpz_add_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Sub,mpz_sub,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Sub,mpz_sub_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Sub,mpz_ui_sub,uint32_t,x,VAR,PTR(y))
MPZ_DEF_2(Mul,mpz_mul,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Mul,mpz_mul_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Div,mpz_div,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Div,mpz_div_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(DivExact,mpz_divexact,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(DivExact,mpz_divexact_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Mod,mpz_mod,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Sc,mpz_mul_si,VAR,PTR(x),int32_t,y)
MPZ_DEF_2(Pow,mpz_pow_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Pow,mpz_ui_pow_ui,uint32_t,x,uint32_t,y)
MPZ_DEF_2(Root,mpz_root,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Gcd,mpz_gcd,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Lcm,mpz_lcm,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_2(Lcm,mpz_lcm_ui,VAR,PTR(x),uint32_t,y)
MPZ_DEF_2(Inv,mpz_invert,VAR,PTR(x),VAR,PTR(y))
MPZ_DEF_3(Powm,mpz_powm,VAR,PTR(x),VAR,PTR(y),VAR,PTR(z))
MPZ_DEF_3(Powm,mpz_powm_ui,VAR,PTR(x),uint32_t,y,VAR,PTR(z))
uint32_t Gcd(VAR x, uint32_t y) { return mpz_gcd_ui(0,PTR(x),y); }
var Shift(VAR x, int32_t y)
{
	var r = Zero();
	if(y >= 0) mpz_mul_2exp(PTR(r),PTR(x),(uint32_t)y);
	else mpz_div_2exp(PTR(r),PTR(x),(uint32_t)-y);
	return r;
}
var Zero()
{
	mpz_ptr r = new __mpz_struct;
	mpz_init_set_ui(r, 0);
	return var(r, INTEGER);
}
var One() { return New(1); }
int32_t Int(VAR x) { return mpz_get_si(PTR(x)); }
var New(const char *x, uint32_t base)
{
	var r = Zero();
	mpz_set_str(PTR(r), x, base);
	return r;
}
var Print(VAR x)
{
	mpz_out_str(0,10,PTR(x));
	return x;
}
uint32_t Mod(VAR x, uint32_t y)
{
	return mpz_fdiv_ui(PTR(x), y);
}
int32_t Cmp(VAR x, VAR y)
{
	return mpz_cmp(PTR(x),PTR(y));
}
int32_t Sgn(VAR x)
{
	return mpz_sgn(PTR(x));
}
int32_t Divisible(VAR x, VAR y)
{
	return mpz_divisible_p(PTR(x),PTR(y));
}
int32_t Divisible(VAR x, uint32_t y)
{
	return mpz_divisible_ui_p(PTR(x),y);
}
int32_t Congruent(VAR x, VAR y, VAR z)
{
	return mpz_congruent_p(PTR(x),PTR(y),PTR(z));
}
int32_t Congruent(VAR x, uint32_t y, uint32_t z)
{
	return mpz_congruent_ui_p(PTR(x),y,z);
}
void DivMod(VAR x, VAR y, var &q, var &r)
{
	q = Zero(); r = Zero();
	mpz_fdiv_qr(PTR(q), PTR(r), PTR(x), PTR(y));
}
uint32_t DivMod(VAR x, uint32_t y, var &q)
{
	q = Zero();
	return mpz_fdiv_q_ui(PTR(q), PTR(x), y);
}
void GcdExt(VAR x, VAR y, var &g, var &s, var &t)
{
	g = Zero(); s = Zero(); t = Zero();
	mpz_gcdext(PTR(g), PTR(s), PTR(t), PTR(x), PTR(y));
}
void RootRem(VAR x, uint32_t y, var& q, var& r)
{
	q = Zero(); r = Zero();
	mpz_rootrem(PTR(q),PTR(r),PTR(x),y);
}
void SqrtRem(VAR x, var& q, var& r)
{
	q = Zero(); r = Zero();
	mpz_sqrtrem(PTR(q),PTR(r),PTR(x));
}
void DESTROY(void *x) { mpz_clear(PTR(x)); delete PTR(x); }
#define MYREG(op) REG(op,INTEGER)
void Init()
{
	INTEGER = AddType(DESTROY);
	MYREG(Zero);
	MYREG(One);
	MYREG(New);
	MYREG(Abs);
	MYREG(Neg);
	MYREG(Add);
	MYREG(Sub);
	MYREG(Mul);
	MYREG(Div);
	MYREG(Gcd);
	MYREG(Lcm);
	MYREG(Sc);
	MYREG(Cmp);
	MYREG(Print);
}
//////////////////////////////////////
}
}
}
