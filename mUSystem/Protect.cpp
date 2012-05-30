#include "common.h"
#include <mU/Number.h>
#include <mU/Pattern.h>
#include <mU/Exceptions.h>
#include <mU/System.h>

using namespace mU;

CPROC_INT(System_DownValues, 1)
{
	return GetDownValues(At(x, 0));
}

CPROC_INT(System_UpValues, 1)
{
	return GetUpValues(At(x, 0));
}

CAPI CPROC(System_Protect)
{
	var list = Vec();

	try {
		FlattenAll(list, x);
		for (size_t i = 0; i < Size(list); ++i)
		{
			var elem = At(list, i);
			if (Type(elem) != TYPE(str) && Type(elem) != TYPE(sym))
			{
				throw LogicError(L"Protect expects strings, "
								L"symbols and lists of those.");
			}
		}
		for (size_t i = 0; i < Size(list); ++i)
		{
			var elem = At(list, i);
			switch (Type(elem))
			{
			case TYPE(str):
			{
				var symbol = Sym2(CStr(elem).c_str());
				Attributes[symbol].insert(Protected);
				break;
			}
			case TYPE(sym):
				Attributes[elem].insert(Protected);
				break;
			default:
				assert(false);
				break;
			}
		}

	} catch (Exception &e) {
		// TODO: log the problem
		return Ex(TAG(Protect), x);
	}

	return list;
}

CAPI CPROC(System_Unprotect)
{
	var list = Vec();

	try {
		FlattenAll(list, x);
		for (size_t i = 0; i < Size(list); ++i)
		{
			var elem = At(list, i);
			if (Type(elem) != TYPE(str) && Type(elem) != TYPE(sym))
			{
				throw LogicError(L"Unprotect expects strings, "
								L"symbols and lists of those.");
			}
		}
		for (size_t i = 0; i < Size(list); ++i)
		{
			var elem = At(list, i);
			switch (Type(elem))
			{
			case TYPE(str):
			{
				var symbol = Sym2(CStr(elem).c_str());
				Attributes[symbol].erase(Protected);
				break;
			}
			case TYPE(sym):
				Attributes[elem].erase(Protected);
				break;
			default:
				assert(false);
				break;
			}
		}

	} catch (Exception &e) {
		// TODO: log the problem
		return Ex(TAG(Unprotect), x);
	}

	return list;
}

CPROC_INT2(System_Quiet, 1, 2)
{
	// TODO: fill this out once we've got error/warning message system
	return At(x, 0);
}

CPROC_INT(System_ListQ, 1)
{
	return ListQ(At(x, 0)) ? True : False;
}

namespace
{
	bool HasAttribute(Var symbol, Var attribute)
	{
		std::map<Var,attr_t>::const_iterator iter = Attributes.find(symbol);
		if(iter != Attributes.end())
			return iter->second.count(attribute) > 0;
		else
			return false;
	}
}

namespace mU
{
	bool NumericQ(Var x)
	{
		switch (Type(x))
		{
		case TYPE(int):
		case TYPE(flt):
		case TYPE(rat):
			return true;
		case TYPE(obj):
			return dynamic_cast<obj_t*>(x)->tag() == SYSTEM_SYM(Complex);
		case TYPE(vec):
		case TYPE(str):
			return false;
		case TYPE(ex):
			if (!HasAttribute(Head(x), NumericFunction))
			{
				return false;
			}
			else
			{
				Var body = Body(x);
				const size_t length = Size(body);
				for (size_t i = 0; i < length; ++i)
				{
					if (!NumericQ(At(body, i)))
					{
						return false;
					}
				}
				return true;
			}
		case TYPE(sym):
			return HasAttribute(x, Constant) || x == TAG(I);
		default:
			assert(false);
		}

		return false;
	}

	bool NumericOrInfQ(Var x)
	{
		return NumericQ(x) || x == TAG(Infinity) || Same(x, NInfinity);
	}
}

