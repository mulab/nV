#include "common.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/System.h>

namespace mU {
namespace {
void Do(dict_t &d, Var x, size_t n, const var *y)
{
	if(n > 0 && VecQ(y[0]))
	{
		var a = y[0], c = At(a,0);
		size_t m = Size(a);
		for(size_t i = 1; i < m; ++i)
			At(a,i) = Eval(Subs(d,At(a,i)));
		switch(m)
		{
		case 1:
			c = Subs(d,c);
			if(IntQ(c) && Z::Sgn(c) > 0)
			{
				size_t m = Z::UI(c);
				for(size_t i = 0; i < m; ++i)
					Do(d,x,n - 1,y + 1);
			}
			break;
		case 2:
			if(IntQ(At(a,1)))
			{
				if(Z::Sgn(At(a,1)) > 0)
				{
					size_t m = Z::UI(At(a,1));
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int((uint)i + 1);
						Do(d,x,n - 1,y + 1);
					}
				}
			}
			else if(VecQ(At(a,1)))
			{
				var u = At(a,1);
				size_t m = Size(u);
				for(size_t i = 0; i < m; ++i)
				{
					d[c] = At(u,i);
					Do(d,x,n - 1,y + 1);
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
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int(u + i);
						Do(d,x,n - 1,y + 1);
					}
				}
			}
			break;
		case 4:
			var u = At(a,1), v = At(a,2), w = At(a,3);
			if(Less(u,v))
				while(!Less(v,u))
				{
					d[c] = u;
					Do(d,x,n - 1,y + 1);
					u = Plus(u,w);
				}
			else
				while(!Greater(v,u))
				{
					d[c] = u;
					Do(d,x,n - 1,y + 1);
					u = Plus(u,w);
				}
				break;
		}
		return;
	}
	Eval(Subs(d,x));
}
}//namespace

void Do(Var x, size_t n, const var *y)
{
    dict_t d;
	Do(d,x,n,y);
}
}//namespace mU

using namespace mU;

CAPI CPROC(System_Do)
{
	Do(At(x,0),Size(x) - 1,&At(x,1));
	return Null;
}
CAPI CPROC(System_N)
{
	size_t n = Size(x);
	if(n == 1) return Evalf(At(x,0));
	if(n == 2 && IntQ(At(x,1)))
		return Evalf(At(x,0),Z::UI(At(x,1)));
	return 0;
}
CAPI CPROC(System_IntegerPart)
{
	return IntegerPart(At(x,0));
}
CAPI CPROC(System_Floor)
{
	return Floor(At(x,0));
}
CAPI CPROC(System_Ceiling)
{
	return Ceiling(At(x,0));
}
CAPI CPROC(System_Round)
{
	return Round(At(x,0));
}
CAPI CPROC(System_Expand)
{
	return Expand(At(x,0));
}
CAPI CPROC(System_Power)
{
	if (Size(x) == 1)
	{
		return At(x, 0);
	}
	return Power(At(x,0),At(x,1));
}
CAPI CPROC(System_Mod)
{
	return Mod(At(x,0),At(x,1));
}
CAPI CPROC(System_StringLength)
{
	if(StrQ(At(x,0))) return Int(S::Length(At(x,0)));
	return 0;
}
CAPI CPROC(System_StringInsert)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		mU::uint n = S::Length(c);
		var r = Str();
		CStr(r) = CStr(c);
		if(Size(x) > 2 && StrQ(At(x,1)) && IntQ(At(x,2)))
		{
			int a = Z::SI(At(x,2));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return r;
			S::Insert(r,At(x,1),a);
			return r;
		}
	}
	return 0;
}
CAPI CPROC(System_StringTake)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		mU::uint n = S::Length(c);
		var d = At(x,1);
		if(VecQ(d))
		{
			int a = Z::SI(At(d,0));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return c;
			if(Size(d) > 1)
			{
				int b = Z::SI(At(d,1));
				b < 0 ? b += n : --b;
				if(b < 0 || b >= n)
					return c;
				return S::Substr(c,a,b < a ? 0 : b - a + 1);
			}
			else
				return S::Substr(c,a,1);
		}
		else if(IntQ(d))
		{
			int a = Z::SI(d);
			return a < 0 ? S::Substr(c,a + n,-a) : S::Substr(c,0,a);
		}
	}
	return 0;
}
CAPI CPROC(System_StringDrop)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		mU::uint n = S::Length(c);
		var r = Str();
		CStr(r) = CStr(c);
		var d = At(x,1);
		if(VecQ(d))
		{
			int a = Z::SI(At(d,0));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return r;
			if(Size(d) > 1)
			{
				int b = Z::SI(At(d,1));
				b < 0 ? b += n : --b;
				if(b < 0 || b >= n)
					return r;
				S::Erase(r,a,b < a ? 0 : b - a + 1);
			}
			else
				S::Erase(r,a,1);
		}
		else if(IntQ(d))
		{
			int a = Z::SI(d);
			a < 0 ? S::Erase(r,a + n,-a) : S::Erase(r,0,a);
		}
		return r;
	}
	return 0;
}
CAPI CPROC(System_Defer)
{
	if (Size(x) == 1)
		return At(x, 0);
	return 0;
}
CAPI COPER(System_Function)
{
	var c = At(x,0);
	if(Size(x) == 1)
		return Eval(Supply(Ex(TAG(Function),c),c,y));
	map_t m;
	if(VecQ(c))
	{
		size_t n = Size(c);
		for(size_t i = 0; i < n; ++i)
			m[At(c,i)] = At(y,i);
	}
	else
		m[c] = At(y,0);
	return Eval(Subs(m,At(x,1)));
}
