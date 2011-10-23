#pragma once
#include <nV/Interface.h>
extern "C"{
#include <fmpq_poly.h>
#include <fmpz_poly.h>
}
typedef fmpz_poly_struct * fmpz_poly_ptr;
typedef fmpq_poly_struct * fmpq_poly_ptr;

namespace nV{
	namespace flint{
		Tuple* from_fmpq_poly(fmpq_poly_t);
		fmpq_poly_ptr to_fmpq_poly(const Tuple&);
		Tuple* from_fmpz_poly(fmpz_poly_t);
		fmpz_poly_ptr to_fmpz_poly(const Tuple&);
		var AddPolyQ(const Tuple& x,const Tuple& y);
		var MulPolyQ(const Tuple& x,const Tuple& y);
		var PowerPolyQ(const Tuple& x,uint e);
		var AddPolyZ(const Tuple& x,const Tuple& y);
		var MulPolyZ(const Tuple& x,const Tuple& y);
		var PowerPolyZ(const Tuple& x,uint e);
	}
}
