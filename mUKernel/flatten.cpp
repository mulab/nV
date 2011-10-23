#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/Kernel.h>
#include <mU/Exceptions.h>

namespace mU {
//////////////////////////////////////
void Flatten(Var r, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		if(VecQ(c))
		{
			size_t n = Size(c);
			for(size_t i = 0; i < n; ++i)
				Push(r,At(c,i));
		}
		else
			Push(r,c);
	}
}

void Flatten(Var r, Var h, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		if(ExQ(c,h))
		{
			c = Body(c);
			size_t n = Size(c);
			for(size_t i = 0; i < n; ++i)
				Push(r,At(c,i));
		}
		else
			Push(r,c);
	}
}

void FlattenAll(Var r, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		VecQ(c) ? FlattenAll(r,c) : Push(r,c);
	}
}

void FlattenAll(Var r, Var h, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		ExQ(c,h) ? FlattenAll(r,h,Body(c)) : Push(r,c);
	}
}
var Increment(Var x)
{
	if (!SymQ(x))
	{
		throw LogicError(L"Increment expects its 1st argument to be a symbol");
	}

	var value = Eval(x);
	if (Same(value, x))
	{
		throw LogicError(L"no value for the symbol being incremented");
	}

	var post_value = Plus(value, Int(static_cast<mU::uint>(1)));
	Set(x, post_value);

	return value;
}

var PreIncrement(Var x)
{
	if (!SymQ(x))
	{
		throw LogicError(L"PreIncrement expects its 1st argument to be a symbol");
	}

	var value = Eval(x);
	if (Same(value, x))
	{
		throw LogicError(L"no value for the symbol being incremented");
	}

	var post_value = Plus(value, Int(static_cast<mU::uint>(1)));
	Set(x, post_value);

	return post_value;
}

var Decrement(Var x)
{
	if (!SymQ(x))
	{
		throw LogicError(L"Decrement expects its 1st argument to be a symbol");
	}

	var value = Eval(x);
	if (Same(value, x))
	{
		throw LogicError(L"no value for the symbol being decremented");
	}

	var post_value = Plus(value, Int(static_cast<mU::sint>(-1)));
	Set(x, post_value);

	return value;
}

var PreDecrement(Var x)
{
	if (!SymQ(x))
	{
		throw LogicError(L"PreDecrement expects its 1st argument to be a symbol");
	}

	var value = Eval(x);
	if (Same(value, x))
	{
		throw LogicError(L"no value for the symbol being decremented");
	}

	var post_value = Plus(value, Int(static_cast<mU::sint>(-1)));
	Set(x, post_value);

	return post_value;
}
var Thread(Var h, Var b)
{
	size_t n = Size(b);
	size_t m = 0;
	for(size_t i = 0; i < n; ++i)
#undef max
		if(VecQ(At(b,i))) m = std::max(m,Size(At(b,i)));
	if(m == 0) return 0;
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			var t = At(b,j);
			if(VecQ(t))
#undef min
				At(c,j) = At(t,std::min(i,Size(t) - 1));
			else
				At(c,j) = t;
		}
		At(r,i) = Ex(h,c);
	}
	return r;
}

bool FixQ(Var x)
{
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			if(!FixQ(At(x,i)))
				return false;
		return true;
	}
	if(ExQ(x))
	{
		var h = Head(x);
		if(h == TAG(Pattern) ||
			h == TAG(Blank) ||
			h == TAG(Condition) ||
			h == TAG(Optional) ||
			h == TAG(PatternTest) ||
			h == TAG(BlankSequence) ||
			h == TAG(Alternatives) ||
			h == TAG(Empty))
			return false;
		return FixQ(Body(x));
	}
	return true;
}

wstring Unique()
{
	static size_t count = 0;
	wostringstream t;
	t.imbue(std::locale("C"));
	t << L'$' << ++count;
	return t.str();
}

