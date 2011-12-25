#include <nV/Interface.h>
#include <nV/Kernel.h>

namespace nV {
Kernel::Kernel() : recursion(25600), log(&wclog), parser(0) {
	static bool Initialized = false;
	if (Initialized) return;
    
	Enum::Init();
#ifdef _MSC_VER
	mp_get_memory_functions(0, 0, &__gmp_free_func);
#endif
    owns[$.Null] = null;
    mContextPath.push_back(std::list<sym>());
    contextPath().push_back(sys);
    contextPath().push_back(root);
    beginContext(SYM(root, Global));
    start();
	if (!cinstall(*this, "nvsystem")) {
        wcerr << _W("System library not found, quit...") << endl;
    }

	Initialized = true;
}
}

using namespace nV;

CAPI void Install(Kernel& k) {
    //wcout << "#Kernel# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
    wcout << "#Kernel# Uninstall Successfully!" << endl;
}
