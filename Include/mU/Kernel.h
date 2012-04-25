#pragma once

#include "Var.h"
#include "Exceptions.h"

namespace mU {
//////////////////////////////////////

// 内核初始化函数
API void Initialize();

// 表达式排序相关函数
API int Compare(Var,Var);
inline bool Before(Var x, Var y) { return Compare(x,y) < 0; }
inline bool Same(Var x, Var y) { return Compare(x,y) == 0; }
inline bool After(Var x, Var y) { return Compare(x,y) > 0; }
struct Before2 { inline bool operator()(Var x, Var y) const { return Before(x,y); } };
struct After2 { inline bool operator()(Var x, Var y) const { return After(x,y); } };

API int Order(Var,Var);
inline bool Less(Var x, Var y) { return Order(x,y) < 0; }
inline bool Equal(Var x, Var y) { return Order(x,y) == 0; }
inline bool Greater(Var x, Var y) { return Order(x,y) > 0; }
struct Less2 { inline bool operator()(Var x, Var y) const { return Less(x,y); } };
struct Greater2 { inline bool operator()(Var x, Var y) const { return Greater(x,y); } };
inline void Sort(Var x) { std::sort(CVec(x).begin(),CVec(x).end(),Less); }

API bool FreeQ(Var,Var);

// 符号定义相关
typedef std::map<Var,var> map_t;
typedef std::map<var,var,Before2> dict_t;
struct def_t {
	DLL static sint default_type;
	typedef std::vector<std::pair<var, std::pair<var,var> > > vec_t;
	vec_t* vec;
	typedef std::map<var,std::pair<var,var>,After2> map_t;
	map_t* map;
	def_t() : vec(0),map(0) {}
	void new_vec() {
		vec = new vec_t;
	}
	void new_map() {
		map = new map_t;
	} 
	bool is_vec() const {
		return vec != 0;
	}
	bool is_map() const {
		return map != 0;
	}
	DLL void insert(Var x, Var y, Var z);
	void erase(Var x) {
		if (vec) {
			
		} else if (map) {
			(*map).erase(x);
		} 
	}
	~def_t() {
		if (vec)
			delete vec;
		else if (map)
			delete map;
	}
};
typedef std::set<Var> attr_t;
typedef var(*CProc)(Var);
typedef var(*COper)(Var,Var);
API map_t OwnValues;
API std::map<Var,dict_t> FactValues;
API std::map<Var,def_t> DownValues;
API std::map<Var,def_t> SubValues;
API std::map<Var,map_t> Properties;
API std::map<Var,attr_t> Attributes;
API stdext::hash_map<Var,CProc> CProcs;
API stdext::hash_map<Var,COper> COpers;
API stdext::hash_set<Var> TraceRuleSymbols;
API var GetDownValues(Var);
API var GetUpValues(Var);

API var Eval(Var);
inline void PrintMessage() {
	
}
API var SafeEval(Var);
API void Set(Var,Var);
API void Unset(Var);
API void Clear(Var);

API var Increment(Var);
API var PreIncrement(Var);
API var Decrement(Var);
API var PreDecrement(Var);

API var Thread(Var,Var);
API void Flatten(Var,Var);
API void Flatten(Var,Var,Var);
API void FlattenAll(Var,Var);
API void FlattenAll(Var,Var,Var);

// 测试某个表达式中是否不含有模式??
API bool FixQ(Var);

API var Supply(Var,Var,Var);

template <typename T>
var Subs(const T& m, Var x)
{
	typename T::const_iterator
        iter = m.find(x);
	if(iter != m.end())
		return iter->second;
	if(VecQ(x))
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = Subs(m,At(x,i));
		return r;
	}
	if(ExQ(x))
		return Ex(Subs(m,Head(x)),Subs(m,Body(x)));
	return x;
}

