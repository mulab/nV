#include "StdAfx.h"
#include <mU/Exceptions.h>
#include <mU/Kernel.h>
#include <mU/Number.h>
#include <mU/Pattern.h>
#include <mU/System.h>
#include "PartSpec.h"
//#include <crtdbg.h>

using namespace mU;

CAPI CPROC_INT(System_Prepend, 2)
{
	var expr = At(x, 0);
	var elem = At(x, 1);
	var rvec = Vec(elem);
	var vec;

	if (AtomQ(expr))
	{
		throw LogicError(L"Prepend expects its 1st argument to be non-atomic");
	}

	switch (Type(expr))
	{
	case TYPE(vec):
		vec = expr;
		break;
	case TYPE(ex):
		vec = Body(expr);
		break;
	default:
		assert(false);
		break;
	}
	CVec(rvec).insert(CVec(rvec).end(), CVec(vec).begin(), CVec(vec).end());

	switch (Type(expr))
	{
	case TYPE(vec):
		return rvec;
	case TYPE(ex):
		return Eval(Ex(Head(expr), rvec));
	default:
		assert(false);
		return expr;
	}
}

CAPI CPROC_INT(System_Append, 2)
{
	var expr = At(x, 0);
	var elem = At(x, 1);
	var vec;

	if (AtomQ(expr))
	{
		throw LogicError(L"Append expects its 1st argument to be non-atomic");
	}

	switch (Type(expr))
	{
	case TYPE(vec):
		vec = expr;
		break;
	case TYPE(ex):
		vec = Body(expr);
		break;
	default:
		assert(false);
		break;
	}
	Push(vec, elem);

	switch (Type(expr))
	{
	case TYPE(vec):
		return vec;
	case TYPE(ex):
		return Eval(Ex(Head(expr), vec));
	default:
		assert(false);
		return expr;
	}
}

bool ReverseAtLevels(var &x, const vec_t::rep_t &l, int idx, int cL)
{
	while (idx < l.size() && Z::SI(l[idx]) < cL) idx++;
	if (idx == l.size()) return false;
	bool reversed = false;
	if (Z::SI(l[idx]) == cL)
	{
		if (VecQ(x))
		{
			std::reverse(CVec(x).begin(), CVec(x).end());
			reversed = true;
		}
		else if (ExQ(x))
		{
			std::reverse(CVec(Body(x)).begin(), CVec(Body(x)).end());
			reversed = true;
		}
	}
	if (VecQ(x))
		for(size_t i = 0; i != Size(x); i++)
			reversed = (ReverseAtLevels(At(x,i), l, idx, cL+1) || reversed);
	else if (ExQ(x))
		for(size_t i = 0; i != Len(x); i++)
			reversed = (ReverseAtLevels(Part(x,i), l, idx, cL+1) || reversed);
	return reversed;
}

CAPI CPROC_INT2(System_Reverse, 1, 2)
{
	var expr = At(x,0), list;

	if (AtomQ(expr))
	{
		throw LogicError(L"Reverse expects its 1st argument to be non-atomic");
	}

	if (Size(x) == 1)
		list = Vec(Int(mU::sint(1)));
	else
		list = VecQ(At(x,1)) ? At(x,1) : Vec(At(x,1));
	Sort(list);
	ReverseAtLevels(expr, CVec(list), 0, 1);

	return expr;
}

var Distribute(const var &arg, const var &g, const var &fp)
{
	size_t n = Size(arg);
	std::vector<int> idx = std::vector<int>(n, 0);
	std::vector<int> cnt = std::vector<int>(n, -1);
	for (size_t i = 0; i < n; i++)
		if (Tag(At(arg, i)) == g)
			cnt[i] = VecQ(At(arg, i)) ? Size(At(arg, i)) : Size(Body(At(arg, i)));

	var ret = Vec();
	bool carry;
	do {
		var para = Vec(n);
		for (size_t i = 0; i < n; i++)
		{
			var argi = At(arg, i);
			At(para, i) = cnt[i] == -1 ? argi : (VecQ(argi) ? At(argi, idx[i]) : At(Body(argi), idx[i]));
		}
		Push(ret, Ex(fp, para));

		//increase
		carry = true;
		size_t i = n;
		do {
			i--;
			idx[i]++;
			if (cnt[i] == -1 || idx[i] == cnt[i])
			{
				idx[i] = 0;
				carry = true;
			}
			else carry = false;
		} while(carry && i!=0);
	} while(!carry);
	return ret;
}

CPROC_INT2(System_Distribute, 1, 5)
{
	var expr = At(x, 0);
	var g = TAG(Plus), f = Tag(expr);
	var gp = g, fp = f;
	if (Size(x) > 1)
	{
		gp = g = At(x, 1);
		if (Size(x) > 2)
		{
			if (At(x, 2) != f) return expr;
			fp = f = At(x, 2);
			if (Size(x) > 3)
			{
				gp = At(x, 3);
				fp = At(x, 4);
			}
		}
	}

	var arg;
	if (ExQ(expr)) arg = Body(expr);
	else arg = expr;

	return Ex(gp, Distribute(arg, g, fp));
}

