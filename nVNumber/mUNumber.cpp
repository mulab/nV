#include "common.h"
#include <mU/Function.h>
#include <mU/Complex.h>
#include <mU/Kernel.h>
#include "mU/NumberTheory.h"

namespace Old = maTHmU::Modules::NumberTheory;
namespace Integer = maTHmU::Objects::Integer;
namespace Rational = maTHmU::Objects::Rational;
namespace List = maTHmU::Objects::List;

using namespace mU;

CAPI void mUInstall() {
	static bool Initialized = false;
	if(Initialized) return;

	Integer::Init();
	Rational::Init();
	List::Init();
	Old::Randomize();
	
	Initialized = true;
}
CAPI void mUUninstall() {
	//wcout << "#nVNumber# Uninstall Successfully!" << std::endl;
}

using namespace Function;

CPROC_ATOMIC(System_Factorial)
{
	if(IntQ(At(x,0))) return Factorial(Z::UI(At(x,0)));
	return 0;
}
CPROC_ATOMIC(System_Binomial)
{
	if(IntQ(At(x,0))) return Binomial(At(x,0),Z::UI(At(x,1)));
	return 0;
}
CPROC_ATOMIC(System_Fibonacci)
{
	if(IntQ(At(x,0))) return Fibonacci(Z::UI(At(x,0)));
	return 0;
}
CPROC_ATOMIC(System_Lucas)
{
	if(IntQ(At(x,0))) return Lucas(Z::UI(At(x,0)));
	return 0;
}
CPROC_ATOMIC(System_EulerGamma)
{
	return Euler();
}
CPROC_ATOMIC(System_Catalan)
{
	return Catalan();
}
CPROC_ATOMIC(System_Log)
{
	if(FltQ(At(x,0))) return Log(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Log2)
{
	if(FltQ(At(x,0))) return Log2(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Log10)
{
	if(FltQ(At(x,0))) return Log10(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Exp)
{
	if(FltQ(At(x,0))) return Exp(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Cos)
{
	if(FltQ(At(x,0))) return Cos(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Sin)
{
	if(FltQ(At(x,0))) return Sin(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Tan)
{
	if(FltQ(At(x,0))) return Tan(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Sec)
{
	if(FltQ(At(x,0))) return Sec(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Csc)
{
	if(FltQ(At(x,0))) return Csc(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Cot)
{
	if(FltQ(At(x,0))) return Cot(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcCos)
{
	if(FltQ(At(x,0))) return Acos(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcSin)
{
	if(FltQ(At(x,0))) return Asin(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcTan)
{
	if(FltQ(At(x,0))) return Atan(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Cosh)
{
	if(FltQ(At(x,0))) return Cosh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Sinh)
{
	if(FltQ(At(x,0))) return Sinh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Tanh)
{
	if(FltQ(At(x,0))) return Tanh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Sech)
{
	if(FltQ(At(x,0))) return Sech(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Csch)
{
	if(FltQ(At(x,0))) return Csch(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Coth)
{
	if(FltQ(At(x,0))) return Coth(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcCosh)
{
	if(FltQ(At(x,0))) return Acosh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcSinh)
{
	if(FltQ(At(x,0))) return Asinh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_ArcTanh)
{
	if(FltQ(At(x,0))) return Atanh(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Gamma)
{
	if(FltQ(At(x,0))) return Gamma(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_LogGamma)
{
	if(FltQ(At(x,0))) return Lngamma(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Zeta)
{
	if(FltQ(At(x,0))) return Zeta(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Erf)
{
	if(FltQ(At(x,0))) return Erf(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Erfc)
{
	if(FltQ(At(x,0))) return Erfc(At(x,0));
	return 0;
}
CPROC_ATOMIC(System_Sqrt)
{
	Var arg = At(x, 0);
	if (FltQ(arg))
	{
		const int sign = F::Sgn(arg);
		var abs = Flt();
		F::Abs(abs, arg);
		var root = Sqrtf(abs);
		if (sign < 0)
		{
			return Complex(F::Zero, root);
		}
		else
		{
			return root;
		}
	}
	if (IntQ(arg))
	{
		const int sign = Z::Sgn(arg);
		var abs = Int();
		Z::Abs(abs, arg);
		var root = Int();
		var remainder = Int();
		mpz_sqrtrem(CInt(root), CInt(remainder), CInt(abs));
		if (Z::Sgn(remainder) == 0)
		{
			if (sign < 0)
			{
				return Complex(Z::Zero, root);
			}
			else
			{
				return root;
			}
		}
	}
	return Power(At(x,0),Rat(1L,2L));
}
CPROC_ATOMIC(System_Pow)
{
	if(FltQ(At(x,0)) && FltQ(At(x,1))) return Powf(At(x,0),At(x,1));
	return 0;
}
CPROC_ATOMIC(System_Pi)
{
	return Pif();
}
CPROC_ATOMIC(System_E)
{
	return Exp(Flt(1L));
}

CPROC_INT(System_Sign, 1)
{
    Var arg = At(x, 0);
    
    switch (Type(arg))
    {
        case TYPE(int):
        case TYPE(rat):
        case TYPE(flt):
        {
            const int sign = N::Sgn(arg);
            if (sign > 0)
            {
                return mU::Z::One;
            }
            else if (sign < 0)
            {
                return mU::Z::NOne;
            }
            else
            {
                return mU::Z::Zero;
            }
            break;
        }
        case TYPE(obj):
            if (Tag(arg) == SYSTEM_SYM(Complex))
            {
                var real = Re(arg);
                var imaginary = Im(arg);
                
                assert (NumQ(real) && NumQ(imaginary));
                
                if (N::Sgn(real) == 0 && N::Sgn(imaginary) == 0)
                {
                    return mU::Z::Zero;
                }
                
                var abs = Ex(SYSTEM_SYM(Sqrt), Vec(N::Add(N::Mul(real, real), N::Mul(imaginary, imaginary))));
                
                var invabs = Ex(TAG(Power), Vec(abs, mU::Z::NOne));
                
                return Eval(Ex(TAG(Times), Vec(arg, invabs)));
            }
            // deliberate fall-through
        default:
            throw LogicError(L"The first argument of Sign must be an explicit number like 1, 2/3, 2.3, or Complex[1, 3]");
    }
    
    return 0;
}
