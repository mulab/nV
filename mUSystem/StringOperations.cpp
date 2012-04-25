#include "common.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/System.h>
#include <mU/Kernel.h>
#include <mU/Exceptions.h>
#include <mU/utils.h>
#include "PartSpec.h"

using namespace mU;

var StringDrop_uni_str(Var x)
{
	SequenceSpec s(x, 1);
	var c = At(x, 0);
	assert (StrQ(c));

	SequenceSpec::LevelIterator iter(s, S::Length(c), 0);
	var r = Str();
	CStr(r) = CStr(c);
	size_t p=0;
	while (iter.has_next())
	{
		S::Erase(r,iter.next()-(++p),1);
	}
	var d = At(x,1);
	return r;
}

CAPI CPROC(System_StringDrop)
{
	var c = At(x,0);
	if (StrQ(c))
		return StringDrop_uni_str(x);
	else if (VecQ(c))
	{
		size_t i;
		var y = x, r(Vec());
		for (i = 0; i < Size(c); i++)
		{
			At(y,0)=At(c,i);
			Push(r, StringDrop_uni_str(y));	//这条语句是否安全？
		}
		return r;
	}
	return 0;
}

//多条规则
//被替换者为空字符串

bool StringMatch_1(Var s, Var sub, uint n)
{
	assert (StrQ(s) && StrQ(sub));
	return (S::Cmp(s, n, S::Length(sub), sub)==0);
}

var StringReplace_uni_str(Var c, Var rul, uint step)
{
	assert (StrQ(c));
	if (step < 0) step = 0;

	var l = Str(), r = Str(), s = c, v, p;
	uint count = 0;
	if (ExQ(rul, TAG(Rule)))
		v = Vec(rul);
	else
		v = rul;
	if (VecQ(v))
	{
		size_t i; uint j;
		for (j = 0; (j < S::Length(s)) && ((step == 0) || (count < step)); j++)
		{
			for (i = 0; i < Size(v); i++)
			{
				p = Body(At(v,i));
				l = At(p, 0); r = At(p, 1);
				if (StringMatch_1(s, l, j))
				{
					S::Erase(s, j, S::Length(l));
					S::Insert(s, r, j);
					j += (S::Length(r) - 1);
					count++;
					break;
				}
			}
		}
	}
	return s;
}

CPROC_INT2(System_StringReplace, 2, 3)
//CPROC_INT2_OPT(System_StringReplace, 2, 3, (IgnoreCase, False))
{
	var c = At(x, 0);
	uint step = 0;
	//FIXME: Merely simple substring exchanging.
	//FIXME: IgnoreCase unconsidered.
	if (Size(x) == 3)
		if (!IntQ(At(x, 2)))
			throw LogicError(L"the 3rd argument to StringReplace should be an non-negative integer");
		else
			step = Z::UI(At(x, 2));
	if (StrQ(c))
		return StringReplace_uni_str(c, At(x, 1), step);
	else if (VecQ(c))
	{
		size_t i;
		var y = x, r(Vec());
		for (i = 0; i < Size(c); i++)
		{
			Push(r, StringReplace_uni_str(At(c, i), At(x, 1), step));	//这条语句是否安全？
		}
		return r;
	}
	return 0;
}