namespace 
{
	typedef std::vector<std::pair<var, std::pair<var, bool> > > replace_rules_t;

	void new_replace_rule(replace_rules_t &rules, var lhs, var rhs)
	{
		if (Type(lhs) != TYPE(vec))
		{
			lhs = Vec(lhs);
		}
		const bool fixed  = FixQ(lhs);
		var lhs_pat = fixed ? lhs : Pat(lhs);
		rules.push_back(std::make_pair(lhs_pat, std::make_pair(rhs, fixed)));
	}

	void new_replace_rule_set(replace_rules_t &rules, var rule_set)
	{
		if (Type(rule_set) != TYPE(ex) || (Head(rule_set) != TAG(Rule) && Head(rule_set) != TAG(RuleDelayed)))
		{
			throw LogicError(L"replace rule expected");
		}
		var body = Body(rule_set);
		assert(Size(body) == 2);
		var lhs = At(body, 0);
		var rhs = At(body, 1);
		
		if (!ListQ(lhs))
		{
			new_replace_rule(rules, lhs, rhs);
			return;
		}
		const size_t length = Size(lhs);
		if (length == 0)
		{
			return;
		}
		if (ListQ(At(lhs, 0)))
		{
			for (size_t i = 0; i < length; ++i)
			{
				new_replace_rule(rules, At(lhs, i), rhs);
			}
		}
		else
		{
			new_replace_rule(rules, lhs, rhs);
		}
	}

	template<class T1, class T2>
	class push_back_entry
	{
	private:
		T1 &container_;
	public:
		push_back_entry(T1 &container, T2 entry) : container_(container) { container_.push_back(entry); }
		~push_back_entry() { container_.pop_back(); }
	};

	bool rule_match_recursive(replace_rules_t::const_iterator i, Var path, Var dimensions, Var search_path, bool only_fixed)
	{
		const size_t at = Size(search_path);

		if (at == Size(path))
		{
			if (i->second.second)
			{
				return Same(i->first, search_path);
			}
			else
			{
				if (only_fixed)
				{
					return false;
				}
				map_t m;
				return MatchQ(m, i->first, search_path);
			}
		}
		else
		{
			var pos = At(path, at);
			var alt_pos = N::Add(N::Neg(N::Add(At(dimensions, at), Int(static_cast<mU::sint>(1)))), At(path, at));
			{
				push_back_entry<vec_t::rep_t, Var> pbe(CVec(search_path), pos);
				if (rule_match_recursive(i, path, dimensions, search_path, only_fixed))
				{
					return true;
				}
			}
			{
				push_back_entry<vec_t::rep_t, Var> pbe(CVec(search_path), alt_pos);
				if (rule_match_recursive(i, path, dimensions, search_path, only_fixed))
				{
					return true;
				}
			}

			return false;
		}
	}

	bool rule_match(replace_rules_t::const_iterator i, Var path, Var dimensions, bool only_fixed)
	{
		var search_path = Vec();

		return rule_match_recursive(i, path, dimensions, search_path, only_fixed);
	}

	var replace_with_rules(var orig, const replace_rules_t &rules, var path, var dimensions, 
							boost::logic::tribool heads, bool only_fixed)
	{
		assert(Type(path) == TYPE(vec));

		// FIXME: to avoid triggering a bug in the pattern matching subsystem, 
		// we will not try to apply the replace rules while path is an empty list,
		if (Size(path) > 0)
		{
			for (replace_rules_t::const_iterator i = rules.begin(); i != rules.end(); ++i)
			{
				if (rule_match(i, path, dimensions, only_fixed))
				{
					// FIXME: we need to use rule replacement here, don't know how
					return i->second.first;
				}
			}
		}

		if (AtomQ(orig))
		{
			return orig;
		}

		assert(Type(orig) == TYPE(vec) || Type(orig) == TYPE(ex));
		var head;
		var body = Vec();
		var orig_body = ListQ(orig) ? orig : Body(orig);
		const size_t length = Size(orig_body);
		push_back_entry<vec_t::rep_t, Var> pbe(CVec(dimensions), Int(static_cast<mU::uint>(length)));

		if (heads)
		{
			push_back_entry<vec_t::rep_t, Var> pbe(CVec(path), Int(static_cast<mU::uint>(0)));
			head = replace_with_rules(Tag(orig), rules, path, dimensions, heads, only_fixed);
		}
		else if (!heads)
		{
			head = Tag(orig);
		}
		else
		{
			push_back_entry<vec_t::rep_t, Var> pbe(CVec(path), Int(static_cast<mU::uint>(0)));
			head = replace_with_rules(Tag(orig), rules, path, dimensions, heads, true);
		}

		for (size_t i = 0; i < length; ++i)
		{
			var elem;
			{
				push_back_entry<vec_t::rep_t, Var> pbe(CVec(path), Int(static_cast<mU::uint>(i+1)));
				elem = replace_with_rules(At(orig_body, i), rules, path, dimensions, heads, only_fixed);
			}
			Push(body, elem);
		}

		return head == TAG(List) ? Eval(body) : Eval(Ex(head, body));
	}
}

