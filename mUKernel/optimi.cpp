#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Optimi(Var x)
{
	switch(Type(x))
	{
	case TYPE(obj):
	case TYPE(int):
	case TYPE(rat):
	case TYPE(flt):
	case TYPE(str):
	case TYPE(sym):
		return x;
	case TYPE(vec):
		{
			size_t n = Size(x);
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Optimi(At(x,i));
			return r;
		}
		break;
	}
	var h = Optimi(Head(x));
	var b = Optimi(Body(x));
	size_t n = Size(b);
	if(SymQ(h))
	{
		std::map<Var,attr_t>::const_iterator
			iter = Attributes.find(h);
		if(iter != Attributes.end())
		{
			if(n == 1 && iter->second.count(OneIdentity))
				return At(b,0);
			if(iter->second.count(Flat))
			{
				var t = Vec();
				Reserve(t,n);
				Flatten(t,h,b);
				b = t;
			}
			if(iter->second.count(Orderless))
				Sort(b);
		}
		if(n == 1)
		{
			var c = At(b,0);
			if(h == TAG(Minus))
			{
				if(IntQ(c))
				{
					var r = Int();
					Z::Neg(r,c);
					return r;
				}
				else if(FltQ(b))
				{
					var r = Flt();
					F::Neg(r,c);
					return r;
				}
				return Ex(TAG(Times),Vec(Int(-1L),c));
			}
			else if(h == TAG(Differential))
				return Ex(Ex(TAG(Derivative),Vec(Int(1L))),Vec(c));
		}
		else if(n == 2)
		{
			var c = At(b,0), d = At(b,1);
			if(h == TAG(Divide))
				return Ex(TAG(Times),Vec(c,Ex(TAG(Power),Vec(d,Int(-1L)))));
			else if(h == TAG(Postfix))
				return Ex(d,Vec(c));
			else if(h == TAG(Prefix))
				return Ex(c,Vec(d));
		}
	}
	return Ex(h,b);
}
//////////////////////////////////////
}
