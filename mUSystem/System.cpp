#include "common.h"
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/System.h>
#include <mU/Interface.h>
#include <mU/utils.h>

using namespace mU;

CAPI void mUInstall() {
	static bool Initialized = false;
	if (Initialized) return;

	DEF_SYSTEM_TAG_SYM($D)
	DEF_SYSTEM_TAG_SYM($Derivative)
	DEF_SYSTEM_TAG_SYM($MultiD)

	DEF_CPROC(Plus)
	DEF_CPROC(Times)
	DEF_CPROC(Dot)

	SET_ATTR(Failed, Locked)
	SET_ATTR(Aborted, Locked)

	SET_TAG_ATTR(CompoundExpression,HoldAll)
	// SET_TAG_ATTR(CompoundExpression,Flat)
    SET_TAG_ATTR(Timing,HoldAll)
    SET_TAG_ATTR(For,HoldAll)
    SET_TAG_ATTR(While,HoldAll)
	SET_TAG_ATTR(Plus,OneIdentity)
    SET_TAG_ATTR(Plus,Flat)
    SET_TAG_ATTR(Plus,Orderless)
    SET_TAG_ATTR(Plus,Listable)
	SET_TAG_ATTR(Plus,NumericFunction)
	SET_TAG_ATTR(Times,OneIdentity)
    SET_TAG_ATTR(Times,Flat)
    SET_TAG_ATTR(Times,Orderless)
    SET_TAG_ATTR(Times,Listable)
	SET_TAG_ATTR(Times,NumericFunction)
	SET_TAG_ATTR(Power,OneIdentity)
    SET_TAG_ATTR(Power,Listable)
	SET_TAG_ATTR(Power,NumericFunction)
    SET_TAG_ATTR(Pi,Constant)
    SET_TAG_ATTR(E,Constant)
	SET_TAG_ATTR(Set,HoldFirst)
    SET_TAG_ATTR(SetDelayed,HoldAll)
    SET_TAG_ATTR(Hold,HoldAll)
	SET_TAG_ATTR(And,OneIdentity)
    SET_TAG_ATTR(And,Flat)
    SET_TAG_ATTR(And,HoldAll)
    SET_TAG_ATTR(Or,OneIdentity)
    SET_TAG_ATTR(Or,Flat)
    SET_TAG_ATTR(Or,HoldAll)
	SET_TAG_ATTR(If,HoldRest)
    SET_TAG_ATTR(Pattern,HoldFirst)
    SET_TAG_ATTR(Condition,HoldAll)
    SET_TAG_ATTR(PatternTest,HoldRest)
	SET_TAG_ATTR(With,HoldAll)
    SET_TAG_ATTR(Block,HoldAll)
    SET_TAG_ATTR(Module,HoldAll)
	SET_TAG_ATTR(SetAttributes,HoldFirst)
    SET_TAG_ATTR(StringJoin,Flat)
    SET_TAG_ATTR(StringJoin,OneIdentity)
	SET_TAG_ATTR(Join,Flat)
    SET_TAG_ATTR(Join,OneIdentity)
    SET_TAG_ATTR(Attributes,HoldAll)
    SET_TAG_ATTR(Clear,HoldAll)
	SET_TAG_ATTR(Unevaluated,HoldAll)
    SET_TAG_ATTR(Function,HoldAll)
    SET_TAG_ATTR(Context,HoldFirst)
    SET_TAG_ATTR(Property,HoldAll)
	SET_TAG_ATTR(RuleDelayed,HoldRest)
    SET_TAG_ATTR(Unset,HoldFirst)
    //SET_TAG_ATTR(Task,HoldAll)
    SET_TAG_ATTR(Table,HoldAll)
    SET_TAG_ATTR(Do,HoldAll)
	SET_TAG_ATTR(Protect,HoldAll)
	SET_TAG_ATTR(Unprotect,HoldAll)
	SET_TAG_ATTR(AbortProtect, HoldAll)
	SET_TAG_ATTR(CheckAbort, HoldAll)
	SET_TAG_ATTR(Catch, HoldFirst)
	SET_TAG_ATTR(Reap, HoldFirst)

	void* M = cnoload("musystem");
	var S = Sym(_W("CProc"),System);
	CProcs[S] = (CProc)cfunc(M, S);
	Begin(System);
	Get(path() + _W("System/init.m"));
	
	Initialized = true;
}
CAPI void mUUninstall() {
	wcout << "#System# Uninstall Successfully!" << std::endl;
}
