#pragma once
#include <boost/logic/tribool.hpp>
#include "Kernel.h"

#ifdef _WIN32
#ifdef MUSYSTEM_EXPORTS
#define SYSTEM_API __declspec(dllexport) extern
#else
#define SYSTEM_API __declspec(dllimport) extern
#pragma comment(lib,"mUSystem")
#endif
#else
#define SYSTEM_API extern
#endif

namespace mU {

SYSTEM_API void BoxPrint(Var,wostream& = wcout,size_t = 0);

// 四则运算
SYSTEM_API var Power(Var,Var);
SYSTEM_API var Mod(Var,Var);
SYSTEM_API var Dot(Var);
SYSTEM_API var Dot(Var,Var);

SYSTEM_API void Do(Var,size_t,const var*);
SYSTEM_API var Table(Var,size_t,const var*);
SYSTEM_API var Array(Var,Var,size_t,const var*);
SYSTEM_API var Array(Var,Var,size_t,const var*,const var*);

// 基本数值计算
SYSTEM_API var Evalf(Var);
SYSTEM_API var Evalf(Var,size_t);
SYSTEM_API var IntegerPart(Var);
SYSTEM_API var Floor(Var);
SYSTEM_API var Ceiling(Var);
SYSTEM_API var Round(Var);

// 多项式基本操作
SYSTEM_API var Expand(Var,Var);
SYSTEM_API var Expand(Var);
SYSTEM_API var Variables(Var);
SYSTEM_API var Coefficient(Var,Var);
SYSTEM_API var Coefficient(Var,Var,Var);
SYSTEM_API var Exponent(Var,Var);
SYSTEM_API var Coefficients(Var,Var);
SYSTEM_API var CoefficientList(Var,Var);
SYSTEM_API var FromCoefficients(Var,Var);
SYSTEM_API var FromCoefficientList(Var,Var);

SYSTEM_API uint Depth(Var);

SYSTEM_API bool NumericQ(Var);
SYSTEM_API bool NumericOrInfQ(Var);
SYSTEM_API boost::logic::tribool NumericLess(Var, Var);

// Stream.cpp
SYSTEM_API var Routine(CProc);
SYSTEM_API var Routine(COper);
SYSTEM_API var Invoke(Var,Var);
SYSTEM_API var Invoke(Var,Var,Var);
struct stream_t : public obj_t { wiostream *rep; };
inline wiostream& CStream(Var x) { return *((stream_t*)x)->rep; }
SYSTEM_API var Buffer();
SYSTEM_API var Buffer(const wstring&);
SYSTEM_API var File(const wstring&);
SYSTEM_API var Link(const wstring&,size_t);
inline var Link(size_t x) { return Link(L"127.0.0.1",x); }
SYSTEM_API var Server(const wstring&,size_t);
inline var Server(size_t x) { return Server(L"127.0.0.1",x); }
SYSTEM_API var Request(Var);
SYSTEM_API var Pipe(const wstring&);

// Differential.cpp
SYSTEM_API var TAG($D), TAG($Derivative), TAG($MultiD);
SYSTEM_API var D(Var x,Var y);
}