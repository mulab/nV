#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class VAR_API Z : public var {
public:
	static bool type(VAR);
	static Z& cast(VAR);
	static int32_t i(VAR);
	Z();
	Z(int32_t);
	Z(uint32_t);
	Z(double);
	Z(const char*,uint32_t);

	void print() const;
	int32_t sgn() const;
	Z copy() const;
};
VAR_API Z operator ~ (const Z&);
VAR_API Z operator + (const Z&);
VAR_API Z operator - (const Z&);
VAR_API Z operator & (const Z&,const Z&);
VAR_API Z operator | (const Z&,const Z&);
VAR_API Z operator ^ (const Z&,const Z&);
VAR_API Z operator + (const Z&,const Z&);
VAR_API Z operator + (const Z&,uint32_t);
VAR_API Z operator - (const Z&,const Z&);
VAR_API Z operator - (const Z&,uint32_t);
VAR_API Z operator * (const Z&,const Z&);
VAR_API Z operator * (const Z&,uint32_t);
VAR_API Z operator / (const Z&,const Z&);
VAR_API Z operator / (const Z&,uint32_t);
VAR_API Z operator % (const Z&,const Z&);
VAR_API uint32_t operator % (const Z&,uint32_t);
VAR_API bool operator < (const Z&,const Z&);
VAR_API bool operator == (const Z&,const Z&);
VAR_API bool operator > (const Z&,const Z&);
VAR_API bool operator >= (const Z&,const Z&);
VAR_API bool operator != (const Z&,const Z&);
VAR_API bool operator <= (const Z&,const Z&);
//////////////////////////////////////
namespace Objects {
namespace Integer {
//////////////////////////////////////
extern VAR_API uint32_t INTEGER;
VAR_API var Zero();
VAR_API var One();
VAR_API int32_t Int(VAR);
VAR_API var New(VAR);
VAR_API var New(int32_t);
VAR_API var New(uint32_t);
VAR_API var New(double);
VAR_API var NewQ(VAR);
VAR_API var NewF(VAR);
VAR_API var New(const char*,uint32_t);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Com(VAR);
VAR_API var Neg(VAR);
VAR_API var And(VAR,VAR);
VAR_API var Ior(VAR,VAR);
VAR_API var Xor(VAR,VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Add(VAR,uint32_t);
VAR_API var Sub(VAR,VAR);
VAR_API var Sub(VAR,uint32_t);
VAR_API var Sub(uint32_t,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Mul(VAR,uint32_t);
VAR_API var Div(VAR,VAR);
VAR_API var Div(VAR,uint32_t);
VAR_API var DivExact(VAR,VAR);
VAR_API var DivExact(VAR,uint32_t);
VAR_API void DivMod(VAR,VAR,var&,var&);
VAR_API uint32_t DivMod(VAR,uint32_t,var&);
VAR_API var Mod(VAR,VAR);
VAR_API uint32_t Mod(VAR,uint32_t);
VAR_API var Sc(VAR,int32_t);
VAR_API var Gcd(VAR,VAR);
VAR_API uint32_t Gcd(VAR,uint32_t);
VAR_API var Lcm(VAR,VAR);
VAR_API var Lcm(VAR,uint32_t);
VAR_API void GcdExt(VAR,VAR,var&,var&,var&);
VAR_API var Pow(VAR,uint32_t);
VAR_API var Pow(uint32_t,uint32_t);
VAR_API var Powm(VAR,VAR);
VAR_API var Powm(VAR,uint32_t);
VAR_API var Root(VAR,uint32_t);
VAR_API void RootRem(VAR,uint32_t,var&,var&);
VAR_API var Sqrt(VAR);
VAR_API void SqrtRem(VAR,var&,var&);
VAR_API var Inv(VAR,VAR);
VAR_API var Shift(VAR,int32_t);
VAR_API int32_t Cmp(VAR,VAR);
VAR_API int32_t Sgn(VAR);
VAR_API int32_t Divisible(VAR,VAR);
VAR_API int32_t Divisible(VAR,uint32_t);
VAR_API int32_t Congruent(VAR,VAR,VAR);
VAR_API int32_t Congruent(VAR,uint32_t,uint32_t);
VAR_API void Init();
//////////////////////////////////////
}
}
}