// 符号表相关
API var Contexts;
API stdext::hash_map<Var,const wchar*> ContextName;
API std::stack<Var> ContextStack;
API std::stack<std::list<Var> > ContextPath;
inline var Context() { return ContextStack.top(); }
inline void Begin(Var x) { ContextStack.push(x); }
inline void End() { ContextStack.pop(); }
API void BeginPackage(Var);
API void EndPackage();
API var Ctx(const wstring&);
API var Sym(const wstring&);
API var Ctx2(const wchar*);
API var Sym2(const wchar*);
API wstring Unique();

API var Optimi(Var);

API var ParseList(wistream& = wcin);
API var Parse(wistream& = wcin);
API var ParseFile(const wstring&);
API var Get(const wstring&);
API var ParseString(const wstring&);
API var ToExpression(const wstring&);
API var Read(wistream&,Var = 0);
API void Write(wostream&,Var);
API void Print(Var,wostream& = wcout,size_t = 0);
API void FullPrint(Var,wostream& = wcout);

API var Plus(Var);
API var Plus(Var,Var);
API var Times(Var);
API var Times(Var,Var);
API var Pretty(Var);
inline void Println(Var x, wostream &f = wcout)
{
	Print(Pretty(x),f);
	f << std::endl;
}
API sint Verbose;
#define SYSTEM_SYM(x) Sym(_W(#x),System)
#define CPROC(f) mU::var f(mU::Var x)

