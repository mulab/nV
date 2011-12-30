#pragma once
#include <nV/Interface.h>
#include <nV/Polynomial.h>
#include "CRE.h"
namespace nV {
	namespace SparseArray {
		/*
			The structure
			($SparseArray (data) (key))
		 */
		inline bool isSparseArray(const var& x){
			return x.isTuple($SparseArray);
		}
		inline uint nnz(const var& x){
			return x.tuple()[1].tuple().size-1;
		}
		inline const var& data(const var& x){
			return x.tuple()[1];
		}
		inline const var& data(const var&x,uint i){
			return x.tuple()[1].tuple()[i+1];//i start with zero
		}
		inline const std::vector<sint> & index(const var&x) {
			return A<sint>(x.tuple()[2]).vec;
		}
		inline const var & index_as_var(const var&x) {
			return x.tuple()[2];
		}
		inline sint index(const var&x,uint i) {
			//exponents are the 4th in tuple
			return index(x)[i];
		}
		sint maxindex(const var& x);
		inline var newSparseArray(const var& data,const var& index){
			return list(data,index,$SparseArray);
		}
		inline var newSparseArray(){
			return list(list(0),new Array<sint>);
		}
		var newSparseArray(const var& Q,sint i=0);
		API var Add(Kernel& k, const var& x, const var& y);
		API var Neg(Kernel& k, const var& x);
		API var Sub(Kernel& k, const var& x, const var& y);
		API var Shift(Kernel& k,const var& P,sint e);
		API var Convolution(Kernel& k, const var& x, const var& y);
		API var ScalarMul(Kernel& k,const var& y,const var& x);
		API var ScalarDiv(Kernel& k,const var& y,const var& x);
		API var Div(Kernel& k,const var& f,const var& g);//elements are treated in R[x1,x2,...]
		API var PseudoRemainder(Kernel& k,const var& f,const var& g);//elements are treated in Q[x1,x2,...]
	}
}