var Supply(Var w, Var x, Var y)
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
			var r = Vec();
			Reserve(r,n);
			for(size_t i = 0; i < n; ++i)
			{
				var c = At(x,i);
				if(ExQ(c,TAG(SlotSequence)))
				{
					size_t n = Z::UI(Left(c));
					if(n > 0 && n <= Size(y))
					{
						CVec(r).insert(
							CVec(r).end(),
							CVec(y).begin() + n - 1,
							CVec(y).end());
					}
				}
				else
					Push(r,Supply(w,c,y));
			}
			return r;
		}
	}
	var h = Head(x);
	var b = Body(x);
	if(h == TAG(Slot))
	{
		size_t n = Z::UI(At(b,0));
		if(n == 0) return w;
		if(n <= Size(y))
			return At(y,n - 1);
	}
	else if(h == TAG(SlotSequence))
	{
		size_t n = Z::UI(At(b,0));
		if(n > 0 && n <= Size(y))
		{
			var r = Vec();
			CVec(r).assign(
				CVec(y).begin() + n - 1,
				CVec(y).end());
			return r;
		}
	}
	return Ex(Supply(w,h,y),Supply(w,b,y));
}

var GetDownValues(Var x)
{
	var rules = Vec();
	if (SymQ(x))
	{
		std::map<Var,dict_t>::const_iterator
			iter = FactValues.find(x);
		if(iter != FactValues.end())
		{
			dict_t::const_iterator
				iter2 = iter->second.begin();
			if(iter2 != iter->second.end())
			{
				var rule_lhs = Ex(x, iter2->first);
				var rule = Ex(TAG(RuleDelayed),
								Vec(rule_lhs, iter2->second));
				Push(rules, rule);
			}
		}

		std::map<Var,def_t>::const_iterator
			diter = DownValues.find(x);
		if(diter != DownValues.end())
		{
			const def_t &definitions = diter->second;
			if (definitions.is_map()) {
				def_t::map_t::const_iterator
					iter2 = definitions.map->begin();
				while(iter2 != definitions.map->end())
				{
					var rule_lhs = iter2->first;
					var def = iter2->second.second;
					// FIXME: we don't have HoldPattern
					var rule = Ex(TAG(RuleDelayed),
						Vec(rule_lhs, def));
					Push(rules, rule);
					++iter2;
				}
			} else {
				def_t::vec_t::const_iterator
					iter2 = definitions.vec->begin();
				while(iter2 != definitions.vec->end())
				{
					var rule_lhs = iter2->first;
					var def = iter2->second.second;
					// FIXME: we don't have HoldPattern
					var rule = Ex(TAG(RuleDelayed),
						Vec(rule_lhs, def));
					Push(rules, rule);
					++iter2;
				}
			}
			
		}
		return rules;
	}
	else
	{
		// FIXME: more specific exception type
		throw LogicError(L"DownValues only accepts symbol");
	}
}

var GetUpValues(Var x)
{
	var rules = Vec();
	if (SymQ(x))
	{
		// TODO: we don't support UpValues right now
	}
	else
	{
		// FIXME: more specific exception type
		throw LogicError(L"UpValues only accepts symbol");
	}

	return rules;
}

size_t get_arg_options(Var args, options_t &options, size_t start_min)
{
	const size_t length = Size(args);
	size_t start = length;
	for (size_t i = start_min; i < length; ++i)
	{
		Var elem = At(args, i);
		if (Type(elem) == TYPE(ex) && Head(elem) == TAG(Rule))
		{
			Var body = Body(elem);
			if (Size(body) == 2)
			{
				Var rule_lhs = At(body, 0);
				if (Type(rule_lhs) == TYPE(sym) && options.find(rule_lhs) != options.end())
				{
					start = i;
					break;
				}
			}
		}
	}

	for (size_t i = length - 1; i >= start; --i)
	{
		Var elem = At(args, i);
		if (Type(elem) != TYPE(ex) || Head(elem) != TAG(Rule))
		{
			throw OptionsExpectedError(args, i, start);
		}

		Var body = Body(elem);
		if (Size(body) != 2)
		{
			throw OptionsExpectedError(args, i, start);
		}
				
		Var rule_lhs = At(body, 0);
		options_t::iterator iter;
		if (Type(rule_lhs) != TYPE(sym))
		{
			throw OptionsExpectedError(args, i, start);
		}
		if ((iter = options.find(rule_lhs)) == options.end())
		{
			throw UnrecognizedOptionError(args, rule_lhs);
		}
		iter->second = At(body, 1);
	}

	return start;
}

boost::logic::tribool fetch_tribool_option(const options_t &options, Var sym)
{
	options_t::const_iterator i = options.find(sym);
	assert(i != options.end());
	if (i->second == True)
	{
		return true;
	}
	else if (i->second == False)
	{
		return false;
	}
	else if (i->second == Automatic)
	{
		return boost::logic::indeterminate;
	}
	else
	{
		throw BooleanOptionValueError(sym, i->second);
	}
}
//////////////////////////////////////
}
