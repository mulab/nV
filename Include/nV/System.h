#pragma once
#include "Interface.h"

#undef API
#ifdef _MSC_VER
#ifdef SYSTEM_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System,f)
#undef VALUE
#define VALUE(f) CVALUE(System,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System,f,sig)

namespace nV {
API var Attributes(Kernel&, sym);
API bool FreeQ(const var&, const var&);
bool Input(Kernel&, var&, wcs);
inline bool Input(Kernel& k, var& r, const wstring& s) {
    return Input(k, r, s.c_str());
}
bool Output(Kernel&, const var&, wcs);
inline bool Output(Kernel& k, const var& r, const wstring& s) {
    return Output(k, r, s.c_str());
}
bool OutputAppend(Kernel&, const var&, wcs);
inline bool OutputAppend(Kernel& k, const var& r, const wstring& s) {
    return OutputAppend(k, r, s.c_str());
}
API double Timing(Kernel&, var&, const var&);
template <class T>
var With(const var& x, const T& m) {
    typename T::const_iterator
    iter = m.find(x);
    if (iter != m.end())
        return iter->second;
    if (x.isTuple()) {
        Tuple* r = x.tuple().clone();
        for (uint i = 0; i < x.tuple().size; ++i)
            r->tuple[i] = With(x.tuple()[i], m);
        return r;
    }
    return x;
}
API var Pretty(Kernel&, const Integer&);
API var Pretty(Kernel&, const Rational&);
API var Pretty(Kernel&, const Real&);
API var Pretty(Kernel&, const Object&);
API var Pretty(Kernel&, const Key&);
API var Pretty(Kernel&, const Tuple&);
API var Pretty(Kernel&, sym);
API var Pretty(Kernel&, const var&);
}
