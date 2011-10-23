#pragma once
#include <nV/Interface.h>
#include <mU/Polynomial_impl.h>

namespace nV{
	namespace CPE{
		void to_sparse_z(mU::sparse_z & r,const var & f,const std::vector<var>& V);
		var from_sparse_z(const mU::sparse_z & f,const std::vector<var>& V);
		//to_sparse_q will handle the conversion of number
		void to_sparse_q(mU::sparse_q & r,const var & f,const std::vector<var>& V);
		//from_sparse_q will handle the conversion of number
		var from_sparse_q(const mU::sparse_q & f,const std::vector<var>& V);
	}
	namespace Polynomial{
		void to_poly_z(mU::poly_z &f,const var& g);
		var from_poly_z(const mU::poly_z& g);
		// V should be sorted and contains variables of f
		var UniGcdZ(Kernel& k,const var& x,const var& y);
		var UniGcdZp(Kernel& k,const var& x,const var& y,const var& p);
		var UniGcdZpExt(Kernel& k,const var& x,const var& y,const var& p);
		var UniFacZp(Kernel& k,const var& x,const var& p);
		var UniFacZ(Kernel& k,const var& x);
		var UniDivZp(Kernel& k,const var& x,const var& y,const var& p);
		var UniAddZ(Kernel& k,const var& x,const var& y);
		var UniAddZp(Kernel& k,const var& x,const var& y,const var& p);
		var UniMulZ(Kernel& k,const var& x,const var& y);
		var UniMulZp(Kernel& k,const var& x,const var& y,const var& p);
	}
}
