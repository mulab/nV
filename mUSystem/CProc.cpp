#include "StdAfx.h"
#include <mU/System.h>
#include <mU/Interface.h>

using namespace mU;

CAPI CPROC(System_CProc) {
	void* m = cnoload(cstr(At(x,0)).c_str());
	if (!m)
		return False;
	for (size_t i = 1; i < Size(x); ++i) {
		CProc ptr = (CProc)cfunc(m, At(x,i));
		if (!ptr) {
			wcerr << _W("CProc not found: ") << Name(At(x,i)) << std::endl;
			return False;
		}
		CProcs[At(x,i)] = ptr;
	}
	return True;
}
CAPI CPROC(System_COper) {
	void* m = cnoload(cstr(At(x,0)).c_str());
	if (!m)
		return False;
	for (size_t i = 1; i < Size(x); ++i) {
		COper ptr = (COper)cfunc(m, At(x,i));
		if (!ptr) {
			wcerr << _W("COper not found: ") << Name(At(x,i)) << std::endl;
			return False;
		}
		COpers[At(x,i)] = ptr;
	}
	return True;
}
CAPI CPROC(System_CInstall) {
	return cinstall(cstr(At(x,0)).c_str()) ? True : False;
}
CAPI CPROC(System_CUninstall) {
	return cuninstall(cstr(At(x,0)).c_str()) ? True : False;
}