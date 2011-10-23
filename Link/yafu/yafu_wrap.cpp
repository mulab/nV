#include "yafu_wrap.h"
#include "yafu/types.h"
#include "yafu/factor.h"
#define HAVE_GMP //used in gmp_xface.h
#include "yafu/gmp_xface.h"
#include "yafu/export.h"
#include <nV/Number.h>

namespace nV {
namespace yafu {
namespace {
var from_factor(factor_t& factor) {
	Integer* t = new Integer();
	mp2gmp(&factor.factor, t->mpz);
	return nV::list(t, new Integer((sint)factor.count));
}
}
var FactorInteger(const Integer& N) {
	factors_t factors = get_factors(const_cast<mpz_ptr>(N.mpz));
	Tuple* r = nV::list(factors.num_factors);
	for (int i = 0; i < factors.num_factors; ++i) {
		r->tuple[i + 1] = from_factor(factors.factors[i]);	
	}
	//Sort the result
	for(uint i=0;i < factors.num_factors; ++i){
		uint j=i;
		while(j>0&&mpz_cmp(Z(r->tuple[j].tuple()[1]).mpz,Z(r->tuple[j+1].tuple()[1]).mpz)>0){
			var temp=r->tuple[j];
			r->tuple[j]=r->tuple[j+1];
			r->tuple[j+1]=temp;
		}
	}
	return r;
}
}
}

using namespace nV;

CAPI void Install(Kernel& k) {
	begin_factors();
}
CAPI void Uninstall(Kernel& k) {
	end_factors();
}

CAPI void CVALUE(YAFU, FactorInteger)(Kernel& k, var& r, Tuple& x) {
	if(x.size != 2 || !x[1].isObject($.Integer))
		return;
	r = yafu::FactorInteger(x[1].cast<Integer>());
}
