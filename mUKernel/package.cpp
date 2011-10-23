#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>
#include <mU/utils.h>

namespace mU {
//////////////////////////////////////
void BeginPackage(Var x)
{
	Begin(x);
	ContextPath.push(std::list<Var>());
	ContextPath.top().push_back(x);
	ContextPath.top().push_back(System);
}
void EndPackage()
{
	ContextPath.pop();
	std::list<Var>::iterator iter =
		std::find(
		ContextPath.top().begin(),
		ContextPath.top().end(),Context());
	if(iter != ContextPath.top().end())
		ContextPath.top().erase(iter);
	ContextPath.top().push_front(Context());
	End();
}
var Ctx(const wstring &x)
{
	std::pair<tab_t::rep_t::iterator,bool> iter =
		CTab(Contexts).insert(
		tab_t::rep_t::value_type(x,var()));
	if(iter.second)
	{
		ContextName[iter.first->second = Tab()]
		= iter.first->first.c_str();
	}
	return iter.first->second;
}
var Sym(const wstring &x)
{
	std::pair<tab_t::rep_t::iterator,bool> curr =
		CTab(Context()).insert(
		tab_t::rep_t::value_type(x,var()));
	if(!curr.second)
		return curr.first->second;
	for(std::list<Var>::const_iterator
		iter = ContextPath.top().begin();
		iter != ContextPath.top().end(); ++iter)
	{
		if(*iter == Context()) continue;
		tab_t::rep_t::const_iterator iter2 =
			CTab(*iter).find(x);
		if(iter2 != CTab(*iter).end())
		{
			CTab(Context()).erase(curr.first);
			return iter2->second;
		}
	}
	return curr.first->second
		= new sym_t(curr.first->first.c_str(),Context());
}
var Ctx2(const wchar *x)
{
	if(x[0] == L'`')
	{
		wstring t = ContextName[Context()];
		t.append(++x);
		return Ctx(t);
	}
	return Ctx(x);
}
var Sym2(const wchar *x)
{
	const wchar *c = wcsrchr(x,L'`');
	if(c)
	{
		wstring t;
		if(x[0] == L'`')
		{
			t.append(ContextName[Context()]);
			++x;
		}
		t.append(x,++c);
		return Sym(c,Ctx(t));
	}
	return Sym(x);
}
//////////////////////////////////////
}