CPROC_INT2_OPT(System_ReplacePart, 2, 3, (Heads, Automatic))
{
	const boost::logic::tribool heads = fetch_tribool_option(opt, Heads);
	var exp = At(x, 0);
	var rules_exp = argnum == 2 ? At(x, 1) : Ex(TAG(Rule), Vec(At(x, 2), At(x, 1)));
	replace_rules_t rules;
	if (ListQ(rules_exp))
	{
		const size_t length = Size(rules_exp);
		for (size_t i = 0; i < length; ++i)
		{
			new_replace_rule_set(rules, At(rules_exp, i));
		}
	}
	else
	{
		new_replace_rule_set(rules, rules_exp);
	}

	var path = Vec();
	var dimensions = Vec();

	return replace_with_rules(exp, rules, path, dimensions, heads, false);
}

CPROC_OPT(System_Union, (SameTest, TAG(SameQ)))
{
	if (Size(x) == 0)
	{
		return Vec();
	}
	std::set<var, Less2> elements;
	var test = opt[SameTest];
	if (AtomQ(At(x, 0)))
	{
		throw IllegalArgumentError(L"arguments to Union should be all non-atomic with the same head");
	}
	Var head = Tag(At(x, 0));
	for (size_t i = 0; i < argnum; ++i)
	{
		if (AtomQ(At(x, i)) || !Same(Tag(At(x, i)), head))
		{
			throw IllegalArgumentError(L"arguments to Union should be all non-atomic with the same head");
		}

		Var body = Type(At(x, i)) == TYPE(vec) ? At(x, i) : Body(At(x, i));
		const size_t length = Size(body);

		for (size_t j = 0; j < length; ++j)
		{
			elements.insert(At(body, j));
		}
	}

	var new_body = Vec();
	size_t num = 0;
	for (std::set<var, Less2>::const_iterator i = elements.begin(); i != elements.end(); ++i)
	{
		var next = *i;
		bool duplicate = false;
		for (size_t j = 0; j < num; ++j)
		{
			const boost::logic::tribool same = toTribool(Eval(Ex(test, Vec(next, At(new_body, j)))));
			if (same)
			{
				duplicate = true;
				break;
			}
			else if (same == boost::logic::indeterminate)
			{
				throw LogicError(L"SameTest should yield either True or False");
			}
		}
		if (!duplicate)
		{
			Push(new_body, next);
			++num;
		}
	}

	return Eval(Ex(head, new_body));
}

namespace
{
	class MapCallback
	{
	private:
		var new_head_;
	public:
		MapCallback(var new_head) : new_head_(new_head) {}

		var operator()(Var expr) { return Ex(new_head_, Vec(expr)); }
	};

	class ApplyCallback
	{
	private:
		var new_head_;
	public:
		ApplyCallback(var new_head) : new_head_(new_head) {}

		var operator()(Var expr)
		{
			switch (Type(expr))
			{
			case TYPE(vec):
				return Ex(new_head_, expr);
			case TYPE(ex):
				return Ex(new_head_, Body(expr));
			default:
				return expr;
			}
		}
	};

	template<typename Callback>
	var Map(Callback f, Var exp, uint level, uint &depth, LevelSpec &spec, bool heads)
	{
		assert(exp != 0);
		var result;
		if (exp == 0)
		{
			return 0;
		}
		if (AtomQ(exp))
		{
			depth = 1;
			result = exp;
		}
		else
		{
			assert(Type(exp) == TYPE(ex) || Type(exp) == TYPE(vec));
			uint curr_depth;
			Var orig_body = Type(exp) == TYPE(vec) ? exp : Var(Body(exp));
			var new_head = heads ? Map(f, Tag(exp), level + 1, curr_depth, spec, heads) : Tag(exp);
			var new_body = Vec();
			depth = 2;
			
			const int length = Size(orig_body);
			for (int i = 0; i < length; ++i)
			{
				var curr_elem = Map(f, At(orig_body, i), level+1, curr_depth, spec, heads);
				if (curr_depth > depth)
				{
					depth = curr_depth;
				}
				Push(new_body, curr_elem);
			}

			result = Ex(new_head, new_body);
		}

		if (spec.filter(level, depth))
		{
			result = f(result);
		}

		return result;
	}
}

CPROC_INT2_OPT(System_Map, 2, 3, (Heads, False))
{
	// TODO: 考虑对argnum == 2的情形做优化
	LevelSpec levels;
	if (argnum == 3)
	{
		levels = LevelSpec(At(x, 2));
	}
	const bool heads = fetch_tribool_option(opt, Heads);
	MapCallback f(At(x, 0));

	uint depth;
	return Eval(Map(f, At(x, 1), 0, depth, levels, heads));
}

CPROC_INT2_OPT(System_Apply, 2, 3, (Heads, False))
{
	// TODO: 考虑对argnum == 2的情形做优化
	LevelSpec levels(0);
	if (argnum == 3)
	{
		levels = LevelSpec(At(x, 2));
	}
	const bool heads = fetch_tribool_option(opt, Heads);
	ApplyCallback f(At(x, 0));

	uint depth;
	return Eval(Map(f, At(x, 1), 0, depth, levels, heads));
}
