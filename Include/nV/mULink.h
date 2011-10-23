#pragma once
#include <mU/Kernel.h>
#include "Interface.h"
#include "Parser.h"

#undef API
#ifdef _MSC_VER
#if defined(mulink_EXPORTS) || defined(MULINK_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace nV {
API mU::var to_mu(sym);
API mU::var to_mu(const Key&);
API mU::var to_mu(const Object&);
API mU::var to_mu(const Tuple&);
API mU::var to_mu(const var&);
API var from_mu(mU::Var);
}
