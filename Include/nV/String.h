#pragma once
#include "Interface.h"

#undef API
#ifdef _MSC_VER
#ifdef STRING_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_String,f)
#undef VALUE
#define VALUE(f) CVALUE(System_String,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_String,f,sig)

namespace nV {
// inline String& S(const var& x) {
//     return x.cast<String>();
// }
// inline String& S(const Object& x) {
//     return x.cast<String>();
// }
}
