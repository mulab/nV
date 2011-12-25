#include "common.h"
#include <nV/Polynomial.h>
#include <mU/Polynomial_impl.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
#pragma comment(lib,"nVNumber")
#endif

namespace nV {
	sym $Polynomial;
	sym $CPE;
	sym $CRE;
	sym $SparseArray;
}
namespace mU{
	mpz_t z_unit,z_zero;
	mpq_t q_unit,q_zero;
}

using namespace nV;

CAPI void Install(Kernel& k) {
    $Polynomial = SYS(Polynomial);
    $CPE = SYS(CPE);
    $CRE = SYS(CRE);
		mpz_init(mU::z_unit);mpz_set_ui(mU::z_unit,1);
    mpz_init(mU::z_zero);mpz_set_ui(mU::z_zero,0);
    mpq_init(mU::q_unit);mpq_set_ui(mU::q_unit,1,1);
    mpq_init(mU::q_zero);mpq_set_ui(mU::q_zero,0,1);
	  mU::random::randomize();
		//UniSolveInitialize();
		//ComplexInitialize();
    //wcout << "#Polynomial# Install Successfully!" << endl;
}
CAPI void Uninstall(nV::Kernel& k) {
    wcout << "#Polynomial# Uninstall Successfully!" << endl;
}
