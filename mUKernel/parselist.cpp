#include "StdAfx.h"
#include <mU/Kernel.h>
#include <mU/Parser.h>
#include <mU/utils.h>

namespace mU {
//////////////////////////////////////
var ParseList(wistream &f)
{
	var r = Vec();
	if(f)
	{
		parser x(f);
		while(x.has_more())
		{
			try {
				x.parse();
			}
			catch (UnexpectedTokenException &)
			{
				wcerr << _W("Syntax::etoken : unexpected token encountered")
					<< _W(" at line ") << x.lineno << _W(", column ") << x.column << std::endl;
				return Failed;
			}
			catch (MoreInputNeededException &)
			{
				wcerr << _W("Syntax::sntxi : Incomplete expression; more input is needed")
					<< _W(" at line ") << x.lineno << _W(", column ") << x.column << std::endl;
				return Failed;
			}
			try {
				Push(r, Eval(Optimi(x.result())));
			}
			catch (...) {
				wcerr << _W("ParseList::eval : at line ") << x.lineno << _W(", column ") << x.column << std::endl;
				throw;
			}
		}
	}
	return r;
}
var Parse(wistream &f) {
	var r = ParseList(f);
	if (r == Failed)
		return r;
	size_t n = Size(r);
	if (n > 0)
		return At(r, n - 1);
	return Null;
}
var ParseFile(const wstring& x) {
	var &notebookDirectory = FactValues[Sym(_W("NotebookDirectory"), System)][Vec()];
	if (x[1] != 
#ifdef _WIN32
		_W(':')
#else
		_W('/')
#endif
	) {

	}
	wstring new_path = x.substr(0, x.find_last_of(_W("\\/"))) + _W('/');
	VarSentry vs(notebookDirectory, Str(new_path));
	wifstream i(to_string(x.c_str(), x.length()).c_str());
	if (!i) {
		wcerr << _W("ParseFile::open : can not open ") << x << endl;
		return Failed;
	}
	if (Verbose >= 1)
		wcerr << _W("Parsing file: ") << x << endl;
	try {
		return ParseList(i);
	}
	catch (...) {
		wcerr << _W("ParseFile::eval : at file ") << x << std::endl;
		//throw;
		return Failed;
	}
}
var Get(const wstring& x) {
	var r = ParseFile(x);
	if (r == Failed)
		return r;
	size_t n = Size(r);
	if (n > 0)
		return At(r, n - 1);
	return Null;
}
var ParseString(const wstring& x) {
	wistringstream i(x);
	try {
		return ParseList(i);
	}
	catch (...) {
		wcerr << _W("ParseString::eval : at string ") << x << std::endl;
		//throw;
		return Failed;
	}
}
var ToExpression(const wstring& x) {
	var r = ParseString(x);
	if (r == Failed)
		return r;
	size_t n = Size(r);
	if (n > 0)
		return At(r, n - 1);
	return Null;
}
var Read(wistream &f, Var x)
{
	wstring t;
	if(x)
	{
		if(SymQ(x))
		{
			if(x == TAG(Integer))
			{
				f >> t;
				return Int(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(Rational))
			{
				f >> t;
				return Rat(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(Real))
			{
				f >> t;
				return Flt(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(String))
			{
				var r = Str();
				std::getline(f,CStr(r));
				return r;
			}
			else if(x == TAG(Symbol))
			{
				f >> t;
				return Sym2(t.c_str());
			}
		}
		else if(StrQ(x) && CStr(x).size() == 1)
		{
			std::getline(f,t,CStr(x)[0]);
			return ParseString(t);
		}
	}
	std::getline(f,t);
	return ParseString(t);
}
//////////////////////////////////////
}