#define CPROC_INT2(f, argnummin, argnummax)	\
CPROC(f##_##argnummin##_##argnummax##_internal);	\
CAPI mU::var f(mU::Var x)				\
{								\
	if (mU::Size(x) < argnummin || mU::Size(x) > argnummax)		\
{								\
	if (mU::Size(x) == 1)		\
		CLogMessage(TAG(argbu), CprocSymbolStack.top(), Int(static_cast<mU::uint>(argnummin)), \
					Int(static_cast<mU::uint>(argnummax))); \
	else						\
		CLogMessage(TAG(argb), CprocSymbolStack.top(), Int(static_cast<mU::uint>(mU::Size(x))), \
					Int(static_cast<mU::uint>(argnummin)), Int(static_cast<mU::uint>(argnummax)));	\
	return 0;					\
}								\
	return f##_##argnummin##_##argnummax##_internal(x);\
}								\
CPROC(f##_##argnummin##_##argnummax##_internal)

#define CPROC_INT(f, argnum)	\
CPROC(f##_##argnum##_internal);	\
CAPI mU::var f(mU::Var x)				\
{								\
	if (mU::Size(x) != argnum)		\
	{								\
		if (argnum == 1)		\
			CLogMessage(TAG(argx), CprocSymbolStack.top(), Int(static_cast<mU::uint>(mU::Size(x)))); \
		else if (mU::Size(x) == 1)						\
			CLogMessage(TAG(argr), CprocSymbolStack.top(), Int(static_cast<mU::uint>(argnum)));	\
		else					\
			CLogMessage(TAG(argrx), CprocSymbolStack.top(), Int(static_cast<mU::uint>(mU::Size(x))), \
						Int(static_cast<mU::uint>(argnum)));	\
		return 0;					\
	}								\
	return f##_##argnum##_internal(x);\
}\
CPROC(f##_##argnum##_internal)

typedef std::map<Var, var> options_t;
using boost::assign::map_list_of;
API size_t get_arg_options(Var args, options_t &options, size_t start_min);
API boost::logic::tribool fetch_tribool_option(const options_t &options, Var sym);

#define CPROC_OPT_SIG(f) var f(Var x, options_t &opt, size_t argnum)

#define CPROC_INT2_OPT(f, argnummin, argnummax, default_options)	\
CPROC_OPT_SIG(f##_##argnummin##_##argnummax##_opt_internal);	\
CAPI mU::var f(mU::Var x)				\
{										\
	options_t opt = boost::assign::map_list_of default_options; \
	const size_t num_arg = get_arg_options(x, opt, argnummin); \
	if (num_arg < argnummin)		\
	{								\
		if (num_arg == 1)		\
			CLogMessage(TAG(argbu), CprocSymbolStack.top(), Int(static_cast<mU::uint>(argnummin)), \
							Int(static_cast<mU::uint>(argnummax))); \
		else						\
			CLogMessage(TAG(argb), CprocSymbolStack.top(), Int(static_cast<mU::uint>(num_arg)), \
						Int(static_cast<mU::uint>(argnummin)), Int(static_cast<mU::uint>(argnummax)));	\
		return 0;					\
	}								\
	else if (num_arg > argnummax)	\
	{								\
		CLogMessage(TAG(nonopt), At(x, argnummax), Int(static_cast<mU::uint>(argnummax)), \
					Ex(CprocSymbolStack.top(), x));	\
		return 0;					\
	}								\
	return f##_##argnummin##_##argnummax##_opt_internal(x, opt, num_arg);\
}\
CPROC_OPT_SIG(f##_##argnummin##_##argnummax##_opt_internal)

#define CPROC_INT_OPT(f, argnum, default_options)	\
CPROC_OPT_SIG(f##_##argnum##_opt_internal);	\
	CAPI mU::var f(mU::Var x)				\
{										\
	options_t opt = boost::assign::map_list_of default_options; \
	const size_t num_arg = get_arg_options(x, opt, argnum); \
	if (num_arg < argnum)		\
	{								\
		if (num_arg == 1)		\
			CLogMessage(TAG(argr), CprocSymbolStack.top(), Int(static_cast<mU::uint>(argnum))); \
		else						\
			CLogMessage(TAG(argrx), CprocSymbolStack.top(), Int(static_cast<mU::uint>(num_arg)), \
						Int(static_cast<mU::uint>(argnum)));	\
		return 0;					\
	}								\
	else if (num_arg > argnum)	\
	{								\
		CLogMessage(TAG(nonopt), At(x, argnum), Int(static_cast<mU::uint>(argnum)), \
					Ex(CprocSymbolStack.top(), x));	\
		return 0;					\
	}								\
	return f##_##argnum##_opt_internal(x, opt, num_arg);\
}\
CPROC_OPT_SIG(f##_##argnum##_opt_internal)

#define CPROC_OPT(f, default_options)	\
	CPROC_OPT_SIG(f##_opt_internal);	\
CAPI mU::var f(mU::Var x)				\
{										\
	options_t opt = boost::assign::map_list_of default_options; \
	const size_t num_arg = get_arg_options(x, opt, 0); \
	return f##_opt_internal(x, opt, num_arg);		\
}\
CPROC_OPT_SIG(f##_opt_internal)

#define CPROC_ATOMIC_(f)			\
CAPI mU::var f##_atomic(mU::Var);	\
CAPI mU::var f(mU::Var x)			\
{								\
	mU::var r;						\
	try							\
	{							\
	mU::AtomicContext ac;		\
	r = f##_atomic(x);			\
	}							\
	catch (mU::RuntimeException&)				\
	{										\
	mU::check_for_abortion();		\
	throw;							\
	}										\
	catch (mU::LogicError&)					\
	{										\
	mU::check_for_abortion();			\
	throw;								\
	}										\
	catch (mU::Exception&)					\
	{										\
	mU::check_for_abortion();			\
	throw;								\
	}										\
	catch (...)								\
	{										\
	CLogMessage(TAG(atomicx), CprocSymbolStack.top()); \
	throw;									\
	}										\
	mU::check_for_abortion();		\
	return r;					\
}								

#define CPROC_ATOMIC(f)			\
CPROC_ATOMIC_(f)\
CPROC(f##_atomic)

#define CPROC_ATOMIC_INT2(f, argnummin, argnummax)  \
CPROC_ATOMIC_(f)\
CPROC_INT2(f##_atomic, argnummin, argnummax)

#define CPROC_ATOMIC_INT(f, argnum)  \
CPROC_ATOMIC_(f)\
CPROC_INT(f##_atomic, argnum)

#define COPER(f) var f(Var x, Var y)

API var
Constant, Flat, HoldAll, HoldAllComplete, HoldFirst,
HoldRest, Listable, Locked, NHoldAll, NHoldFirst,
// FIXME: we do not actually protect symbols labeled "Protected" or "ReadProtected"
// FIXME: predefined symbols should be protected
NHoldRest, NumericFunction, OneIdentity, Orderless, Protected,
ReadProtected, SequenceHold, Stub, Temporary, 
Heads, SameTest, Automatic, 
//IgnoreCase,  //Newly added
TAG(Symbol), TAG(General), TAG(List), TAG(String), TAG(Integer), TAG(Rational), TAG(Real),
TAG(Postfix), TAG(Prefix), TAG(Differential), TAG(Minus), TAG(Divide), TAG(Plus),
TAG(Times), TAG(Power), TAG(Return), TAG(Continue), TAG(Break), TAG(Set), TAG(Rule),
TAG(Pattern), TAG(Blank), TAG(Optional), TAG(Condition), TAG(PatternTest),
TAG(Unevaluated), TAG(Derivative), TAG(Function), TAG(Slot), TAG(BlankSequence),
TAG(SlotSequence), TAG(Part), TAG(Property), TAG(Sequence), TAG(Sqrt), TAG(Radical),
TAG(RuleDelayed), TAG(SetDelayed), TAG(CompoundExpression), TAG(Integrate), TAG(D), TAG(Sum), TAG(Limit),
TAG(Infinity), TAG(Pi), TAG(E), TAG(I), TAG(Mod), TAG(Dot), TAG(Pow),
TAG(Timing), TAG(And), TAG(Or), TAG(Not), TAG(If), TAG(For), TAG(While), TAG(Flatten), TAG(FlattenAll), 
TAG(SameQ), TAG(Less), TAG(Equal), TAG(Greater), TAG(UnsameQ), TAG(GreaterEqual), TAG(Unequal), TAG(LessEqual), 
TAG(FreeQ), TAG(MatchQ), TAG(MemberQ), TAG(With), TAG(Block), TAG(Module), TAG(ReplaceRepeated), 
TAG(Replace), TAG(ReplaceAll), TAG(OddQ), TAG(EvenQ), TAG(SetAttributes), TAG(StringJoin), TAG(Join), TAG(Attributes), 
TAG(Get), TAG(Put), TAG(PutAppend), TAG(Install), TAG(Uninstall), TAG(NumberQ), TAG(AtomQ), TAG(IntegerQ), 
TAG(SymbolQ), TAG(StringQ), TAG(Pretty), TAG(Input), TAG(Print), TAG(Clear), TAG(BeginPackage), TAG(EndPackage), 
TAG(Begin), TAG(End), TAG(Evaluate), TAG(Dispatch), TAG(Length), TAG(Path), TAG(Head), TAG(Context), 
TAG(Contexts), TAG(ContextPath), TAG(Apply), TAG(Map), TAG(Unset), TAG(FullForm), TAG(ToString), TAG(ToExpression), 
TAG(Exit), TAG(Quit), TAG(Hold), TAG(Run), TAG(Task), TAG(Kill), TAG(Array), TAG(Table), TAG(Do), TAG(Box),
TAG(N), TAG(IntegerPart), TAG(Floor), TAG(Ceiling), TAG(Round), TAG(Expand), TAG(Variables), TAG(Coefficient), 
TAG(Exponent), TAG(Deg), TAG(CoefficientList), TAG(FromCoefficientList), TAG(Graphics2D), TAG(Graphics3D),
TAG(Options), TAG(StringLength), TAG(StringInsert), TAG(StringTake), TAG(StringDrop),
TAG(DownValues), TAG(UpValues), TAG(Protect), TAG(Unprotect),
TAG(Quiet), TAG(ListQ), TAG(NumericQ), TAG(First), TAG(Rest), TAG(Prepend),
TAG(Dimensions), TAG(LeafCount), TAG(ConstantArray), 
TAG(Abort), TAG(Interrupt), TAG(AbortProtect), TAG(CheckAbort),
TAG(Throw), TAG(Catch), TAG(Reap), TAG(Sow), TAG(Depth),
TAG(Min), TAG(Max), TAG(Complex), TAG(Alternatives), TAG(Empty), TAG(Default), TAG(Inequality);

const var NInfinity = Ex(TAG(Times), Vec(Int(static_cast<mU::sint>(-1)), TAG(Infinity)));

#define SET_ATTR(x,y) Attributes[x].insert(y);
#define SET_TAG_ATTR(x,y) Attributes[TAG(x)].insert(y);
#define DEF_SYSTEM_SYM(x) x = Sym(_W(#x),System); SET_ATTR(x, Protected);
#define DEF_SYSTEM_NAME_SYM(x, name) x = Sym(name,System); SET_ATTR(x, Protected);
#define DEF_SYSTEM_TAG_SYM(x) TAG(x) = Sym(_W(#x),System); SET_TAG_ATTR(x, Protected);
#define DEF_CPROC(x) CProcs[TAG(x)] = x;
#define DEF_WRAPPED_CPROC(x) CProcs[TAG(x)] = WRAP(x);
#define DECL_TAG_SYM_WRAPPED_CPROC(x) var DEF_SYSTEM_TAG_SYM(x)DEF_WRAPPED_CPROC(x)
#define DEF_WRAPPED_COPER(x) COpers[TAG(x)] = WRAP(x);
#define DECL_TAG_SYM_WRAPPED_COPER(x) var DEF_SYSTEM_TAG_SYM(x)DEF_WRAPPED_COPER(x)

inline var Tag(Var x)
{
	switch(Type(x))
	{
	case TYPE(null): return Null;
	case TYPE(obj): return CObj<obj_t>(x).tag();
	case TYPE(int): return TAG(Integer);
	case TYPE(rat): return TAG(Rational);
	case TYPE(flt): return TAG(Real);
	case TYPE(str): return TAG(String);
	case TYPE(sym): return TAG(Symbol);
	case TYPE(vec): return TAG(List);
	case TYPE(ex): return Head(x);
	}
	return x;
}

inline bool NumberQ(Var x)
{
	switch (Type(x))
	{
	case TYPE(int):
	case TYPE(rat):
	case TYPE(flt):
		return true;
	case TYPE(obj):
		return CObj<obj_t>(x).tag() == TAG(Complex);
	case TYPE(sym):
		return x == TAG(I);
	default:
		return false;
	}
}

inline bool ZeroQ(Var x)
{
	return (IntQ(x) && mpz_cmp_ui(CInt(x),0L) == 0)
		|| (RatQ(x) && mpq_cmp_ui(CRat(x),0L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_ui(CFlt(x),0L) == 0)
		;
}

inline bool OneQ(Var x)
{
	return (IntQ(x) && mpz_cmp_ui(CInt(x),1L) == 0)
		|| (RatQ(x) && mpq_cmp_ui(CRat(x),1L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_ui(CFlt(x),1L) == 0)
		;
}

inline bool NOneQ(Var x)
{
	return (IntQ(x) && mpz_cmp_si(CInt(x),-1L) == 0)
		|| (RatQ(x) && mpq_cmp_si(CRat(x),-1L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_si(CFlt(x),-1L) == 0)
		;
}

inline bool OddQ(Var x)
{
	return IntQ(x) && mpz_odd_p(CInt(x));
}

inline bool EvenQ(Var x)
{
	return IntQ(x) && mpz_even_p(CInt(x));
}

inline boost::logic::tribool toTribool(Var expr)
{
	if (expr == True)
	{
		return true;
	}
	else if (expr == False)
	{
		return false;
	}
	else
	{
		return boost::logic::indeterminate;
	}

}

API uint InputLineCounter;

//////////////////////////////////////
}
