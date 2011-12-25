#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_CompoundExpression)
{
	var r;
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		r = Eval(At(x,i));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return) || h == TAG(Break) || h == TAG(Continue))
				break;
		}
	}
	return r;
}
CAPI CPROC(System_Flatten)
{
	var c = At(x,0);
	if(VecQ(c))
	{
		var r = Vec();
		Flatten(r,c);
		return r;
	}
	else if(ExQ(c))
	{
		var r = Vec();
		var h = Head(c);
		var b = Body(c);
		Flatten(r,h,b);
		return Ex(h,r);
	}
	return 0;
}
CAPI CPROC(System_FlattenAll)
{
	var c = At(x,0);
	if(VecQ(c))
	{
		var r = Vec();
		FlattenAll(r,c);
		return r;
	}
	else if(ExQ(c))
	{
		var r = Vec();
		var h = Head(c);
		var b = Body(c);
		FlattenAll(r,h,b);
		return Ex(h,r);
	}
	return 0;
}
CAPI CPROC(System_If)
{
/*
	if (Size(x) >= 2) {
		var r;
		if (At(x,0) == True) {
			r = Eval(At(x,1));
			if (ExQ(r, TAG(Return)))
				return Size(Body(r)) > 1 ? At(Body(r), 1) : Null;
			return r;
		}
		if (Size(x) == 3 && At(x,2) == False) {
			r = Eval(At(x,2));
			if (ExQ(r, TAG(Return)))
				return Size(Body(r)) > 1 ? At(Body(r), 1) : Null;
			return r;
		}
	}
	return 0;
*/
	return At(x,0) == True
		? Eval(At(x,1))
		: At(x,0) == False && Size(x) == 3
		? Eval(At(x,2))
		: Null;
}
CAPI CPROC(System_For)
{
	var r;
	for(Eval(At(x,0));
		Eval(At(x,1)) == True;
		Eval(At(x,2)))
	{
		r = Eval(At(x,3));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return)) return r;
			if(h == TAG(Break)) break;
			if(h == TAG(Continue)) continue;
		}
	}
	return Null;
}
CAPI CPROC(System_While)
{
	var r;
	while(Eval(At(x,0)) == True)
	{
		r = Eval(At(x,1));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return)) return r;
			if(h == TAG(Break)) break;
			if(h == TAG(Continue)) continue;
		}
	}
	return Null;
}
CAPI CPROC(System_With)
{
	var a = At(x,0);
	size_t n = Size(a);
	map_t m;
	for(size_t i = 0; i < n; ++i)
		if(ExQ(At(a,i),TAG(Set)))
			m[Left(At(a,i))] = Eval(Right(At(a,i)));
	var r = Eval(Subs(m,At(x,1)));
	if (ExQ(r)) {
		var h = Head(r);
		var b = Body(r);
		if (h == TAG(Return)) {
			if (Size(b) > 0)
				r = At(b, 0);
		}
		else if (h == TAG(Condition)) {
			if (Eval(At(b, 1)) == True)
				r = Eval(At(b, 0));
		}
	}
	return r;
}
CAPI CPROC(System_Block)
{
	var a = At(x,0);
	size_t n = Size(a);
	std::vector<Var> locals(n);
	std::vector<var> local_values(n);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(a,i);
		if(ExQ(c,TAG(Set)))
		{
			locals[i] = Left(c);
			local_values[i] = Eval(Left(c));
			Set(Left(c),Eval(Right(c)));
		}
		else if(SymQ(At(a,i)))
		{
			locals[i] = c;
			local_values[i] = Eval(c);
		}
	}
	var r = Eval(At(x,1));
	if (ExQ(r)) {
		var h = Head(r);
		var b = Body(r);
		if (h == TAG(Return)) {
			if (Size(b) > 0)
				r = At(b, 0);
		}
		else if (h == TAG(Condition)) {
			if (Eval(At(b, 1)) == True)
				r = Eval(At(b, 0));
		}
	}
	for(size_t i = 0; i < n; ++i)
		Set(locals[i],local_values[i]);
	return r;
}
CAPI CPROC(System_Module)
{
	wstring s = Unique();
	var a = At(x,0);
	size_t n = Size(a);
	map_t m;
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(a,i);
		if(ExQ(c,TAG(Set))) {
			var t = Sym(Name(Left(c)) + s);
			m[Left(c)] = t;
			Set(t,Eval(Right(c)));
		} else if(SymQ(At(a,i))) {
			var t = Sym(Name(c) + s);
			m[c] = t;
		}
	}
	var r = Eval(Subs(m,At(x,1)));
	if (ExQ(r)) {
		var h = Head(r);
		var b = Body(r);
		if (h == TAG(Return)) {
			if (Size(b) > 0)
				r = At(b, 0);
		}
		else if (h == TAG(Condition)) {
			if (Eval(At(b, 1)) == True)
				r = Eval(At(b, 0));
		}
	}
	map_t::const_iterator
		iter = m.begin();
	while(iter != m.end())
	{
		Unset(iter->second);
		++iter;
	}
	return r;
}
CPROC_INT(System_Compare, 2)
{
	return Int((sint)Compare(At(x,0),At(x,1)));
}
CPROC_INT(System_Order, 2)
{
	return Int((sint)-Order(At(x,0),At(x,1)));
}
