#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Kernel.h>
#include <mU/Pattern.h>

using namespace mU;

CPROC_INT(System_Abort, 0)
{
	mU::abort();
	return mU::Aborted;
}

CPROC_INT(System_Interrupt, 0)
{
	mU::abort();
	return mU::Aborted;
}

CPROC_INT(System_AbortProtect, 1)
{
	var r;
	{
		AtomicContext ac;
		r = Eval(At(x, 0));
	}
	check_for_abortion();
	return r;
}

CPROC_INT(System_CheckAbort, 2)
{
	try
	{
		return Eval(At(x, 0));
	}
	catch (boost::thread_interrupted &)
	{
		return Eval(At(x, 1));
	}
}

CPROC_INT2(System_Throw, 1, 2)
{
	switch (Size(x))
	{
	case 1:
		throw UserException(At(x, 0));
	case 2:
		throw UserException(At(x, 0), At(x, 1));
	default:
		assert(false);
	}
}

CPROC_INT2(System_Catch, 1, 3)
{
	try
	{
		return Eval(At(x, 0));
	}
	catch (UserException &e)
	{
		if (Size(x) > 1)
		{
			if (e.tag() == 0)
				throw e;
			var form = At(x, 1);
			map_t m;
			bool caught = FixQ(form) ? Same(form, e.tag()) : MatchQ(m, Pat(form), e.tag());
			if (caught)
			{
				if (Size(x) == 2)
				{
					return e.value();
				}
				else
				{
					return Eval(Ex(At(x, 2), Vec(e.value(), e.tag())));
				}
			}
			else
			{
				throw e;
			}
		}
		else
		{
			return e.value();
		}
	}
}

namespace
{
	struct reap_list
	{
		// FIXME: do we really need this pattern_fixed field?
		bool pattern_fixed;
		var pattern;
		std::vector<std::pair<var, var> > tag_map;

		reap_list();
		reap_list(var pattern);

		bool match(Var tag);
	};

	reap_list::reap_list() : pattern_fixed(false), pattern(catch_all_pattern)
	{}

	reap_list::reap_list(var pat) : pattern_fixed(FixQ(pat))
	{
		if (pattern_fixed)
		{
			pattern = pat;
		}
		else
		{
			pattern = Pat(pat);
		}
	}

	bool reap_list::match(Var tag)
	{
		if (tag == 0)
		{
			// FIXME: how do we say "any catch-all pattern"?
			return Same(pattern, catch_all_pattern);
		}
		map_t m;
		return pattern_fixed ? Same(pattern, tag) : MatchQ(m, pattern, tag);
	}

	// TODO: is it good to embed vector in vector?
	typedef std::vector<reap_list> reap_level_t;
	std::vector<reap_level_t> ReapStack;

	var reap_list_result(const reap_list &list, Var f)
	{
		var result = Vec();
		for (std::vector<std::pair<var, var> >::const_iterator i = list.tag_map.begin(); 
				i != list.tag_map.end(); ++i)
		{
			if (f != 0)
			{
				Push(result, Ex(f, Vec(i->first, i->second)));
			}
			else
			{
				Push(result, i->second);
			}
		}

		return result;
	}

	class NewLevelInReapStack
	{
	public:
		NewLevelInReapStack() { ReapStack.push_back(reap_level_t()); }
		~NewLevelInReapStack() { ReapStack.pop_back(); }
	};
}

CPROC_INT2(System_Reap, 1, 3)
{
	NewLevelInReapStack newLevel;
	//NOTE: std::vector<reap_level_t>::reference level = ReapStack.back() may be invalid after ReapStack.resize(...)/.push_back(...)
#define level ReapStack.back()
	switch (Size(x))
	{
	case 1:
		level.push_back(reap_list());
		break;
	case 2:
	case 3:
		if (ListQ(At(x, 1)))
		{
			var pattern_list = At(x, 1);
			const size_t length = Size(pattern_list);
			level.reserve(length);
			for (size_t i = 0; i < length; ++i)
			{
				level.push_back(reap_list(At(pattern_list, i)));
			}
		}
		else
		{
			level.push_back(reap_list(At(x, 1)));
		}
		break;
	default:
		assert(false);
	}
	var result = Vec(Eval(At(x, 0)));
	if (level.size() == 0)
	{
		Push(result, Vec());
		return result;
	}
	var f = Size(x) > 2 ? At(x, 2) : 0;
	assert(level.size() > 0);
	if (level.size() == 1)
	{
		Push(result, reap_list_result(level[0], f));
	}
	else
	{
		var reap_results = Vec();
		CVec(reap_results).reserve(level.size());
		for (std::vector<reap_list>::const_iterator i = level.begin(); i != level.end(); ++i)
		{
			Push(reap_results, reap_list_result(*i, f));
		}
		Push(result, reap_results);
	}
#undef level
	return result;
}

namespace
{
	void sow(Var e, Var tag = 0)
	{
		for (std::vector<reap_level_t>::reverse_iterator i = ReapStack.rbegin(); 
				i != ReapStack.rend(); ++i)
		{
			std::vector<reap_level_t>::reference level = (*i);
			bool level_match = false;
			for (std::vector<reap_list>::iterator j = level.begin();
					j != level.end(); ++j)
			{
				const bool pattern_match = j->match(tag);
				std::vector<std::pair<var, var> > &tag_map = j->tag_map;
				if (pattern_match)
				{
					bool tag_match = false;
					for (std::vector<std::pair<var, var> >::iterator k = tag_map.begin();
							k != tag_map.end(); ++k)
					{
						if (tag == 0 && k->first != 0)
						{
							continue;
						}
						if (tag == 0 || Same(k->first, tag))
						{
							Push(k->second, e);
							tag_match = true;
							break;
						}
					}
					if (!tag_match)
					{
						tag_map.push_back(std::make_pair(tag, Vec()));
						Push(tag_map.rbegin()->second, e);
					}
				}
			}

			if (level_match)
			{
				break;
			}
		}
	}
}

CPROC_INT2(System_Sow, 1, 2)
{
	var e = At(x, 0);
	switch (Size(x))
	{
	case 1:
		sow(e);
		break;
	case 2:
	{
		Var tags = At(x, 1);
		if (ListQ(tags))
		{
			const size_t num_tag = Size(tags);
			for (size_t i = 0; i < num_tag; ++i)
			{
				sow(e, At(tags, i));
			}
		}
		else
		{
			sow(e, tags);
		}
		break;
	}
	default:
		assert(false);
	}
	return e;
}
