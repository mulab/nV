#include "StdAfx.h"
#include <mU/Pattern.h>

namespace mU {
//////////////////////////////////////
using namespace Pattern;
namespace
{
	std::stack<Var> FlatHead;
	std::stack<var> OptionalDefault;
}
var Pat(pos_t &x)
{
	if (VecQ(*x))
    {
		if (Size(*x) > 0) {
			pos_t p((Var)*x);
			return new chain(new push(),new chain(p),new pop());
		}
    }
	else if (ExQ(*x))
	{
		var h = Head(*x);
		var b = Body(*x);
		size_t n = Size(b);
		if(h == TAG(Blank))
		{
			Var a = FlatHead.empty() ? 0 : FlatHead.top();
			if(a && (n == 0 || At(b,0) == a))
				return new blank_flat(a,x);
			if(n)
				return new blank_head(At(b,0));
			return new blank();
		}
		else if(h == TAG(BlankSequence))
		{
			if(n)
				return new blanks_head(At(b,0),x);
			return new blanks(x);
		} else if(h == TAG(Pattern) && n == 2)
			return new chain(Pat(At(b,1)), new name(At(b,0)));
		else if(h == TAG(Condition) && n == 2)
			return new chain(Pat(Eval(At(b,0))), new condition(At(b,1)));
		else if(h == TAG(PatternTest) && n == 2)
			return new chain(Pat(At(b,0)), new patterntest(At(b,1)));
		else if (h == TAG(Alternatives))
			return new alternatives(b);
		else if (h == TAG(Optional)) {
			var a = n == 1 ? OptionalDefault.top() : At(b,1);
			if (a == 0)
				a = Null;
			if (ExQ(At(b,0), TAG(Pattern)))
				return new chain(new alternatives(Pat(Right(At(b,0))), new empty(a)), new name(Left(At(b,0))));
			return new alternatives(Pat(At(b,0)), new empty(a));
		} else if (h == TAG(Empty)) {
			var a = n == 0 ? OptionalDefault.top() : At(b,0);
			if (a == 0)
				a = Null;
			return new empty(a);
		}
		return PatEx(h, b);
	}
	return new fix(x);
}
var Pat(Var h, pos_t &x)
{
	OptionalDefault.push(FactValues[TAG(Default)][Vec(h)]);
	std::map<Var,attr_t>::const_iterator
		iter = Attributes.find(h);
	var r;
	if(iter != Attributes.end())
	{
		FlatHead.push(iter->second.count(Flat) ? h : 0);
		if(iter->second.count(Orderless))
			r = new orderless(x);
		else
			r = new chain(x);
	}
	else
	{
		FlatHead.push(0);
		r = new chain(x);
	}
	FlatHead.pop();
	OptionalDefault.pop();
	return r;
}
var PatEx(Var h, Var b) {
	std::map<Var,attr_t>::const_iterator
		iter = Attributes.find(h);
	if(iter != Attributes.end() && iter->second.count(OneIdentity))
		return new chain(new alternatives(new head(h), new oneidentity(h)),Pat(h,b),new pop());
	return new chain(new head(h),Pat(h,b),new pop());
}
bool MemberQ(Var x, Var y)
{
	var z = Pat(y);
	size_t n = Size(x);
	map_t m;
	for(size_t i = 0; i < n; ++i)
		if(MatchQ(m,z,At(x,i)))
			return true;
	return false;
}
var Dispatch(Var x)
{
	return x;
}
var Replace(Var x, Var y, Var z)
{
	map_t m;
	if(MatchQ(m,y,x))
		return Eval(Subs(m,z));
	return x;
}
var Replace(Var x, Var y)
{
	return Replace(x,Pat(Left(y)),Right(y));
}
var ReplaceAll(Var x, Var y, Var z)
{
	map_t m;
	if(MatchQ(m,y,x))
		return Eval(Subs(m,z));
	if(VecQ(x))
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = ReplaceAll(At(x,i),y,z);
		return r;
	}
	if(ExQ(x))
		return Eval(Ex(ReplaceAll(Head(x),y,z),ReplaceAll(Body(x),y,z)));
	return x;
}
var ReplaceAll(Var x, Var y)
{
	if (ExQ(y) && (Head(y) == TAG(Rule) || Head(y) == TAG(RuleDelayed)) && Size(Body(y)) == 2)
		return ReplaceAll(x,Pat(Left(y)),Right(y));
	return x;
}

var catch_all_pattern = Pat(Ex(TAG(Blank), Vec()));
//////////////////////////////////////
}
