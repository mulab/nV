#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>
#include <mU/Exceptions.h>
#include <mU/utils.h>
#include "PartSpec.h"

using namespace mU;

namespace
{
	sint get_int(Var expr)
	{
#undef min
#undef max
		const sint minint = std::numeric_limits<mU::sint>::min();
		const sint maxint = std::numeric_limits<mU::sint>::max();
		assert(Type(expr) == TYPE(int));

		mpz_t &mpz = CInt(expr);
		if (mpz_cmp_si(mpz, minint) < 0)
		{
			// TODO: log warning
			return minint;
		}
		if (mpz_cmp_si(mpz, maxint) > 0)
		{
			// TODO: log warning
			return maxint;
		}
		return mpz_get_si(mpz);
	}

	Var Part(Var expr, sint index)
	{
		if (index == 0)	
		{
			return Tag(expr);
		}
		if (AtomQ(expr))
		{
			throw LogicError(L"Part spec depth error");
		}
		assert(Type(expr) == TYPE(vec) || Type(expr) == TYPE(ex));
		Var body = Type(expr) == TYPE(vec) ? expr : Var(Body(expr));
		const sint length = Size(body);
		if (index < -length || index > length)
		{
			throw LogicError(L"Part index out of range");
		}

		return index < 0 ? At(body, length + index) : At(body, index - 1);
	}

	var Part(Var expr, Var index_set, size_t offset)
	{
		if (offset >= Size(index_set))
		{
			return expr;
		}
		Var index = At(index_set, offset);

		switch (Type(index))
		{
		case TYPE(int):
			{
				const sint i = get_int(index);
				return Part(Part(expr, i), index_set, offset+1);
			}
		case TYPE(vec):
			{
				const size_t index_length = Size(index);
				var body = Vec();
				for (size_t i = 0; i < index_length; ++i)
				{
					const sint j = get_int(At(index, i));
					Push(body, Part(Part(expr, j), index_set, offset+1));
				}
				return Eval(Ex(Tag(expr), body));
			}
		case TYPE(sym):
			{
				if (index == SYSTEM_SYM(All))
				{
					if (AtomQ(expr))
						return Eval(Ex(Tag(expr), Vec()));
					assert(Type(expr) == TYPE(ex) || Type(expr) == TYPE(vec));
					var orig_body = Type(expr) == TYPE(ex) ? Body(expr) : var(expr);
					const size_t length = Size(orig_body);
					var body = Vec();
					for (size_t i = 0; i < length; ++i)
					{
						Push(body, Part(At(orig_body, i), index_set, offset+1));
					}
					return Eval(Ex(Tag(expr), body));
				}
				else if (index == SYSTEM_SYM(None))
				{
					return Eval(Ex(Tag(expr), Vec()));
				}
				else
				{
					throw LogicError(L"index spec error");
				}
			}
		default:
			throw LogicError(L"index spec error");
		}
	}
}

CPROC_INT2(System_Part, 2, UINT_MAX)
{
	var expr = At(x,0);

	return Part(expr, x, 1);
}

CAPI CPROC(System_Extract)
{
	var expr = At(x, 0);
	var lists = At(x, 1);
	size_t len = Size(lists);
	if (len > 0 && !VecQ(At(lists, 0)))
	{
		lists = Vec(lists);
		len = 1;
	}
	var ret = Vec(len);
	for (size_t k = 0; k < len; ++k)
	{
		var c = expr;
		var list = At(lists, k);
		if (!VecQ(list)) list = Vec(list);
		size_t n = Size(list);
		for(size_t i = 0; i < n; ++i)
		{
			int j = Z::SI(At(list, i));
			if(j == 0)
			{
				c = Tag(c);
				break;
			}
			if(ExQ(c)) c = Body(c);
			if(VecQ(c))
			{
				j < 0 ? j += Size(c) : --j;
				if(j < 0 || j >= Size(c))
					break;
				c = At(c, j);
			}
		}
		At(ret, k) = c;
	}
	if (len == 1) ret = At(ret, 0);
	return ret;
}

