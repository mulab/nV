#include "common.h"
#include <mU/Number.h>
#include <mU/Kernel.h>
#include <mU/Exceptions.h>
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

CPROC_ATOMIC(System_CoprimeQ)
{
	using maTHmU::Modules::NumberTheory::CoprimeQ;
	using maTHmU::list;

	// TODO: we don't need to handle lists, because we are Listable
	size_t n = Size(x);
	bool has_list = false;
	size_t dim = 0;

	for (int i = 0; i < n; ++i)
	{
		Var arg = At(x, i);
		if (VecQ(arg))
		{
			size_t n2 = Size(arg);
			if (has_list && n2 != dim)
			{
				throw LogicError(L"all list arguments of CoprimeQ should have the same length");
			}
			for (int j = 0; j < n2; ++j)
			{
				if (!IntQ(At(arg, j)))
				{
					throw LogicError(L"we only accept integer or list of integers in CoprimeQ");
				}
			}

			dim = n2;
			has_list = true;
		}
		else if (!IntQ(arg))
		{
			throw LogicError(L"we only accept integer or list of integers in CoprimeQ");
		}
	}

	if (n==0)
	{
		return False;
	}
	else if (n==1)
	{
		Var arg = At(x, 0);
		return Z::Cmp(arg, 1)==0 || Z::Cmp(arg, -1)==0 ? True : False;
	}

	if (has_list)
	{
		var results = Vec();
		for (int i = 0; i < dim; ++i)
		{
			list instance;
			for (int j = 0; j < n; ++j)
			{
				Var arg = At(x, j);
				if (VecQ(arg))
				{
					Var item = At(arg, i);
					assert(IntQ(item));
					instance.push(item);
				}
				else
				{
					assert(IntQ(arg));
					instance.push(arg);
				}
			}

			Push(results, CoprimeQ(instance) ? True : False);
		}

		return results;
	}
	else
	{
		return CoprimeQ(Tolist(x)) ? True : False;
	}
}
