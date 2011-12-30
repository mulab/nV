#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Number.h>
#include <mU/Pattern.h>
#include <mU/System.h>
#include "PartSpec.h"

using namespace mU;

namespace
{
#undef min
#undef max
const int minint = std::numeric_limits<int>::min();
const int maxint = std::numeric_limits<int>::max();
const var neginf = NInfinity;
}

int mU::LevelSpec::get_int(Var e, Var spec)
{
	switch (Type(e))
	{
	case TYPE(int):
	{
		mpz_t &mpz = CInt(e);
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
	case TYPE(sym):
	{
		if (e == TAG(Infinity))
		{
			return maxint;
		}
		else
		{
			throw IllFormedLevelSpecError(spec);
		}
	}
	case TYPE(ex):
	{
		if (Same(e, neginf))
		{
			return minint;
		}
		else
		{
			throw IllFormedLevelSpecError(spec);
		}
	}
	default:
		throw IllFormedLevelSpecError(spec);
	}
	return 0;
}

void mU::LevelSpec::init(Var spec)
{
	assert(spec);
	min_level = 0; 
	max_level = maxint;
	min_depth = 0;
	max_depth = maxint;
	if (ListQ(spec))
	{
		switch (Size(spec))
		{
		case 1:
		{
			int value = get_int(At(spec, 0), spec);
			if (value >= 0)
			{
				min_level = value;
				max_level = value;
			}
			else
			{
				min_depth = -value;
				max_depth = -value;
			}
			break;
		}
		case 2:
		{
			int value1 = get_int(At(spec, 0), spec);
			int value2 = get_int(At(spec, 1), spec);
			if (value1 >= 0)
			{
				min_level = value1;
			}
			else
			{
				max_depth = -value1;
			}
			if (value2 >= 0)
			{
				max_level = value2;
			}
			else
			{
				min_depth = -value2;
			}
			break;
		}
		default:
			throw IllFormedLevelSpecError(spec);
		}
	}
	else
	{
		min_level = 1;
		int value = get_int(spec, spec);
		if (value >= 0)
		{
			max_level = value;
		}
		else
		{
			min_depth = -value;
		}
	}
}

mU::LevelSpec::LevelSpec(int level)
{
	min_level = level;
	max_level = level;
	min_depth = 0;
	max_depth = maxint;
}

mU::LevelSpec::LevelSpec(Var spec)
{
	init(spec);
}

bool mU::LevelSpec::filter(size_t level, size_t depth) const
{
	return level >= min_level && level <= max_level 
			&& depth >= min_depth && depth <= max_depth;
}

mU::SequenceSpec::SequenceSpec(Var spec, size_t start)
{
	assert(Type(spec) == TYPE(vec));
	const size_t length = Size(spec);
	assert(start < length);
	spec_length_ = length - start;
	spec_.reset(new var[spec_length_]);

	for (size_t i = 0; i < spec_length_; ++i)
	{
		spec_[i] = At(spec, start+i);
	}
}

namespace 
{
	mU::sint get_int(Var e, size_t dim, bool convert = true)
	{
		assert(Type(e) == TYPE(int));
		mpz_t &mpz = CInt(e);
		if (mpz_cmp_si(mpz, -dim) < 0)
		{
			throw LogicError(L"sequence spec out of range");
		}
		if (mpz_cmp_si(mpz, dim) > 0)
		{
			throw LogicError(L"sequence spec out of range");
		}
		const sint v = mpz_get_si(mpz);
		if (convert)
			return v >= 0 ? v : dim + v + 1;
		else
			return v;
	}
}

mU::SequenceSpec::LevelIterator::LevelIterator(const SequenceSpec &spec, size_t dim, size_t level) 
{
	assert(level < spec.spec_length_);
	var level_spec = spec.spec_[level];
	switch (Type(level_spec))
	{
	case TYPE(int):
	{
		step_ = 1;
		sint i = get_int(level_spec, dim, false);
		if (i > 0)
		{
			min_ = current_ = 1;
			max_ = i;
		}
		else if (i < 0)
		{
			min_ = current_ = dim + i + 1;
			max_ = dim;
		}
		else
		{
			throw LogicError(L"sequence spec error!");
		}
		break;
	}
	case TYPE(vec):
	{
		switch (Size(level_spec))
		{
		case 2:
			min_ = current_ = get_int(At(level_spec, 0), dim);
			max_ = get_int(At(level_spec, 1), dim);
			step_ = 1;
			break;
		case 3:
			min_ = get_int(At(level_spec, 0), dim);
			max_ = get_int(At(level_spec, 1), dim);
			// FIXME: use another generic get_int for step_
			step_ = get_int(At(level_spec, 2), maxint, false);
			if (step_ == 0)
			{
				throw LogicError(L"sequence spec error!");
			}
			else if (step_ > 0)
			{
				current_ = min_;
			}
			else
			{
				std::swap(min_, max_);
				current_ = max_;
			}
			break;
		default:
			throw LogicError(L"sequence spec error!");
		}
		break;
	}
	case TYPE(sym):
		if (level_spec == SYSTEM_SYM(All))
		{
			min_ = current_ = 1;
			step_ = 1;
			max_ = dim;
		}
		else if (level_spec == SYSTEM_SYM(None))
		{
			min_ = current_ = 2;
			max_ = 1;
			step_ = 1;
		}
		else
		{
			throw LogicError(L"sequence spec error!");
		}
		break;
	default:
		throw LogicError(L"sequence spec error!");
	}
}

bool mU::SequenceSpec::LevelIterator::has_next() const
{
	return step_ > 0 ? current_ <= max_ : current_ >= min_;
}

size_t mU::SequenceSpec::LevelIterator::next()
{
	assert(current_ >= 0 && has_next());
	const size_t v = current_;
	current_ += step_;

	return v;
}

uint mU::Depth(Var x)
{
	assert(x != 0);
	if (x == 0)
	{
		return 0;
	}
	if (AtomQ(x))
	{
		return 1;
	}
	else
	{
		Var vec;
		uint result = 2;
		switch (Type(x))
		{
		case TYPE(vec):
			vec = x;
			break;
		case TYPE(ex):
			vec = Body(x);
			break;
		default:
			assert(false);
			vec = Vec();
		}
		const int length = Size(vec);
		for (int i = 0; i < length; ++i)
		{
			const uint curr = Depth(At(vec, i)) + 1;
			if (curr > result)
			{
				result = curr;
			}
		}

		return result;
	}
}

namespace
{
	class ScanCallback
	{
	private:
		Var f_;
	public:
		ScanCallback(Var f) : f_(f) {}

		void operator()(Var exp) { Eval(Ex(f_, Vec(exp))); }
	};

	class LevelCallback
	{
	private:
		Var vec_;
	public:
		LevelCallback(Var vec) : vec_(vec) { assert(Type(vec) == TYPE(vec)); }

		void operator()(Var exp) { Push(vec_, exp); }
	};

	class CountCallback
	{
	private:
		size_t counter_;
		var pattern_;
		bool is_fixed_;

		bool match(Var exp)
		{
			map_t m;
			return is_fixed_ ? Same(exp, pattern_) : MatchQ(m, pattern_, exp);
		}
	public:
		CountCallback(Var pattern) : counter_(0), is_fixed_(FixQ(pattern)) 
		{
			pattern_ = is_fixed_ ? var(pattern) : Pat(pattern);
		}

		void operator()(Var exp) 
		{
			if (match(exp)) ++counter_; 
		}
		size_t get() { return counter_; }
	};

	template<class Callback>
	uint Scan(Callback &f, Var exp, uint level, LevelSpec &spec, bool heads)
	{
		assert(exp != 0);
		uint depth;
		if (exp == 0)
		{
			return 0;
		}
		if (AtomQ(exp))
		{
			depth = 1;
		}
		else
		{
			Var vec;
			depth = 2;
			switch (Type(exp))
			{
			case TYPE(vec):
				if (heads)
				{
					Scan(f, TAG(List), level + 1, spec, heads);
				}
				vec = exp;
				break;
			case TYPE(ex):
				if (heads)
				{
					Scan(f, Head(exp), level + 1, spec, heads);
				}
				vec = Body(exp);
				break;
			default:
				assert(false);
				vec = Vec();
			}
			const int length = Size(vec);
			for (int i = 0; i < length; ++i)
			{
				const uint curr = Scan(f, At(vec, i), level+1, spec, heads) + 1;
				if (curr > depth)
				{
					depth = curr;
				}
			}
		}

		if (spec.filter(level, depth))
		{
			f(exp);
		}

		return depth;
	}
}

CAPI CPROC(System_FullForm)
{
	wostringstream t;
	FullPrint(At(x,0),t);
	return Str(t.str());
}

CAPI CPROC(System_Head)
{
	return Tag(At(x,0));
}

CAPI CPROC(System_Length)
{
	var c = At(x,0);
	size_t n = 0;
	if(VecQ(c))
		n = Size(c);
	else if(ExQ(c))
		n = Size(Body(c));
	return Int((mU::uint)n);
}

mU::uint LeafCount(Var expr)
{
	if (AtomQ(expr))
	{
		return 1;
	}
	else if (ListQ(expr))
	{
		uint total = 0;
		for (int i = 0; i < Size(expr); ++i)
		{
			total += LeafCount(At(expr, i));
		}
		return total;
	}
	else
	{
		assert(Type(expr) == TYPE(ex));
		return LeafCount(Head(expr)) + LeafCount(Body(expr));
	}
}

CPROC_INT(System_LeafCount, 1)
{
	var expr = At(x, 0);
	return Int(LeafCount(expr));
}

CPROC_INT(System_Depth, 1)
{
	return Int(mU::Depth(At(x, 0)));
}

CPROC_INT2_OPT(System_Scan, 2, 3, (Heads, False))
{
	LevelSpec spec;
	if (argnum == 3)
	{
		spec = LevelSpec(At(x, 2));
	}

	const bool heads = fetch_tribool_option(opt, Heads);

	ScanCallback f(At(x, 0));
	Scan(f, At(x, 1), 0, spec, heads);

	return Null;
}

CPROC_INT2_OPT(System_Level, 2, 3, (Heads, False))
{
	LevelSpec spec(At(x, 1));
	const bool heads = fetch_tribool_option(opt, Heads);
	var list;

	list = Vec();
	LevelCallback f(list);
	Scan(f, At(x, 0), 0, spec, heads);
		
	if (argnum == 3)
	{
		return Eval(Ex(At(x, 2), list));
	}
	else
	{
		return list;
	}
}

CPROC_INT2_OPT(System_Count, 2, 3, (Heads, False))
{
	LevelSpec spec;
	if (argnum == 3)
	{
		spec = LevelSpec(At(x, 2));
	}
	const bool heads = fetch_tribool_option(opt, Heads);

	CountCallback cc(At(x, 1));
	Scan(cc, At(x, 0), 0, spec, heads);

	return Int(static_cast<mU::uint>(cc.get()));
}