CAPI CPROC_INT(System_First, 1)
{
	var arg = At(x, 0);
	if (AtomQ(arg))
	{
		throw LogicError(L"First expects its 1st argument to be non-atomic");
	}
	switch (Type(arg))
	{
	case TYPE(vec):
		if (Size(arg) > 0)
			return At(arg, 0);
		break;
	case TYPE(ex):
		if (Size(Body(arg)) > 0)
			return At(Body(arg), 0);
		break;
	default:
		assert(false);
	}
	return 0;
}

CAPI CPROC_INT(System_Last, 1)
{
	var arg = At(x, 0);
	if (AtomQ(arg))
	{
		throw LogicError(L"Last expects its 1st argument to be non-atomic");
	}
	switch (Type(arg))
	{
	case TYPE(vec):
		return At(arg, Size(arg)-1);
	case TYPE(ex):
		return At(Body(arg), Size(Body(arg))-1);
	default:
		assert(false);
		return Null;
	}
}

CAPI CPROC_INT(System_Rest, 1)
{
	var arg = At(x, 0);
	if (AtomQ(arg))
	{
		throw LogicError(L"Rest expects its 1st argument to be non-atomic");
	}
	var vec;
	switch (Type(arg))
	{
	case TYPE(vec):
		vec = arg;
		break;
	case TYPE(ex):
		vec = Body(arg);
		break;
	default:
		assert(false);
		break;
	}
	var rest = Vec();
	for (int i = 1; i < Size(vec); ++i)
	{
		Push(rest, At(vec, i));
	}
	switch (Type(arg))
	{
	case TYPE(vec):
		return rest;
	case TYPE(ex):
		return Ex(Head(arg), rest);
	}
	assert(false);
}

CAPI CPROC_INT(System_Most, 1)
{
	var arg = At(x, 0);
	if (AtomQ(arg))
	{
		throw LogicError(L"Most expects its 1st argument to be non-atomic");
	}
	var vec;
	switch (Type(arg))
	{
	case TYPE(vec):
		vec = arg;
		break;
	case TYPE(ex):
		vec = Body(arg);
		break;
	}
	CVec(vec).pop_back();
	return Eval(arg);
}

namespace
{
var Take(Var x, const SequenceSpec &spec, size_t level)
{
	assert (level < spec.spec_length());

	if (AtomQ(x))
		throw LogicError(L"sequence spec out of range");

	assert(Type(x) == TYPE(vec) || Type(x) == TYPE(ex));
	Var orig_body = Type(x) == TYPE(ex) ? Var(Body(x)) : x;
	var body = Vec();
	
	SequenceSpec::LevelIterator iter(spec, Size(orig_body), level);
	
	while (iter.has_next())
	{
		Var orig_elem = At(orig_body, iter.next() - 1);
		var elem = level + 1 >= spec.spec_length() ? var(orig_elem) : Take(orig_elem, spec, level + 1);
		Push(body, elem);
	}

	return Type(x) == TYPE(ex) ? Ex(Head(x), body) : body;
}

var Drop(Var x, const SequenceSpec &spec, size_t level)
{
	assert (level < spec.spec_length());

	if (AtomQ(x))
		throw LogicError(L"sequence spec out of range");

	assert(Type(x) == TYPE(vec) || Type(x) == TYPE(ex));
	Var orig_body = Type(x) == TYPE(ex) ? Var(Body(x)) : x;
	var body = Vec();
	const size_t orig_length = Size(orig_body);

	SequenceSpec::LevelIterator iter(spec, orig_length, level);
	stdext::hash_set<size_t> dropped;
	
	while (iter.has_next())
	{
		dropped.insert(iter.next());
	}

	for (size_t i = 0; i < orig_length; ++i)
	{
		if (dropped.count(i+1))
			continue;

		Var orig_elem = At(orig_body, i);
		var elem = level + 1 >= spec.spec_length() ? var(orig_elem) : Drop(orig_elem, spec, level + 1);
		Push(body, elem);
	}

	return Type(x) == TYPE(ex) ? Ex(Head(x), body) : body;
}
}

CPROC_INT2(System_Take, 2, UINT_MAX)
{
	SequenceSpec spec(x, 1);
	return Eval(Take(At(x,0), spec, 0));
}

CPROC_INT2(System_Drop, 2, UINT_MAX)
{
	SequenceSpec spec(x, 1);
	return Eval(Drop(At(x, 0), spec, 0));
}
