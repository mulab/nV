/*
 * Exceptions.h
 *
 *  Created on: 2010-1-8
 *      Author: baritono
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <deque>
#include <string>
#include <vector>
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/version.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <errno.h>
#endif
#include <mU/Var.h>

namespace mU
{
#define TAG(x) tag_##x

	// common property tags
	API var TAG(general), TAG(error), TAG(usage), TAG(argx), TAG(argt), TAG(argr), TAG(argrx), TAG(argb), TAG(argbu);
	API var TAG(nonopt), TAG(opttf), TAG(level), TAG(optx), TAG(atomicx);

	API std::stack<var> CprocSymbolStack;

	class Exception
	{
	public:
		Exception();

	};

	class ParserException : public Exception
	{

	};

	class UnexpectedTokenException : public ParserException
	{

	};

	class MoreInputNeededException : public ParserException
	{

	};

	class RuntimeException : public Exception
	{

	};

	class LogicError : public Exception
	{
	public:
		LogicError() {}
		LogicError(const std::wstring &msg)
			: symbol_(CprocSymbolStack.top()), tag_(TAG(error)), args_(Vec(Str(msg))) 
		{
		}
		LogicError(Var symbol, Var tag, Var args) : symbol_(symbol), tag_(tag), args_(args) {}

		var symbol() const { return symbol_; }
		var tag() const { return tag_; }
		var args() const { return args_; }
	private:
		var symbol_;
		var tag_;
		var args_;
	};

	class IllegalArgumentError : public LogicError
	{
	public:
		IllegalArgumentError() {}
		IllegalArgumentError(const std::wstring &msg) : LogicError(msg) {}
	};

	class UnrecognizedOptionError : public LogicError
	{
	public:
		UnrecognizedOptionError(Var args, Var offender)
			: LogicError(CprocSymbolStack.top(), TAG(optx), 
							Vec(offender, Ex(CprocSymbolStack.top(), args)))
		{

		}
	};

	class OptionsExpectedError : public LogicError
	{
	public:
		OptionsExpectedError(Var args, std::size_t offender_index, std::size_t start)
			: LogicError(CprocSymbolStack.top(), TAG(nonopt), 
							Vec(At(args, offender_index), 
								Int(static_cast<mU::uint>(start)),
								Ex(CprocSymbolStack.top(), args)))
		{
		}
	};

	class IllFormedLevelSpecError : public LogicError
	{
	public:
		IllFormedLevelSpecError(Var spec)
			: LogicError(CprocSymbolStack.top(), TAG(level), Vec(spec))
		{

		}
	};

	class BooleanOptionValueError : public LogicError
	{
	public:
		BooleanOptionValueError(Var option, Var value)
			: LogicError(CprocSymbolStack.top(), TAG(opttf), Vec(option, value))
		{

		}
	};

	class UserException : public Exception
	{
	private:
		var value_;
		var tag_;
	public:
		UserException(var value, var tag = 0) : value_(value), tag_(tag) {}

		var value() { return value_; }
		var tag() { return tag_; }
	};

	class IOException : public RuntimeException
	{

	};

	class InvalidCodePointException : public RuntimeException
	{

	};

	class InvalidUtf8StreamException : public InvalidCodePointException
	{

	};

	class UnsupportedCharacterException : public RuntimeException
	{

	};

	class SystemError : public RuntimeException
	{
	public:
		SystemError()
#if (BOOST_VERSION >= 104400)
#ifdef _WIN32
			: error(GetLastError(), boost::system::system_category())
#else
			: error(errno, boost::system::system_category())
#endif
#else
#ifdef _WIN32
			: error(GetLastError(), boost::system::system_category)
#else
			: error(errno, boost::system::system_category)
#endif
#endif
		{}
	private:
		boost::system::error_condition error;
	};

#ifdef _WIN32
#define OS_API_CALL(api_func, ...)			\
	if (!api_func(__VA_ARGS__))			\
		throw SystemError();
#define OS_API_CALL_R(r, api_func, ...)	\
	if (!(r = api_func(__VA_ARGS__)))	\
		throw SystemError();
#else
#define OS_API_CALL(api_func, ...) 		\
	if (api_func(__VA_ARGS__) == -1)	\
		throw SystemError();
#define OS_API_CALL_R(r, api_func, ...)		\
	if ((r = api_func(__VA_ARGS__)) == -1)	\
		throw SystemError();
#endif

	API boost::thread_specific_ptr<bool> abortion_requested;
	API std::stack<var> AtomicSymbolStack;

	class AtomicContext
	{
	private:
		boost::this_thread::disable_interruption boost_disable;
	public:
		AtomicContext() 
		{ 
			if (abortion_requested.get() == 0) abortion_requested.reset(new bool(false));
			AtomicSymbolStack.push(CprocSymbolStack.top());
		}
		~AtomicContext() 
		{
			AtomicSymbolStack.pop();
		}
	};

	API void check_for_abortion();	// throw AbortException
	API void abort(boost::thread *);
	API void abort();

	struct KernelMessage
	{
		var symbol_;
		var tag_;
		var args_;

		KernelMessage(Var symbol, Var tag, const var &args) : symbol_(symbol), tag_(tag), args_(args) {}
		KernelMessage(const LogicError &error) : symbol_(error.symbol()), tag_(error.tag()), args_(error.args()) {}

		wstring format() const;
	};

	API std::deque<std::vector<KernelMessage> > MessageLists;

inline void LogMessage(Var symbol, Var tag, const var &args)
{
	MessageLists.back().push_back(KernelMessage(symbol, tag, args));
}

inline void LogMessage(const LogicError &error)
{
	MessageLists.back().push_back(KernelMessage(error));
}

inline void CLogMessage(Var tag, Var arg)
{
	LogMessage(CprocSymbolStack.top(), tag, Vec(arg));
}

inline void CLogMessage(Var tag, Var arg1, Var arg2)
{
	LogMessage(CprocSymbolStack.top(), tag, Vec(arg1, arg2));
}

inline void CLogMessage(Var tag, Var arg1, Var arg2, Var arg3)
{
	LogMessage(CprocSymbolStack.top(), tag, Vec(arg1, arg2, arg3));
}

inline void CLogMessage(Var tag, Var arg1, Var arg2, Var arg3, Var arg4)
{
	LogMessage(CprocSymbolStack.top(), tag, Vec(arg1, arg2, arg3, arg4));
}

inline Exception::Exception()
{
	if (!boost::this_thread::interruption_enabled() && !AtomicSymbolStack.empty())
	{
		CLogMessage(TAG(atomicx), AtomicSymbolStack.top());
	}
}
}

#endif /* EXCEPTIONS_H_ */
