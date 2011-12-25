#include "common.h"
#include <mU/Number.h>
#include <mU/Complex.h>
#include <mU/System.h>

using namespace mU;

CPROC_INT(System_Complex, 2) {
	Var v1 = At(x, 0);
	Var v2 = At(x, 1);

	if (NumQ(v1) && NumQ(v2))
	{
		return Complex(v1, v2);
	}
	else
	{
		return 0;
	}
}

CPROC_INT(System_ComplexRe, 1) {
	Var v = At(x, 0);

	if (ObjQ(v, TAG(Complex)))
	{
		return Re(v);
	}
	else if (NumQ(v))
	{
		return v;
	}
	else
	{
		return 0;
	}
}

CPROC_INT(System_ComplexIm, 1) {
	Var v = At(x, 0);

	if (ObjQ(v, TAG(Complex)))
	{
		return Im(v);
	}
	else if (NumQ(v))
	{
		return Int(static_cast<mU::uint>(0));
	}
	else
	{
		return 0;
	}
}

CPROC_INT(System_ComplexConjugate, 1)
{
	Var v = At(x, 0);

	switch (Type(v))
	{
	case TYPE(int):
	case TYPE(flt):
	case TYPE(rat):
		return v;
	case TYPE(obj):
		return ObjQ(v, TAG(Complex)) ? Complex(Re(v), N::Neg(Im(v))) : 0;
	default:
		return 0;
	}
}
