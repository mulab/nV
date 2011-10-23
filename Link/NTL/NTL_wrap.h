#pragma once
#include <nV/Interface.h>
#define  NTL_GMP_LIP
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

#undef API
#ifdef _MSC_VER
#if defined(nvlinkntl_EXPORTS) || defined(NVLINKNTL_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif


namespace nV {
	NTL::ZZ to_ZZ(const Integer&);
	var from_ZZ(const NTL::ZZ&);
	NTL::ZZX to_ZZX(const Tuple&);
	Tuple* from_ZZX(const NTL::ZZX&);
}
