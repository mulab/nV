#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_SameQ)
{
	return Same(At(x,0),At(x,1)) ? True : False;
}

namespace mU
{
	boost::logic::tribool NumericLess(Var lhs, Var rhs)
	{
		if (NumericQ(lhs)) 
		{
			if (NumericQ(rhs))
			{
				return F::Cmp(Evalf(lhs), Evalf(rhs)) < 0;
			}
			else if (rhs == TAG(Infinity))
			{
				return true;
			}
			else if (Same(rhs, NInfinity))
			{
				return false;
			}
		}
		else if (lhs == TAG(Infinity))
		{
			if (NumericOrInfQ(rhs))
			{
				return false;
			}
		}
		else if (Same(lhs, NInfinity))
		{
			if (NumericQ(rhs) || rhs == TAG(Infinity))
			{
				return true;
			}
			else if (Same(rhs, NInfinity))
			{
				return false;
			}
		}

		return boost::logic::indeterminate;
	}
}

CAPI CPROC(System_Less)
{
	Var lhs = At(x, 0);
	Var rhs = At(x, 1);

	boost::logic::tribool r = NumericLess(lhs, rhs);

	if (r)
	{
		return True;
	}
	else if (!r)
	{
		return False;
	}
	else
	{
		return 0;
	}
}
CAPI CPROC(System_Equal)
{
	return Equal(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_Greater)
{
	return Greater(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_UnsameQ)
{
	return !Same(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_GreaterEqual)
{
	Var lhs = At(x, 0);
	Var rhs = At(x, 1);

	boost::logic::tribool r = NumericLess(lhs, rhs);

	if (r)
	{
		return False;
	}
	else if (!r)
	{
		return True;
	}
	else
	{
		return 0;
	}
}
CAPI CPROC(System_Unequal)
{
	return !Equal(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_LessEqual)
{
	return !Greater(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_FreeQ)
{
	return FreeQ(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_And)
{
	var r = Vec();
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		var c = Eval(At(x,i));
		if(c == False) return False;
		if(c == True) continue;
		Push(r,c);
	}
	if(Size(r) == 0) return True;
	return 0;
}
CAPI CPROC(System_Or)
{
	var r = Vec();
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		var c = Eval(At(x,i));
		if(c == True) return True;
		if(c == False) continue;
		Push(r,c);
	}
	if(Size(r) == 0) return False;
	return 0;
}
CAPI CPROC(System_Not)
{
	return At(x,0) == True
		? False
		: At(x,0) == False
		? True
		: 0;
}
CPROC_ATOMIC(System_Inequality) {
	var r = Vec();
	int n = Size(x);
	for(int i = 0; i < n - 1; i += 2)
	{
		var c = Eval(Ex(At(x, i + 1), Vec(At(x, i), At(x, i + 2))));
		if(c == False) return False;
		if(c == True) continue;
		Push(r, c);
	}
	if(Size(r) == 0) return True;
	return 0;
}
