#include "flint_wrap.h"
#include <nV/Number.h>

namespace nV{
	namespace flint{
		fmpq_poly_ptr to_fmpq_poly(const Tuple& x){
			uint n=x.size-1;
			fmpq_poly_ptr y = new fmpq_poly_struct;
			fmpq_poly_init2(y,n);
			for(uint i=0;i<n;i++){
				if(x[i+1].isObject($.Rational))
					fmpq_poly_set_coeff_mpq(y,i,x[i+1].cast<Rational>().mpq);
				else//Assume it is Integer(mpz_t)
					fmpq_poly_set_coeff_mpz(y,i,x[i+1].cast<Integer>().mpz);
			}
			return y;
		}
		fmpz_poly_ptr to_fmpz_poly(const Tuple& x){
			uint n=x.size-1;
			fmpz_poly_ptr y = new fmpz_poly_struct;
			fmpz_poly_init2(y,n);
			fmpz_t temp;
			fmpz_init(temp);
			for(uint i=0;i<n;i++){
				fmpz_set_mpz(temp,x[i+1].cast<Integer>().mpz);
				fmpz_poly_set_coeff_fmpz(y,i,temp);
			}
			return y;
		}
		Tuple* from_fmpq_poly(fmpq_poly_t x){
			uint n=fmpq_poly_length(x);
			Tuple* r=list(n);
			mpq_t temp;
			mpq_init(temp);
			for(uint i=0;i<n;i++){
				fmpq_poly_get_coeff_mpq(temp,x,i);
				if(mpz_cmp_ui(mpq_denref(temp),1)==0){
					r->tuple[i+1]=new Integer;
					mpz_set(r->tuple[i+1].cast<Integer>().mpz,mpq_numref(temp));
				}
				else{
					r->tuple[i+1]=new Rational;
					mpq_set(r->tuple[i+1].cast<Rational>().mpq,temp);
				}
			}
			return r;
		}
		Tuple* from_fmpz_poly(fmpz_poly_t x){
			uint n=fmpz_poly_length(x);
			Tuple* r=list(n);
			fmpz_t temp;
			fmpz_init(temp);
			for(uint i=0;i<n;i++){
				fmpz_poly_get_coeff_fmpz(temp,x,i);
				r->tuple[i+1]=new Integer;
				fmpz_get_mpz(r->tuple[i+1].cast<Integer>().mpz,temp);
			}
			return r;
		}
		var AddPolyQ(const Tuple& x,const Tuple& y){
			fmpq_poly_t res;
			fmpq_poly_init(res);
			fmpq_poly_add(res,to_fmpq_poly(x),to_fmpq_poly(y));
			return from_fmpq_poly(res);
		}
		var MulPolyQ(const Tuple& x,const Tuple& y){
			fmpq_poly_t res;
			fmpq_poly_init(res);
			fmpq_poly_mul(res,to_fmpq_poly(x),to_fmpq_poly(y));
			return from_fmpq_poly(res);
		}
		var PowerPolyQ(const Tuple& x,uint e){
			fmpq_poly_t res;
			fmpq_poly_init(res);
			fmpq_poly_pow(res,to_fmpq_poly(x),e);
			return from_fmpq_poly(res);
		}
		var AddPolyZ(const Tuple& x,const Tuple& y){
			fmpz_poly_t res;
			fmpz_poly_init(res);
			fmpz_poly_add(res,to_fmpz_poly(x),to_fmpz_poly(y));
			return from_fmpz_poly(res);
		}
		var MulPolyZ(const Tuple& x,const Tuple& y){
			fmpz_poly_t res;
			fmpz_poly_init(res);
			fmpz_poly_mul(res,to_fmpz_poly(x),to_fmpz_poly(y));
			return from_fmpz_poly(res);
		}
		var PowerPolyZ(const Tuple& x,uint e){
			fmpz_poly_t res;
			fmpz_poly_init(res);
			fmpz_poly_pow(res,to_fmpz_poly(x),e);
			return from_fmpz_poly(res);
		}
	}
}
using namespace nV;
CAPI void CVALUE(FLINT,QAdd)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::AddPolyQ(x[1].tuple(),x[2].tuple());
}
CAPI void CVALUE(FLINT,QMul)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::MulPolyQ(x[1].tuple(),x[2].tuple());
}
CAPI void CVALUE(FLINT,QPower)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::PowerPolyQ(x[1].tuple(),Z(x[2]).toUI());
}
CAPI void CVALUE(FLINT,ZAdd)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::AddPolyZ(x[1].tuple(),x[2].tuple());
}
CAPI void CVALUE(FLINT,ZMul)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::MulPolyZ(x[1].tuple(),x[2].tuple());
}
CAPI void CVALUE(FLINT,ZPower)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=flint::PowerPolyZ(x[1].tuple(),Z(x[2]).toUI());
}
