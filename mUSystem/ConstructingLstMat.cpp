#include "StdAfx.h"
#include <mU/Exceptions.h>
#include <mU/Number.h>
#include <mU/System.h>
//#include <crtdbg.h>

namespace mU {
namespace {
var Table(dict_t &d, Var x, size_t n, const var *y)
{
	if(n > 0 && VecQ(y[0]))
	{
		var a = y[0], c = At(a,0);
		size_t m = Size(a);
		for(size_t i = 1; i < m; ++i)
			At(a,i) = Eval(Subs(d,At(a,i)));
		var r;
		switch(m)
		{
		case 1:
			c = Subs(d,c);
			c = Eval(c);
			if(IntQ(c) && Z::Sgn(c) > 0)
			{
				size_t m = Z::UI(c);
				r = Vec(m);
				for(size_t i = 0; i < m; ++i)
					At(r,i) = Table(d,x,n - 1,y + 1);
			}
			break;
		case 2:
			if(IntQ(At(a,1)))
			{
				if(Z::Sgn(At(a,1)) > 0)
				{
					size_t m = Z::UI(At(a,1));
					r = Vec(m);
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int((uint)i + 1);
						At(r,i) = Table(d,x,n - 1,y + 1);
					}
				}
			}
			else if(VecQ(At(a,1)))
			{
				var u = At(a,1);
				size_t m = Size(u);
				r = Vec(m);
				for(size_t i = 0; i < m; ++i)
				{
					d[c] = At(u,i);
					At(r,i) = Table(d,x,n - 1,y + 1);
				}
			}
			break;
		case 3:
			if(IntQ(At(a,1)) && IntQ(At(a,2)))
			{
				sint u = Z::SI(At(a,1)), v = Z::SI(At(a,2));
				if(u <= v)
				{
					size_t m = v + 1 - u;
					r = Vec(m);
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int(u + i);
						At(r,i) = Table(d,x,n - 1,y + 1);
					}
				}
			}
			break;
		case 4:
			var u = At(a,1), v = At(a,2), w = At(a,3);
			r = Vec();
			if(Less(u,v))
				while(!Less(v,u))
				{
					d[c] = u;
					Push(r,Table(d,x,n - 1,y + 1));
					u = Plus(u,w);
				}
			else
				while(!Greater(v,u))
				{
					d[c] = u;
					Push(r,Table(d,x,n - 1,y + 1));
					u = Plus(u,w);
				}
				break;
		}
		return r ? r : Vec();
	}
	return Eval(Subs(d,x));
}
}//namespace

var Table(Var x, size_t n, const var *y)
{
    dict_t d;
	return Table(d,x,n,y);
}
var Array(Var h, Var b, size_t n, const var *x)
{
	if(n > 0 && IntQ(x[0]) && Z::Sgn(x[0]) > 0)
	{
		size_t m = Z::UI(x[0]);
		var r = Vec(m);
		for(size_t i = 0; i < m; ++i)
		{
			var t = Vec();
			CVec(t) = CVec(b);
			Push(t,Int((uint)i+1));
			At(r,i) = Array(h,t,n - 1,x + 1);
		}
		return r;
	}
	return Eval(Ex(h,b));
}
var Array(Var h, Var b, size_t n, const var *x, const var *y)
{
	if(n > 0 && IntQ(x[0]) && Z::Sgn(x[0]) > 0)
	{
		size_t m = Z::UI(x[0]);
		var r = Vec(m);
		for(size_t i = 0; i < m; ++i)
		{
			var t = Vec();
			CVec(t) = CVec(b);
			Push(t,Plus(y[0],Int((uint)i+1)));
			At(r,i) = Array(h,t,n - 1,x + 1,y + 1);
		}
		return r;
	}
	return Eval(Ex(h,b));
}
}//namespace mU

using namespace mU;

CAPI CPROC(System_List)
{
	return x;
}

CPROC_INT2(System_Array, 2, 3)
{
	size_t n = Size(x);
	var h = At(x,0);
	var b = Vec();
	size_t m = Size(At(x,1));
	if(n == 2) return Array(h,b,m,&At(At(x,1),0));
	if(n == 3) return Array(h,b,m,&At(At(x,1),0),&At(At(x,2),0));
	return 0;
}

CAPI CPROC(System_Table)
{
	return Table(At(x,0),Size(x) - 1,&At(x,1));
}

var ConstantArray(Var expr, Var dims, size_t offset, size_t size)
{
	assert(size > offset);
	Var firstv = At(dims, offset);
#undef max
	if (Z::Cmp(firstv, std::numeric_limits<mU::uint>::max()) > 0)
	{
		throw LogicError(L"dimension limit to ConstantArray too large");
	}
	const mU::uint repeat = mpz_get_ui(CInt(firstv));
	if (repeat == 0)
	{
		return Vec();
	}
	var result = Vec(repeat);
	var children = size == offset + 1 ? var(expr) : ConstantArray(expr, dims, offset + 1, size);

	for (mU::uint i = 0; i < repeat; ++i)
	{
		At(result, i) = children;
	}

	return result;
}

CPROC_INT(System_ConstantArray, 2)
{
	var expr = At(x, 0);
	var dims = At(x, 1);

	if (Type(dims) == TYPE(int))
	{
		dims = Vec(dims);
	}
	else if (Type(dims) == TYPE(vec))
	{
		const int size = Size(dims);
		if (size == 0)
		{
			throw LogicError(L"the 2nd argument to ConstantArray should be an non-negative integer or a non-empty list of non-negative integers");
		}
		for (int i = 0; i < size; ++i)
		{
			Var elem = At(dims, i);
			if (Type(elem) != TYPE(int) || mpz_sgn(CInt(elem)) < 0)
			{
				throw LogicError(L"the 2nd argument to ConstantArray should be an non-negative integer or a non-empty list of non-negative integers");
			}
		}
	}
	else
	{
		throw LogicError(L"the 2nd argument to ConstantArray should be an non-negative integer or a non-empty list of non-negative integers");
	}

	return ConstantArray(expr, dims, 0, Size(dims));
}
