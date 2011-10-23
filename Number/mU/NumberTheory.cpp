#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/Kernel.h>
#include "NumberTheory.h"

namespace Old = maTHmU::Modules::NumberTheory;
namespace Integer = maTHmU::Objects::Integer;
namespace Rational = maTHmU::Objects::Rational;
namespace List = maTHmU::Objects::List;

using namespace mU;

namespace {
inline maTHmU::Z ToZ(Var x)
{
	maTHmU::Z r;
	mpz_set((mpz_ptr)r.ptr,CInt(x));
	return r;
}
inline var FromZ(maTHmU::var x)
{
	var r = Int();
	mpz_set(CInt(r),(mpz_ptr)x.ptr);
	return r;
}
inline maTHmU::list Tolist(Var x)
{
	size_t n = Size(x);
	maTHmU::list r(n);
	for(size_t i = 0; i < n; ++i)
		r[i] = ToZ(At(x,i));
	return r;
}
inline var Fromlist(maTHmU::var x)
{
	size_t n = List::Size(x);
	var r = Vec(n);
	for(size_t i = 0; i < n; ++i)
		At(r,i) = FromZ(List::At(x,i));
	return r;
}
}

CPROC_ATOMIC(System_FactorInteger)
{
	if (IntQ(At(x,0)))
	{
		maTHmU::list y = Old::FactorInteger(ToZ(At(x,0)));
		size_t n = y.size();
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = Fromlist(y[i]);
		return r;
	}
	return x;
}
CPROC_ATOMIC(System_SeedRandom)
{
	if(Size(x) == 1)
		Old::SeedRandom();
	else if(IntQ(At(x,0)))
		Old::SeedRandom(ToZ(At(x,0)));
	return Null;
}
CPROC_ATOMIC(System_RandomInteger)
{
	if (IntQ(At(x,0))) return FromZ(Old::RandomInteger(ToZ(At(x,0))));
	return x;
}
CPROC_ATOMIC(System_RandomPrime)
{
	if (IntQ(At(x,0))) return FromZ(Old::RandomPrime(ToZ(At(x,0))));
	return x;
}
CPROC_ATOMIC(System_NextPrime)
{
	if (Size(x)==1 && IntQ(At(x,0))) return FromZ(Old::NextPrime(ToZ(At(x,0))));
	if (Size(x)==2 && IntQ(At(x,0)) && IntQ(At(x,1))) return FromZ(Old::NextPrime(ToZ(At(x,0)),ToZ(At(x,1))));
	return x;
}
CPROC_ATOMIC(System_IntegerLength)
{
	if (Size(x)==1 && IntQ(At(x,0))) return Int((mU::uint)Old::IntegerLength(ToZ(At(x,0)),10));
	if (Size(x)==2 && IntQ(At(x,0)) && IntQ(At(x,1))) return Int((mU::uint)Old::IntegerLength(ToZ(At(x,0)),Z::UI(At(x,1))));
	return x;
}
CPROC_ATOMIC(System_GCD)
{
	size_t n = Size(x);
	if (n==2 && IntQ(At(x,0)) && IntQ(At(x,1))) return FromZ(Old::GCD(ToZ(At(x,0)),ToZ(At(x,1))));
	if(n>2) {
		int flag=1;
		for (size_t i=0;i<n;++i) {
			if (!IntQ(At(x,i))) {flag=0;break;}
		}
		if (flag==1) {
			return FromZ(Old::GCD(Tolist(x)));
		}
	}
	return x;
}
CPROC_ATOMIC(System_LCM)
{
	size_t n = Size(x);
	if (n==2 && IntQ(At(x,0)) && IntQ(At(x,1))) return FromZ(Old::LCM(ToZ(At(x,0)),ToZ(At(x,1))));
	if(n>2) {
		int flag=1;
		for (size_t i=0;i<n;++i) {
			if (!IntQ(At(x,i))) {flag=0;break;}
		}
		if (flag==1) {
			return FromZ(Old::LCM(Tolist(x)));
		}
	}
	return x;
}
CPROC_ATOMIC(System_ExtendedGCD)
{
	if (Size(x)==2 && IntQ(At(x,0)) && IntQ(At(x,1)))
	{
		maTHmU::list y = Old::ExtendedGCD(ToZ(At(x,0)),ToZ(At(x,1)));
		size_t n = y.size();
		var r = Vec(n);
		At(r,0) = FromZ(y[0]);
		for(size_t i = 1; i < n; ++i)
			At(r,i) = Fromlist(y[i]);
		return r;
	}
	return x;
}
CPROC_ATOMIC(System_ChineseRemainder)
{
	if (Size(x)==2 && VecQ(At(x,0)) && VecQ(At(x,1))) {
		var a=At(x,0),b=At(x,1);
		size_t n=Size(a);
		if(n == Size(b)) {
			int flag=1;
			for (size_t i=0;i<n;i++) {
				if (!IntQ(At(a,i)) || !IntQ(At(b,i))) {flag=0;break; }
			}
			if (flag==1) {
				return FromZ(Old::ChineseRemainder(Tolist(a),Tolist(b)));
			}
		}
	}
	return x;
}
CPROC_ATOMIC(System_Radical)
{
	if(IntQ(At(x,1)))
	{
		sint se = Z::SI(At(x,1));
		return Ex(TAG(Power),Vec(At(x,0), se > 0 ? Rat(1L,se) : Rat(-1L,-se)));
	}
	return 0;
}
CPROC_ATOMIC(System_PrimeQ)
{
	return IntQ(At(x,0)) && Old::PrimeQ(ToZ(At(x,0))) ? True : False;
}