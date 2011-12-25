#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Kernel.h>

using namespace boost;

namespace mU
{
	boost::thread_specific_ptr<bool> abortion_requested;
	std::stack<var> AtomicSymbolStack;

	void check_for_abortion()
	{
		this_thread::interruption_point();

		if (*abortion_requested)
		{
			*abortion_requested = false;
			throw thread_interrupted();
		}
	}

	void abort(thread *t)
	{
		t->interrupt();
	}

	void abort()
	{
		if (this_thread::interruption_enabled())
		{
			throw thread_interrupted();
		}
		else
		{
			*abortion_requested = true;
		}
	}

	std::deque<std::vector<KernelMessage> > MessageLists;

	namespace
	{
		wstring format_message(Var vformat, Var args)
		{
			assert(Type(args) == TYPE(vec));
			wstring format;
			if (Type(vformat) == TYPE(str))
			{
				format = CStr(vformat);
			}
			else
			{
				wstringstream wss1;
				Print(vformat, wss1);
				format = wss1.str();
			}

			const size_t n_args = Size(args);
			// FIXME: handle insufficient message argument errors later
			for (uint n = 0; n < n_args; ++n)
			{
				wstring pl = L"`" + boost::lexical_cast<wstring>(n+1) + L"`";
				if (boost::algorithm::find_first(format, pl))
				{
					wstringstream wss;
					Print(At(args, n), wss);
					boost::algorithm::replace_all(format, pl, wss.str());
				}
				else
				{
					break;
				}
			}

			return format;
		}
	}

	wstring KernelMessage::format() const
	{
		wstringstream wss;
		Print(symbol_, wss);
		wss << L"::";
		Print(tag_, wss);
		wss << L" : ";
		if (Properties.count(symbol_) && Properties[symbol_].count(tag_))
		{
			wss << format_message(Properties[symbol_][tag_], args_);
		}
		else if (Properties[TAG(General)].count(tag_))
		{
			wss << format_message(Properties[TAG(General)][tag_], args_);
		}
		else
		{
			wss << format_message(Properties[TAG(General)][TAG(general)], args_);
		}

		return wss.str();
	}
}

