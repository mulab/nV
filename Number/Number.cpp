#include "StdAfx.h"
#include <nV/Number.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#endif

using namespace nV;

CAPI void Install(Kernel& k) {
    //wcout << "#Number# Install Successfully!" << endl;
    Number::Randomize();
}
CAPI void Uninstall(nV::Kernel& k) {
    wcout << "#Number# Uninstall Successfully!" << endl;
}