#include "NTL_wrap.h"
#include <NTL/ZZXFactoring.h>

namespace nV{
	namespace LinkNTL{
		var AddPolyZ(const Tuple& x,const Tuple& y){
			return from_ZZX(to_ZZX(x)+to_ZZX(y));
		}
		var MulPolyZ(const Tuple& x,const Tuple& y){
			return from_ZZX(to_ZZX(x)*to_ZZX(y));
		}
	}
}
using namespace nV;

CAPI void Install(Kernel& k) {
    // wcout << "#LinkNTL# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
    wcout << "#LinkNTL# Uninstall Successfully!" << endl;
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(NTL,f)
#undef VALUE
#define VALUE(f) CVALUE(NTL,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(NTL,f,sig)

CAPI void VALUE(Diff)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZX(NTL::diff(to_ZZX(x[1].tuple())));
}
CAPI void VALUE(Add)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isTuple())
        return;
    NTL::ZZX c = to_ZZX(x[1].tuple());
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isTuple())
            c += to_ZZX(x[i].tuple());
    r = from_ZZX(c);
}
CAPI void VALUE(Sub)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(to_ZZX(x[1].tuple()) - to_ZZX(x[2].tuple()));
}
CAPI void VALUE(Neg)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZX(-to_ZZX(x[1].tuple()));
}
CAPI void VALUE(Mul)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isTuple())
        return;
    NTL::ZZX c = to_ZZX(x[1].tuple());
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isTuple())
            c *= to_ZZX(x[i].tuple());
    r = from_ZZX(c);
}
CAPI void VALUE(Sqr)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZX(NTL::sqr(to_ZZX(x[1].tuple())));
}
CAPI void VALUE(PseudoDivRem)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    NTL::ZZX q, c;
    NTL::PseudoDivRem(q, c, to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple()));
    r = list(from_ZZX(q), from_ZZX(c));
}
CAPI void VALUE(PseudoDiv)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(NTL::PseudoDiv(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(PseudoRem)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(NTL::PseudoRem(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(DivRem)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    NTL::ZZX q, c;
    NTL::DivRem(q, c, to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple()));
    r = list(from_ZZX(q), from_ZZX(c));
}
CAPI void VALUE(Div)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(to_ZZX(x[1].tuple()) / to_ZZX(x[2].tuple()));
}
CAPI void VALUE(Rem)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(to_ZZX(x[1].tuple()) % to_ZZX(x[2].tuple()));
}
CAPI void VALUE(Divide)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    NTL::ZZX q;
    long flag = NTL::divide(q, to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple()));
    r = list(boolean(flag != 0), from_ZZX(q));
}
CAPI void VALUE(Content)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZ(NTL::content(to_ZZX(x[1].tuple())));
}
CAPI void VALUE(PrimitivePart)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZX(NTL::PrimitivePart(to_ZZX(x[1].tuple())));
}
CAPI void VALUE(GCD)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1 || !x[1].isTuple())
        return;
    NTL::ZZX c = to_ZZX(x[1].tuple());
    for (uint i = 2; i < x.size; ++i)
        if (x[i].isTuple())
            c = NTL::GCD(c, to_ZZX(x[i].tuple()));
    r = from_ZZX(c);
}
CAPI void VALUE(TraceMod)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZ(NTL::TraceMod(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(Resultant)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZ(NTL::resultant(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(NormMod)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZ(NTL::NormMod(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(Discriminant)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    r = from_ZZ(NTL::discriminant(to_ZZX(x[1].tuple())));
}
CAPI void VALUE(CharPolyMod)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(NTL::CharPolyMod(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(MinPolyMod)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    r = from_ZZX(NTL::MinPolyMod(to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple())));
}
CAPI void VALUE(XGCD)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 3 || !x[1].isTuple() || !x[2].isTuple())
        return;
    NTL::ZZ c;
    NTL::ZZX s, t;
    NTL::XGCD(c, s, t, to_ZZX(x[1].tuple()), to_ZZX(x[2].tuple()));
    r = list(from_ZZ(c), from_ZZX(s), from_ZZX(t));
}
CAPI void VALUE(Factor)(Kernel& k, var& r, Tuple& x) {
    if (x.size != 2 || !x[1].isTuple())
        return;
    NTL::ZZ c;
    NTL::vec_pair_ZZX_long factors;
    NTL::factor(c, factors, to_ZZX(x[1].tuple()));
    uint n = factors.length();
    Tuple* t = list(n + 1);
    t->tuple[1] = from_ZZ(c);
    for (uint i = 0; i < n; ++i)
        t->tuple[i + 2] = list(from_ZZX(factors[i].a), new Integer(factors[i].b));
    r = t;
}
