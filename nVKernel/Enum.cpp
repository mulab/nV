#include <nV/Enum.h>

namespace nV {
bool Enum::Inited = false;
void Enum::Init() {
    if (Inited)
        return;
    sys = SYM(root, System);
#define M(x) $.x = SYS(x);$.PrimaryName[++i] = $.x;
    {
        int i = -1;
        M(Null)M(Object)M(Key)M(Symbol)M(Tuple)
    }
#undef M
#define M(x) $.x = SYS(x);$.Attributes.insert($.x);
    M(Constant)
    M(Flat)
    M(HoldAll)
    M(HoldAllComplete)
    M(HoldFirst)
    M(HoldRest)
    M(Listable)
    M(Locked)
    M(NHoldAll)
    M(NHoldFirst)
    M(NHoldRest)
    M(NumericFunction)
    M(OneIdentity)
    M(Orderless)
    M(Protected)
    M(ReadProtected)
    M(SequenceHold)
    M(Stub)
    M(Temporary)
#undef M
#define M(x) $.x = SYS(x);$.Patterns.insert($.x);
    M(Alternatives)
    M(Blank)
    M(Condition)
    M(Optional)
    M(Pattern)
    M(PatternTest)
    M(Production)
    M(Rule)
    M(RuleDelayed)
#undef M
#define M(x) $.x = SYS(x);$.Objects.insert($.x);
    M(Integer)M(Rational)M(Real)M(String)M(Array)M(Complex)M(RealI)
#undef M
#define M(x) $.Numbers.insert($.x);
	M(Integer)M(Rational)M(Real)M(Complex)M(RealI)
#undef M
#define M(x) $.x = SYS(x);
    M(Assign)
    M(Break)
    M(Continue)
    M(Delayed)
    M(Fail)
    M(False)
    M(Function)
    M(Infinity)
    M(Interface)
    M(List)
    M(Map)
    M(Match)
    M(Method)
    M(Minus)
	M(NaN)
    M(Plus)
    M(Power)
	M(Print)
    M(Radical)
    M(Return)
    M(Self)
    M(Sequence)
    M(Serial)
    M(Set)
    M(Slot)
    M(Sqrt)
    M(Times)
    M(True)
    M(Value)
#undef M
    Inited = true;
}
Enum $;
}