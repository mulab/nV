#include <nV/String.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#endif

using namespace nV;

CAPI void Install(Kernel& k) {
    //wcout << "#String# Install Successfully!" << endl;
}
CAPI void Uninstall(nV::Kernel& k) {
    wcout << "#String# Uninstall Successfully!" << endl;
}