#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class VAR_API Q : public var {
public:
	static bool type(VAR);
	static Q& cast(VAR);
	static double d(VAR);
	static Z num(VAR);
	static Z den(VAR);
	Q();
	Q(int32_t,uint32_t);
	Q(uint32_t,uint32_t);
	Q(double);
	Q(const char*,uint32_t);

	void print() const;
	int32_t sgn() const;
	Q copy() const;
	void reduce() const;
};
VAR_API Q operator + (const Q&);
VAR_API Q operator - (const Q&);
VAR_API Q operator + (const Q&,const Q&);
VAR_API Q operator - (const Q&,const Q&);
VAR_API Q operator * (const Q&,const Q&);
VAR_API Q operator / (const Q&,const Q&);
VAR_API bool operator < (const Q&,const Q&);
VAR_API bool operator == (const Q&,const Q&);
VAR_API bool operator > (const Q&,const Q&);
VAR_API bool operator >= (const Q&,const Q&);
VAR_API bool operator != (const Q&,const Q&);
VAR_API bool operator <= (const Q&,const Q&);
//////////////////////////////////////
namespace Objects {
namespace Rational {
//////////////////////////////////////
extern VAR_API uint32_t RATIONAL;
VAR_API var Zero();
VAR_API var One();
VAR_API double Double(VAR);
VAR_API var New(VAR);
VAR_API var New(int32_t,uint32_t);
VAR_API var New(uint32_t,uint32_t);
VAR_API var New(double);
VAR_API var NewZ(VAR);
VAR_API var NewZ(VAR,VAR);
VAR_API var NewF(VAR);
VAR_API var New(const char*,uint32_t);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Neg(VAR);
VAR_API var Inv(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Sub(VAR,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Div(VAR,VAR);
VAR_API var Sc(VAR,int32_t);
VAR_API var Shift(VAR,int32_t);
VAR_API int32_t Cmp(VAR,VAR);
VAR_API int32_t Equ(VAR,VAR);
VAR_API int32_t Sgn(VAR);
VAR_API void Reduce(VAR);
VAR_API Z Num(VAR); 
VAR_API Z Den(VAR);
VAR_API void Init();
//////////////////////////////////////
}
}
}