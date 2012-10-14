#include "common.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/Kernel.h>
#include <mU/Interface.h>

namespace mU {
//void(*Types[TYPE_SIZE])(Var);
var Contexts = Tab();
stdext::hash_map<Var,const wchar*> ContextName;
std::stack<Var> ContextStack;
std::stack<std::list<Var> > ContextPath;
map_t OwnValues;
std::map<Var,dict_t> FactValues;
std::map<Var,def_t> DownValues;
std::map<Var,def_t> SubValues;
std::map<Var,map_t> Properties;
std::map<Var,attr_t> Attributes;
stdext::hash_map<Var,CProc> CProcs;
stdext::hash_map<Var,COper> COpers;
sint Verbose = 0;
var
Global, System, Null, True, False, Nil, Failed, Aborted, Automatic,
Constant, Flat, HoldAll, HoldAllComplete, HoldFirst,
HoldRest, Listable, Locked, NHoldAll, NHoldFirst,
NHoldRest, NumericFunction, OneIdentity, Orderless, Protected,
ReadProtected, SequenceHold, Stub, Temporary,
Heads, SameTest, IgnoreCase, LongForm,
TAG(Symbol), TAG(General), TAG(List), TAG(String), TAG(Integer), TAG(Rational), TAG(Real),
TAG(Postfix), TAG(Prefix), TAG(Differential),
TAG(Minus), TAG(Divide), TAG(Plus), TAG(Times), TAG(Power),
TAG(Return), TAG(Continue), TAG(Break),
TAG(Set), TAG(Rule), TAG(Pattern), TAG(Blank), TAG(Optional), TAG(Condition),
TAG(PatternTest), TAG(Unevaluated), TAG(Derivative), TAG(Function), TAG(Slot),
TAG(BlankSequence), TAG(SlotSequence), TAG(Part), TAG(Property), TAG(Sequence),
TAG(Sqrt), TAG(Radical), TAG(RuleDelayed), TAG(SetDelayed),
TAG(CompoundExpression), TAG(Integrate), TAG(D), TAG(Sum), TAG(Limit),
TAG(Infinity), TAG(Pi), TAG(E), TAG(I), TAG(Mod), TAG(Dot), TAG(Pow),
TAG(Timing),
TAG(And), TAG(Or), TAG(Not), TAG(If), TAG(For), TAG(While),
TAG(Flatten), TAG(FlattenAll), TAG(SameQ), TAG(Less), TAG(Equal), TAG(Greater),
TAG(UnsameQ), TAG(GreaterEqual), TAG(Unequal), TAG(LessEqual), TAG(FreeQ),
TAG(MatchQ), TAG(MemberQ), TAG(With), TAG(Block), TAG(Module),
TAG(ReplaceRepeated), TAG(Replace), TAG(ReplaceAll), TAG(OddQ), TAG(EvenQ),
TAG(SetAttributes), TAG(StringJoin), TAG(Join), TAG(Attributes),
TAG(Get), TAG(Put), TAG(PutAppend),
TAG(Install), TAG(Uninstall),
TAG(NumberQ), TAG(AtomQ), TAG(IntegerQ), TAG(SymbolQ), TAG(StringQ),
TAG(Pretty), TAG(Input), TAG(Print), TAG(Clear),
TAG(BeginPackage), TAG(EndPackage), TAG(Begin), TAG(End),
TAG(Evaluate), TAG(Dispatch), TAG(Length), TAG(Path), TAG(Head), TAG(Context),
TAG(Contexts), TAG(ContextPath), TAG(Apply), TAG(Map), TAG(Unset), TAG(FullForm),
TAG(ToString), TAG(ToExpression),
TAG(Exit), TAG(Quit), TAG(Hold), TAG(Run), /*TAG(Task), TAG(Kill),*/ TAG(Array),
TAG(Table), TAG(Do), TAG(Box), TAG(N), TAG(IntegerPart),
TAG(Floor), TAG(Ceiling), TAG(Round),
TAG(Expand), TAG(Variables), TAG(Coefficient),
TAG(Exponent), TAG(Deg), TAG(CoefficientList), TAG(FromCoefficientList),
TAG(Graphics2D), TAG(Graphics3D),
TAG(Options), TAG(StringLength), TAG(StringInsert), TAG(StringTake),
TAG(StringDrop),
TAG(DownValues), TAG(UpValues), TAG(Protect), TAG(Unprotect),
TAG(Quiet), TAG(ListQ), TAG(NumericQ), TAG(First), TAG(Rest),
TAG(Prepend), TAG(Dimensions), TAG(LeafCount), TAG(ConstantArray),
TAG(Abort), TAG(Interrupt), TAG(AbortProtect), TAG(CheckAbort),
TAG(Throw), TAG(Catch), TAG(Reap), TAG(Sow), TAG(Depth),
TAG(Min), TAG(Max), TAG(Complex), TAG(Alternatives), TAG(Empty), TAG(Default), TAG(Inequality);

var TAG(general), TAG(error), TAG(usage), TAG(argx), TAG(argt), TAG(argr), TAG(argrx), TAG(argb), TAG(argbu);
var TAG(nonopt), TAG(opttf), TAG(level), TAG(optx), TAG(atomicx);

uint InputLineCounter = 0;

void Initialize()
{
	static bool Initialized = false;
	if (Initialized) return;

	Global = Ctx(L"Global`");
	System = Ctx(L"System`");

	DEF_SYSTEM_SYM(Protected)
	DEF_SYSTEM_SYM(Null)
	DEF_SYSTEM_SYM(True)
	DEF_SYSTEM_SYM(False)
	DEF_SYSTEM_SYM(Nil)
	DEF_SYSTEM_SYM(Constant)
	DEF_SYSTEM_SYM(Flat)
	DEF_SYSTEM_NAME_SYM(Aborted, L"$Aborted")
	DEF_SYSTEM_NAME_SYM(Failed, L"$Failed")
	DEF_SYSTEM_SYM(Automatic)
	DEF_SYSTEM_SYM(HoldAll)
	DEF_SYSTEM_SYM(HoldAllComplete)
	DEF_SYSTEM_SYM(HoldFirst)
	DEF_SYSTEM_SYM(HoldRest)
	DEF_SYSTEM_SYM(Listable)
	DEF_SYSTEM_SYM(Locked)
	DEF_SYSTEM_SYM(NHoldAll)
	DEF_SYSTEM_SYM(NHoldFirst)
	DEF_SYSTEM_SYM(NHoldRest)
	DEF_SYSTEM_SYM(NumericFunction)
	DEF_SYSTEM_SYM(OneIdentity)
	DEF_SYSTEM_SYM(Orderless)
	DEF_SYSTEM_SYM(ReadProtected)
	DEF_SYSTEM_SYM(SequenceHold)
	DEF_SYSTEM_SYM(Stub)
	DEF_SYSTEM_SYM(Temporary)
	DEF_SYSTEM_SYM(Heads)
	DEF_SYSTEM_SYM(SameTest)
	DEF_SYSTEM_SYM(IgnoreCase)
	DEF_SYSTEM_SYM(LongForm)

	DEF_SYSTEM_TAG_SYM(Symbol)
	DEF_SYSTEM_TAG_SYM(General)
	DEF_SYSTEM_TAG_SYM(List)
	DEF_SYSTEM_TAG_SYM(String)
	DEF_SYSTEM_TAG_SYM(Integer)
	DEF_SYSTEM_TAG_SYM(Rational)
	DEF_SYSTEM_TAG_SYM(Real)
	DEF_SYSTEM_TAG_SYM(Return)
	DEF_SYSTEM_TAG_SYM(Continue)
	DEF_SYSTEM_TAG_SYM(Break)
	DEF_SYSTEM_TAG_SYM(Set)
	DEF_SYSTEM_TAG_SYM(Rule)
	DEF_SYSTEM_TAG_SYM(Minus)
	DEF_SYSTEM_TAG_SYM(Divide)
	DEF_SYSTEM_TAG_SYM(Plus)
	DEF_SYSTEM_TAG_SYM(Times)
	DEF_SYSTEM_TAG_SYM(Power)
	DEF_SYSTEM_TAG_SYM(Integrate)
	DEF_SYSTEM_TAG_SYM(D)
	DEF_SYSTEM_TAG_SYM(Postfix)
	DEF_SYSTEM_TAG_SYM(Prefix)
	DEF_SYSTEM_TAG_SYM(Differential)
	DEF_SYSTEM_TAG_SYM(Pattern)
	DEF_SYSTEM_TAG_SYM(Blank)
	DEF_SYSTEM_TAG_SYM(Optional)
	DEF_SYSTEM_TAG_SYM(Condition)
	DEF_SYSTEM_TAG_SYM(PatternTest)
	DEF_SYSTEM_TAG_SYM(Unevaluated)
	DEF_SYSTEM_TAG_SYM(Derivative)
	DEF_SYSTEM_TAG_SYM(Function)
	DEF_SYSTEM_TAG_SYM(Slot)
	DEF_SYSTEM_TAG_SYM(BlankSequence)
	DEF_SYSTEM_TAG_SYM(SlotSequence)
	DEF_SYSTEM_TAG_SYM(Part)
	DEF_SYSTEM_TAG_SYM(Property)
	DEF_SYSTEM_TAG_SYM(Sequence)
	DEF_SYSTEM_TAG_SYM(RuleDelayed)
	DEF_SYSTEM_TAG_SYM(SetDelayed)
	DEF_SYSTEM_TAG_SYM(CompoundExpression)
	DEF_SYSTEM_TAG_SYM(Sum)
	DEF_SYSTEM_TAG_SYM(Limit)
	DEF_SYSTEM_TAG_SYM(Infinity)
	DEF_SYSTEM_TAG_SYM(Pi)
	DEF_SYSTEM_TAG_SYM(E)
    DEF_SYSTEM_TAG_SYM(I)
    DEF_SYSTEM_TAG_SYM(Sqrt)
    DEF_SYSTEM_TAG_SYM(Radical)
    DEF_SYSTEM_TAG_SYM(Mod)
    DEF_SYSTEM_TAG_SYM(Dot)
    DEF_SYSTEM_TAG_SYM(Pow)
	DEF_SYSTEM_TAG_SYM(Timing)
    DEF_SYSTEM_TAG_SYM(And)
    DEF_SYSTEM_TAG_SYM(Or)
    DEF_SYSTEM_TAG_SYM(Not)
    DEF_SYSTEM_TAG_SYM(If)
    DEF_SYSTEM_TAG_SYM(For)
    DEF_SYSTEM_TAG_SYM(While)
    DEF_SYSTEM_TAG_SYM(Flatten)
    DEF_SYSTEM_TAG_SYM(FlattenAll)
	DEF_SYSTEM_TAG_SYM(SameQ)
    DEF_SYSTEM_TAG_SYM(Less)
    DEF_SYSTEM_TAG_SYM(Equal)
    DEF_SYSTEM_TAG_SYM(Greater)
    DEF_SYSTEM_TAG_SYM(UnsameQ)
    DEF_SYSTEM_TAG_SYM(GreaterEqual)
    DEF_SYSTEM_TAG_SYM(Unequal)
    DEF_SYSTEM_TAG_SYM(LessEqual)
	DEF_SYSTEM_TAG_SYM(FreeQ)
    DEF_SYSTEM_TAG_SYM(MatchQ)
    DEF_SYSTEM_TAG_SYM(MemberQ)
    DEF_SYSTEM_TAG_SYM(With)
    DEF_SYSTEM_TAG_SYM(Block)
    DEF_SYSTEM_TAG_SYM(Module)
    DEF_SYSTEM_TAG_SYM(ReplaceRepeated)
	DEF_SYSTEM_TAG_SYM(Replace)
    DEF_SYSTEM_TAG_SYM(ReplaceAll)
    DEF_SYSTEM_TAG_SYM(OddQ)
    DEF_SYSTEM_TAG_SYM(EvenQ)
    DEF_SYSTEM_TAG_SYM(SetAttributes)
    DEF_SYSTEM_TAG_SYM(StringJoin)
    DEF_SYSTEM_TAG_SYM(Join)
    DEF_SYSTEM_TAG_SYM(Attributes)
	DEF_SYSTEM_TAG_SYM(Get)
    DEF_SYSTEM_TAG_SYM(Put)
    DEF_SYSTEM_TAG_SYM(PutAppend)
    DEF_SYSTEM_TAG_SYM(Install)
    DEF_SYSTEM_TAG_SYM(Uninstall)
    DEF_SYSTEM_TAG_SYM(NumberQ)
    DEF_SYSTEM_TAG_SYM(AtomQ)
    DEF_SYSTEM_TAG_SYM(IntegerQ)
	DEF_SYSTEM_TAG_SYM(SymbolQ)
    DEF_SYSTEM_TAG_SYM(StringQ)
    DEF_SYSTEM_TAG_SYM(Pretty)
    DEF_SYSTEM_TAG_SYM(Input)
    DEF_SYSTEM_TAG_SYM(Print)
    DEF_SYSTEM_TAG_SYM(Clear)
    DEF_SYSTEM_TAG_SYM(BeginPackage)
    DEF_SYSTEM_TAG_SYM(EndPackage)
	DEF_SYSTEM_TAG_SYM(Begin)
    DEF_SYSTEM_TAG_SYM(End)
    DEF_SYSTEM_TAG_SYM(Evaluate)
    DEF_SYSTEM_TAG_SYM(Dispatch)
    DEF_SYSTEM_TAG_SYM(Length)
    DEF_SYSTEM_TAG_SYM(Path)
    DEF_SYSTEM_TAG_SYM(Head)
    DEF_SYSTEM_TAG_SYM(Context)
	DEF_SYSTEM_TAG_SYM(Contexts)
    DEF_SYSTEM_TAG_SYM(ContextPath)
    DEF_SYSTEM_TAG_SYM(Apply)
    DEF_SYSTEM_TAG_SYM(Map)
    DEF_SYSTEM_TAG_SYM(Unset)
    DEF_SYSTEM_TAG_SYM(FullForm)
    DEF_SYSTEM_TAG_SYM(ToString)
    DEF_SYSTEM_TAG_SYM(ToExpression)
	DEF_SYSTEM_TAG_SYM(Exit)
    DEF_SYSTEM_TAG_SYM(Quit)
    DEF_SYSTEM_TAG_SYM(Hold)
    DEF_SYSTEM_TAG_SYM(Run)
    //DEF_SYSTEM_TAG_SYM(Task)
    //DEF_SYSTEM_TAG_SYM(Kill)
    DEF_SYSTEM_TAG_SYM(Array)
    DEF_SYSTEM_TAG_SYM(Table)
    DEF_SYSTEM_TAG_SYM(Do)
    DEF_SYSTEM_TAG_SYM(Box)
	DEF_SYSTEM_TAG_SYM(N)
    DEF_SYSTEM_TAG_SYM(IntegerPart)
    DEF_SYSTEM_TAG_SYM(Floor)
    DEF_SYSTEM_TAG_SYM(Ceiling)
    DEF_SYSTEM_TAG_SYM(Round)
    DEF_SYSTEM_TAG_SYM(Expand)
    DEF_SYSTEM_TAG_SYM(Variables)
    DEF_SYSTEM_TAG_SYM(Coefficient)
	DEF_SYSTEM_TAG_SYM(Exponent)
    DEF_SYSTEM_TAG_SYM(Deg)
    DEF_SYSTEM_TAG_SYM(CoefficientList)
    DEF_SYSTEM_TAG_SYM(FromCoefficientList)
    DEF_SYSTEM_TAG_SYM(Graphics2D)
    DEF_SYSTEM_TAG_SYM(Graphics3D)
	DEF_SYSTEM_TAG_SYM(Options)
    DEF_SYSTEM_TAG_SYM(StringLength)
    DEF_SYSTEM_TAG_SYM(StringInsert)
    DEF_SYSTEM_TAG_SYM(StringTake)
    DEF_SYSTEM_TAG_SYM(StringDrop)
    DEF_SYSTEM_TAG_SYM(DownValues)
    DEF_SYSTEM_TAG_SYM(UpValues)
    DEF_SYSTEM_TAG_SYM(Protect)
    DEF_SYSTEM_TAG_SYM(Unprotect)
    DEF_SYSTEM_TAG_SYM(Quiet)
    DEF_SYSTEM_TAG_SYM(ListQ)
    DEF_SYSTEM_TAG_SYM(NumericQ)
    DEF_SYSTEM_TAG_SYM(First)
    DEF_SYSTEM_TAG_SYM(Rest)
    DEF_SYSTEM_TAG_SYM(Prepend)
    DEF_SYSTEM_TAG_SYM(Dimensions)
	DEF_SYSTEM_TAG_SYM(LeafCount)
	DEF_SYSTEM_TAG_SYM(ConstantArray)
	DEF_SYSTEM_TAG_SYM(Abort)
	DEF_SYSTEM_TAG_SYM(Interrupt)
	DEF_SYSTEM_TAG_SYM(AbortProtect)
	DEF_SYSTEM_TAG_SYM(CheckAbort)
	DEF_SYSTEM_TAG_SYM(Throw)
	DEF_SYSTEM_TAG_SYM(Catch)
	DEF_SYSTEM_TAG_SYM(Reap)
	DEF_SYSTEM_TAG_SYM(Sow)
	DEF_SYSTEM_TAG_SYM(Depth)
	DEF_SYSTEM_TAG_SYM(Min)
	DEF_SYSTEM_TAG_SYM(Max)
	DEF_SYSTEM_TAG_SYM(Complex)
	DEF_SYSTEM_TAG_SYM(Alternatives)
	DEF_SYSTEM_TAG_SYM(Empty)
	DEF_SYSTEM_TAG_SYM(Default)
	DEF_SYSTEM_TAG_SYM(Inequality)

	DEF_SYSTEM_TAG_SYM(general)
	DEF_SYSTEM_TAG_SYM(usage)
	DEF_SYSTEM_TAG_SYM(argx)
	DEF_SYSTEM_TAG_SYM(argt)
	DEF_SYSTEM_TAG_SYM(error)
	DEF_SYSTEM_TAG_SYM(argr)
	DEF_SYSTEM_TAG_SYM(argrx)
	DEF_SYSTEM_TAG_SYM(argb)
	DEF_SYSTEM_TAG_SYM(argbu)
	DEF_SYSTEM_TAG_SYM(nonopt)
	DEF_SYSTEM_TAG_SYM(opttf)
	DEF_SYSTEM_TAG_SYM(level)
	DEF_SYSTEM_TAG_SYM(optx)
	DEF_SYSTEM_TAG_SYM(atomicx)

	ContextPath.push(std::list<Var>());
	ContextPath.top().push_back(System);
	ContextPath.top().push_back(Global);
	ContextStack.push(Global);
	MessageLists.push_back(std::vector<KernelMessage>());

	if (!cinstall("musystem")) {
		wcerr << _W("System library not found, quit...") << std::endl;
		return;
	}
		
	Initialized = true;
}
}
