#include "common.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/Kernel.h>
#include <mU/Exceptions.h>

namespace mU {
//////////////////////////////////////
namespace {
// TODO: each return statement here should be replaced by a throw statement.
void SetPart(var x_indices, var y)
{
	if(SymQ(At(x_indices,0)))
	{
		const var x = At(x_indices,0);
		map_t::const_iterator iter = OwnValues.find(x);
		if(iter != OwnValues.end())
		{
			var c = iter->second;
			size_t n = Size(x_indices);
			for(size_t i = 1; i < n - 1; ++i)
			{
				int j = Z::SI(Eval(At(x_indices,i)));
				if(j == 0) return;
				if(ExQ(c)) c = Body(c);
				if(VecQ(c))
				{
					j < 0 ? j += Size(c) : --j;
					if(j < 0 || j >= Size(c))
						return;
					c = At(c,j);
				}
			}
			int j = Z::SI(Eval(At(x_indices,n - 1)));
			if(j == 0) return;
			if(ExQ(c)) c = Body(c);
			if(VecQ(c))
			{
				j < 0 ? j += Size(c) : --j;
				if(j < 0 || j >= Size(c))
					return;
				At(c,j) = y;
			}
		}
		else
		{
			// TODO: more specific exception class here
			throw LogicError(L"Part assignment failed!");
		}
	}
	else
	{
		// TODO: we need more specific exception class here.
		throw LogicError(L"first argument to Part should be a symbol!");
	}
}

void SetDownValue(var head, var body, var y)
{
	if(ExQ(y,TAG(Condition))) {
		DownValues[head].insert(
			Ex(TAG(Condition),Vec(Ex(head, body),Right(y))), 
			new Pattern::chain(new Pattern::push(),
								Pat(head,body),
								new Pattern::pop(),
								new Pattern::condition(Right(y))),
								Left(y));
	} else {
		DownValues[head].insert(Ex(head, body),
			new Pattern::chain(
					new Pattern::push(),
					Pat(head,body),
					new Pattern::pop()),
					y);
	}
}

void SetSubValue(var head, var body, var y)
{
	var b = Eval(body);
	var t = Vec(Body(head),b);
	if(ExQ(y,TAG(Condition)))
	{
		SubValues[Head(head)].insert(Ex(TAG(Condition),Vec(Ex(head,body),Right(y))),
						new Pattern::chain(Pat(t),
								new Pattern::condition(Right(y))),
								Left(y));
	}
	else
	{
		SubValues[Head(head)].insert(Ex(head,body),Pat(t),y);
	}
}

void SetConditionDownValue(Var body, Var y) {
	var t = Eval(At(body,0));
	if (ExQ(t)) {
		var h = Head(t);
		if (SymQ(h)) {
			DownValues[h].insert(Ex(TAG(Condition),body),
				new Pattern::chain(new Pattern::push(), 
				Pat(h, Body(t)), 
				new Pattern::pop(),
				new Pattern::condition(At(body,1))), y);
		}
	}
} 

void SetDownValues(Var h, Var y) {
	if (!SymQ(h) || !VecQ(y))
		return;
	def_t& definitions = DownValues[h];
	if (Size(y) == 0) {
		definitions.new_vec();
	}
} 

}

void Set(Var x, Var y)
{
	if(SymQ(x))
	{
		OwnValues[x] = y;
	}
	else if(VecQ(x) && VecQ(y))
	{
		size_t n = Size(x);
		size_t ny = Size(y);
		if (ny < n) {
			for (size_t i = 0; i < ny; ++i)
				Set(At(x,i),At(y,i));
			for (size_t i = ny; i < n; ++i)
				Set(At(x,i),At(y,ny - 1));
		}
		else {
			for (size_t i = 0; i < n; ++i)
				Set(At(x,i),At(y,i));
		}
	}
	else if(ExQ(x))
	{
		var head = Eval(Head(x));
		if(SymQ(head))
		{
			var body = Body(x);
			if(head == TAG(Part))
			{
				SetPart(body, y);
			}
			else if(head == TAG(Property))
			{
				if(SymQ(At(body,0)) && SymQ(At(body,1)))
					Properties[At(body,0)][At(body,1)] = y;
			} else if (head == TAG(Condition)) {
				SetConditionDownValue(body, y);
			} else if (head == TAG(DownValues)) {
				SetDownValues(At(body,0), y);
			}
			else
			{
				body = Eval(body);
				if(FixQ(body))
				{
					FactValues[head][body] = y;
				}
				else
				{
					SetDownValue(head, body, y);
				}
			}
		}
		else if(ExQ(head) && SymQ(Head(head)))
		{
			var body = Body(x);
			SetSubValue(head, body, y);
		}
		else
		{
			// TODO: more specific exception class
			throw LogicError(L"no assignment performed!");
		}
	}
	else
	{
		// TODO: more specific exception class
		throw LogicError(L"no assignment performed!");
	}
}

void Unset(Var x)
{
	if(SymQ(x))
	{
		OwnValues.erase(x);
		return;
	}
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			Unset(At(x,i));
		return;
	}
	if(ExQ(x))
	{
		var h = Eval(Head(x));
		if(SymQ(h))
		{
			var b = Body(x);
			if(h == TAG(Property))
			{
				if(SymQ(At(b,0)) && SymQ(At(b,1)))
				{
					std::map<Var,map_t>::iterator
						iter = Properties.find(At(b,0));
					if(iter != Properties.end())
						iter->second.erase(At(b,1));
				}
				return;
			}
			b = Eval(b);
			if(FixQ(b))
			{
				std::map<Var,dict_t>::iterator
					iter = FactValues.find(h);
				if(iter != FactValues.end())
					iter->second.erase(b);
			}
			else
			{
				std::map<Var,def_t>::iterator
					iter = DownValues.find(h);
				if(iter != DownValues.end())
					iter->second.erase(b);
			}
			return;
		}
		else if(ExQ(h) && SymQ(Head(h)))
		{
			var b = Eval(Body(x));
			var t = Vec(Body(h),b);
			std::map<Var,def_t>::iterator
				iter = SubValues.find(Head(h));
			if(iter != SubValues.end())
					iter->second.erase(t);
			return;
		}
	}
}

void Clear(Var x)
{
	OwnValues.erase(x);
	FactValues.erase(x);
	DownValues.erase(x);
	SubValues.erase(x);
}
//////////////////////////////////////
}
