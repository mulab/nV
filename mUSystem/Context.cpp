#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Context)
{
	var r;
	if(Size(x))
		r = Context(At(x,0));
	else
		r = Context();
	return Str(ContextName[r]);
}
CAPI CPROC(System_Contexts)
{
	size_t n = CTab(Contexts).size();
	var r = Vec(n);
	tab_t::rep_t::const_iterator
		iter = CTab(Contexts).begin();
	for(size_t i = 0; i < n; ++i)
	{
		At(r,i) = Str(iter->first);
		++iter;
	}
	return r;
}
CAPI CPROC(System_ContextPath)
{
	size_t n = ContextPath.top().size();
	var r = Vec(n);
	std::list<Var>::const_iterator
		iter = ContextPath.top().begin();
	for(size_t i = 0; i < n; ++i)
	{
		At(r,i) = Str(ContextName[*iter]);
		++iter;
	}
	return r;
}
CAPI CPROC(System_Begin)
{
	var r = Ctx2(CStr(At(x,0)).c_str());
	Begin(r);
	return Str(ContextName[r]);
}
CAPI CPROC(System_End)
{
	var r = Context();
	End();
	return Str(ContextName[r]);
}
CAPI CPROC(System_BeginPackage)
{
	var r = Ctx2(CStr(At(x,0)).c_str());
	BeginPackage(r);
	return Null;
}
CAPI CPROC(System_EndPackage)
{
	EndPackage();
	return Null;
}
CAPI CPROC(System_Unevaluated)
{
	return At(x,0);
}

CAPI CPROC(System_Property)
{
	std::map<Var,map_t>::const_iterator
		iter = Properties.find(At(x,0));
	if(iter != Properties.end())
	{
		map_t::const_iterator
			iter2 = iter->second.find(At(x,1));
		if(iter2 != iter->second.end())
			return iter2->second;
	}
	return 0;
}
CAPI CPROC(System_ToString)
{
	wostringstream t;
	Print(Pretty(At(x,0)),t);
	return Str(t.str());
}
CAPI CPROC(System_ToExpression)
{
	if(StrQ(At(x,0)))
	{
		var r = ParseString(CStr(At(x,0)));
		return r ? r : Nil;
	}
	return 0;
}
CAPI CPROC(System_Exit)
{
	if(Size(x) && IntQ(At(x,0)))
		exit(Z::SI(At(x,0)));
	exit(-1);
	return Null;
}
CAPI CPROC(System_Quit)
{
	exit(-1);
	return Null;
}
CAPI CPROC(System_Set)
{
	Set(At(x,0),At(x,1));
	return At(x,1);
}
CAPI CPROC(System_SetDelayed)
{
	Set(At(x,0),At(x,1));
	return Null;
}

CPROC_INT(System_Increment, 1)
{
	return Increment(At(x, 0));
}

CPROC_INT(System_PreIncrement, 1)
{
	return PreIncrement(At(x, 0));
}

CPROC_INT(System_Decrement, 1)
{
	return Decrement(At(x, 0));
}

CPROC_INT(System_PreDecrement, 1)
{
	return PreDecrement(At(x, 0));
}

CAPI CPROC(System_Hold)
{
	return 0;
}

namespace
{
	var Symbols(const wstring &name)
	{
		tab_t::rep_t::const_iterator i = CTab(Contexts).find(name);
		if (i == CTab(Contexts).end())
		{
			return Null;
		}
		const tab_t::rep_t &context = CTab(i->second);
		var symbols = Vec();
		for (tab_t::rep_t::const_iterator j = context.begin(); j != context.end(); ++j)
		{
			Push(symbols, j->second);
		}

		return symbols;
	}
}

CPROC_INT(System_Symbols, 1)
{
	if (Type(At(x, 0)) != TYPE(str))
	{
		throw LogicError(L"Symbols expects its only argument to be a string");
	}

	return Symbols(CStr(At(x, 0)));
}

namespace
{
	class TraceRuleSymbol
	{
	private:
		std::stack<var> symbols_;
	public:
		TraceRuleSymbol(Var symbols)
		{
			assert(ListQ(symbols));
			const size_t len = Size(symbols);
			for (size_t i = 0; i < len; ++i)
			{
				Var sym = At(symbols, i);
				assert(Type(sym) == TYPE(sym));
				if (TraceRuleSymbols.insert(sym).second)
				{
					symbols_.push(sym);
				}
			}
		}

		~TraceRuleSymbol() 
		{
			while (!symbols_.empty())
			{
				TraceRuleSymbols.erase(symbols_.top());
				symbols_.pop();
			}
		}
	};
}

CPROC_INT(System_TraceRule, 2)
{
	Var expr = At(x, 0);
	Var arg2 = At(x, 1);
	var symbols;

	switch (Type(arg2))
	{
	case TYPE(sym):
		symbols = Vec(arg2);
		break;
	case TYPE(vec):
		{
			const size_t len = Size(arg2);
			for (size_t i = 0; i < len; ++i)
			{
				if (Type(At(arg2, i)) != TYPE(sym))
				{
					throw LogicError(L"the 2nd argument to TraceRule should be a symbol or a list of symbols");
				}
			}
			symbols = arg2;
		}
		break;
	default:
		throw LogicError(L"the 2nd argument to TraceRule should be a symbol or a list of symbols");
	}

	TraceRuleSymbol trs(symbols);
	return Eval(expr);
}
