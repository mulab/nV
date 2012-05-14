#include <nV/System.h>
#include <nV/utils.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#pragma comment(lib,"nVParser")
#endif

using namespace nV;

CAPI void Install(Kernel& k) {
#define M(x,y) k.attributes[SYS(x)].insert($.y);
    M(CInterface, HoldRest)
#undef M
    cvalue(k, cnoload("nvsystem"), SYS(CInterface));
    var r;
    k.beginContext(sys);
    Input(k, r, path() + _W("System/System.n"));
}
CAPI void Uninstall(Kernel& k) {
    wcout << "#System# Uninstall Successfully!" << endl;
}