CAPI CPROC(System_Min)
{
	var list = Vec();
	FlattenAll(list, x);
	const size_t length = Size(list);
	if (length == 0)
	{
		return TAG(Infinity);
	}
	else if (length == 1)
	{
		return At(list, 0);
	}
	var numeric_min = TAG(Infinity);
	bool has_numeric_elems = false;
	std::set<Var, Less2> non_numeric_elems;

	for (size_t i = 0; i < length; ++i)
	{
		Var elem = At(list, i);
		if (NumericOrInfQ(elem))
		{
			if (NumericLess(elem, numeric_min))
			{
				numeric_min = elem;
				has_numeric_elems = true;
			}
		}
		else
		{
			non_numeric_elems.insert(elem);
		}
	}

	if (non_numeric_elems.empty())
	{
		return numeric_min;
	}
	else
	{
		var r = Vec();
		if (has_numeric_elems)
		{
			Push(r, numeric_min);
		}
		for (std::set<Var, Less2>::const_iterator j = non_numeric_elems.begin();
			j != non_numeric_elems.end(); ++j)
		{
			Push(r, *j);
		}

		return Ex(TAG(Min), r);
	}
}

CAPI CPROC(System_Max)
{
	var list = Vec();
	FlattenAll(list, x);
	const size_t length = Size(list);
	if (length == 0)
	{
		return NInfinity;
	}
	else if (length == 1)
	{
		return At(list, 0);
	}
	var numeric_max = NInfinity;
	bool has_numeric_elems = false;
	std::set<Var, Less2> non_numeric_elems;

	for (size_t i = 0; i < length; ++i)
	{
		Var elem = At(list, i);
		if (NumericOrInfQ(elem))
		{
			if (NumericLess(numeric_max, elem))
			{
				numeric_max = elem;
				has_numeric_elems = true;
			}
		}
		else
		{
			non_numeric_elems.insert(elem);
		}
	}

	if (non_numeric_elems.empty())
	{
		return numeric_max;
	}
	else
	{
		var r = Vec();
		if (has_numeric_elems)
		{
			Push(r, numeric_max);
		}
		for (std::set<Var, Less2>::const_iterator j = non_numeric_elems.begin();
			j != non_numeric_elems.end(); ++j)
		{
			Push(r, *j);
		}

		return Ex(TAG(Max), r);
	}
}

CPROC_INT(System_NumericQ, 1)
{
	return NumericQ(At(x, 0)) ? True : False;
}

var Dimensions(Var expr, mU::uint depth, Var parent_head)
{
	assert(depth >= 0);

	if (AtomQ(expr))
	{
		return Vec();
	}
	else
	{
		Var head = Tag(expr);
		if (parent_head && !Same(head, parent_head))
		{
			return Vec();
		}
		Var vec = Type(expr) == TYPE(ex) ?
					Body(expr).get()
					: (assert(Type(expr) == TYPE(vec)), expr);
		var result = Vec(Int(static_cast<mU::sint>(Size(vec))));
		if (Size(vec) == 0 || depth == 1)
		{
			return result;
		}
		var first_result = Dimensions(At(vec, 0),
									depth == 0 ? 0 : depth - 1, head);
		int first_size = Size(first_result);
		for (size_t i = 1; i < Size(vec); ++i)
		{
			var curr_result = Dimensions(At(vec, i),
										depth == 0 ? 0 : depth - 1, head);
			int curr_size = Size(curr_result);

			for (int j = 0; j < first_size && j < curr_size; ++j)
			{
				if (Z::Cmp(At(first_result, j), At(curr_result, j)) != 0)
				{
					first_size = j;
					break;
				}

			}
		}

		for (int j = 0; j < first_size; ++j)
		{
			Push(result, At(first_result, j));
		}

		return result;
	}
}

CPROC_INT2(System_Dimensions, 1, 2)
{
	var expr = At(x, 0);
	mU::uint depth = 0;

	if (Size(x) >= 2)
	{
		var depthv = At(x, 1);
		if (Type(depthv) != TYPE(int) || mpz_sgn(CInt(depthv)) < 0)
		{
			throw LogicError(L"Dimensions expects its 2nd argument "
							L"to be non-negative integer");
		}
#undef max
		if (mpz_cmp_ui(CInt(depthv), std::numeric_limits<mU::uint>::max()) > 0)
		{
			throw LogicError(L"2nd argument to Dimensions is too large");
		}

		depth = mpz_get_ui(CInt(depthv));
		if (depth == 0)
		{
			return Vec();
		}
	}

	return Dimensions(expr, depth, 0);
}
