#include "common.h"
#include "objects/integer.h"
#include "objects/rational.h"
#include "objects/interface.h"
#include <gmp.h>

namespace maTHmU {
//////////////////////////////////////
namespace
{
inline mpq_ptr PTR(void *x) { return (mpq_ptr)x; }
inline mpq_ptr PTR(VAR x) { return PTR(x.ptr); }
inline Q& CAST(VAR x) { return *(Q*)(&x); }
}
using namespace Objects::Rational;
//////////////////////////////////////
bool Q::type(VAR x) { return GetType(x) == RATIONAL; }
Q& Q::cast(VAR x) { return CAST(x); }
double Q::d(VAR x) { return Double(x); }
Z Q::num(VAR x) { return Num(x); }
Z Q::den(VAR x) { return Den(x); }
Q::Q() : var(Zero()) {}
Q::Q(int32_t x, uint32_t y) : var(New(x,y)) {}
Q::Q(uint32_t x, uint32_t y) : var(New(x,y)) {}
Q::Q(double x) : var(New(x)) {}
Q::Q(const char *x, uint32_t base) : var(New(x,base)) {}
int32_t Q::sgn() const { return Sgn(*this); }
void Q::print() const { Print(*this); printf("\n"); }
Q Q::copy() const { return CAST(New(*this)); }
void Q::reduce() const { Reduce(*this); }
Q operator + (const Q& x) { return CAST(Abs(x)); }
Q operator - (const Q& x) { return CAST(Neg(x)); }
Q operator + (const Q& x, const Q& y) { return CAST(Add(x,y)); }
Q operator - (const Q& x, const Q& y) { return CAST(Sub(x,y)); }
Q operator * (const Q& x, const Q& y) { return CAST(Mul(x,y)); }
Q operator / (const Q& x, const Q& y) { return CAST(Div(x,y)); }
#define Q_DEF_CMP(op)\
	bool operator op (const Q& x,const Q& y) { return Cmp(x,y) op 0; }

Q_DEF_CMP(>)
Q_DEF_CMP(<)
Q_DEF_CMP(!=)
Q_DEF_CMP(>=)
Q_DEF_CMP(<=)
Q_DEF_CMP(==)
//////////////////////////////////////
namespace Objects {
namespace Rational {
//////////////////////////////////////
uint32_t RATIONAL;
#define MPQ_DEF_1(name,f,type1,x1)\
var name(type1 x)\
{\
var r = Zero();\
f(PTR(r),x1);\
return r;\
}

#define MPQ_DEF_2(name,f,type1,x1,type2,x2)\
var name(type1 x, type2 y)\
{\
var r = Zero();\
f(PTR(r),x1,x2);\
return r;\
}

//////////////////////////////////////
MPQ_DEF_1(New,mpq_set,VAR,PTR(x))
MPQ_DEF_1(New,mpq_set_d,double,x)
MPQ_DEF_1(NewZ,mpq_set_z,VAR,(mpz_ptr)PTR(x))
MPQ_DEF_1(NewF,mpq_set_f,VAR,(mpf_ptr)PTR(x))
MPQ_DEF_1(Abs,mpq_abs,VAR,PTR(x))
MPQ_DEF_1(Neg,mpq_neg,VAR,PTR(x))
MPQ_DEF_1(Inv,mpq_inv,VAR,PTR(x))
MPQ_DEF_2(New,mpq_set_si,int32_t,x,uint32_t,y)
MPQ_DEF_2(New,mpq_set_ui,uint32_t,x,uint32_t,y)
MPQ_DEF_2(Add,mpq_add,VAR,PTR(x),VAR,PTR(y))
MPQ_DEF_2(Sub,mpq_sub,VAR,PTR(x),VAR,PTR(y))
MPQ_DEF_2(Mul,mpq_mul,VAR,PTR(x),VAR,PTR(y))
MPQ_DEF_2(Div,mpq_div,VAR,PTR(x),VAR,PTR(y))
var Shift(VAR x, int32_t y)
{
	var r = Zero();
	if(y >= 0) mpq_mul_2exp(PTR(r),PTR(x),(uint32_t)y);
	else mpq_div_2exp(PTR(r),PTR(x),(uint32_t)-y);
	return r;
}
var Zero()
{
	mpq_ptr r = new __mpq_struct;
	mpq_init(r); mpq_set_ui(PTR(r),0,0);
	return var(r, RATIONAL);
}
var One() { return New(1,1); }
double Double(VAR x) { return mpq_get_d(PTR(x)); }
var NewZ(VAR x, VAR y)
{
	var r = Zero();
	mpq_set_num(PTR(r),(mpz_ptr)PTR(x));
	mpq_set_den(PTR(r),(mpz_ptr)PTR(y));
	return r;
}
var New(const char *x, uint32_t base)
{
	var r = Zero();
	mpq_set_str(PTR(r), x, base);
	return r;
}
var Print(VAR x)
{
	mpq_out_str(0,10,PTR(x));
	return x;
}
var Sc(VAR x, int32_t y)
{
	return Mul(x, New(y, 1));
}
int32_t Cmp(VAR x, VAR y)
{
	return mpq_cmp(PTR(x),PTR(y));
}
int32_t Equ(VAR x, VAR y)
{
	return mpq_equal(PTR(x),PTR(y));
}
int32_t Sgn(VAR x)
{
	return mpq_sgn(PTR(x));
}
void Reduce(VAR x)
{
	mpq_canonicalize(PTR(x));
}
Z Num(VAR x) { Z r; mpz_set((mpz_ptr)PTR(r),mpq_numref(PTR(x))); return r; }
Z Den(VAR x) { Z r; mpz_set((mpz_ptr)PTR(r),mpq_denref(PTR(x))); return r; }
void DESTROY(void *x) { mpq_clear(PTR(x)); delete PTR(x); }
#define MYREG(op) REG(op,RATIONAL)
void Init()
{
	RATIONAL = AddType(DESTROY);
	MYREG(Zero);
	MYREG(One);
	MYREG(New);
	MYREG(Abs);
	MYREG(Neg);
	MYREG(Add);
	MYREG(Sub);
	MYREG(Mul);
	MYREG(Div);
	MYREG(Sc);
	MYREG(Cmp);
	MYREG(Print);
}
//////////////////////////////////////
}
}
}
