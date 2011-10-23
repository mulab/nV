#include "StdAfx.h"
#include "CRE.h"
namespace nV {
	namespace SparseArray {
		var newSparseArray(const var& Q,sint i){
			Tuple* C=list(Q);
			var I=new Array<sint>;
			A<sint>(I).vec.push_back(i);
			return newSparseArray(C,I);
		}
		var Add(Kernel& k, const var& x, const var& y){
			if(nnz(x)==0)
				return y;
			if(nnz(y)==0)
				return x;
			std::vector<var> Sd;
			var Index=new Array<sint>;
			std::vector<sint>& Si=A<sint>(Index).vec;
			//Sd.reserve(nnz(x)>nnz(y)?nnz(x):nnz(y));
			//Si.reserve(nnz(x)>nnz(y)?nnz(x):nnz(y));
			Sd.reserve(nnz(x)+nnz(y));
			Si.reserve(nnz(x)+nnz(y));
			uint xi=0,yi=0;
			while(xi<nnz(x)&&yi<nnz(y)){
				if(index(x,xi)<index(y,yi)){
					Sd.push_back(data(x,xi));
					Si.push_back(index(x,xi));
					xi++;
				}
				else if(index(x,xi)>index(y,yi)){
					Sd.push_back(data(y,yi));
					Si.push_back(index(y,yi));
					yi++;
				}
				else if(index(x,xi)==index(y,yi)){
					var temp=CRE::Add(k,data(x,xi),data(y,yi));
					if(!isZero(temp)){
						Sd.push_back(temp);
						Si.push_back(index(x,xi));
					}
					xi++;
					yi++;
				}
			}
			while(xi<nnz(x)){
				Sd.push_back(data(x,xi));
				Si.push_back(index(x,xi));
				xi++;
			}
			while(yi<nnz(y)){
				Sd.push_back(data(y,yi));
				Si.push_back(index(y,yi));
				yi++;
			}
			return newSparseArray(nV::list(Sd.size(),Sd.begin()),Index);
		}
		var Neg(Kernel& k, const var& x){
			if(nnz(x)==0)
				return x;
			Tuple* D=list(nnz(x));
			var minus_one=new Integer(-1L);
			for(uint i=0;i<nnz(x);i++){
				D->tuple[i+1]=CRE::Mul(k,minus_one,data(x,i));
			}
			return newSparseArray(D,index_as_var(x));
		}
		API var Sub(Kernel& k, const var& x, const var& y){
			return Add(k,x,Neg(k,y));
		}
		var Convolution(Kernel& k,const var& P,const var& c,const sint& e){//P(x)*(c*x^e)
			var expo=new Array<sint>(index(P));
			std::vector<sint>& Ve=A<sint>(expo).vec;
			Tuple* C=list(nnz(P));
			for(uint i=0;i<nnz(P);i++){
				C->tuple[i+1]=CRE::Mul(k,c,data(P,i));
				Ve[i]+=e;
			}
			return newSparseArray(C,expo);
		}
		var Convolution(Kernel& k, const var& x, const var& y){
			if(nnz(x)==0||nnz(y)==0)
				return newSparseArray();
			var xx,yy;
			if(nnz(x)<=nnz(y)){//Improve the speed a lot
				xx=x;
				yy=y;
			}
			else{
				xx=y;
				yy=x;
			}
			var r=Convolution(k,yy,data(xx,0),index(xx,0));
			for(uint i=1;i<nnz(xx);i++){
				var temp=Convolution(k,yy,data(xx,i),index(xx,i));
				r=SparseArray::Add(k,r,temp);
			}
			return r;
		}
		var ScalarMul(Kernel& k,const var& y,const var& x){
			//x is a scalar
			if(isZero(x))
				return newSparseArray();
			if(x==$.Infinity){
				if(nnz(y)==0)
					return $.NaN;
				return x;
			}
			Tuple* C=list(nnz(y));
			for(uint i=0;i<nnz(y);i++)
				C->tuple[i+1]=CRE::Mul(k,x,data(y,i));
			return newSparseArray(C,index_as_var(y));
		}
		var ScalarDiv(Kernel& k,const var& y,const var& x){
			if(isZero(x)){
				if(nnz(y)==0)
					return $.NaN;
				return $.Infinity;
			}
			if(x==$.Infinity)
				return new Integer(0L);
			return ScalarMul(k,y,CRE::Inv(k,x));
		}
		var cdr(const var& P){
			uint n=nnz(P);//assume n>0
			Tuple* C=list(n-1,data(P).tuple().tuple+1);
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			Se.assign(index(P).begin(),index(P).end()-1);
			return newSparseArray(C,expo);
		}
		sint maxindex(const var& x){
			if(nnz(x)==0)
				return 0;
			return index(x,nnz(x)-1);
		}
		var Shift(Kernel& k,const var& P,sint e){
			if(e==0)
				return P;
			var expo=new Array<sint>(index(P));
			std::vector<sint>& Ve=A<sint>(expo).vec;
			for(uint i=0;i<nnz(P);i++){
				Ve[i]+=e;
			}
			return newSparseArray(data(P),expo);
		}
		var lc(const var& x){
			//assume nnz(x)>0
			return data(x,nnz(x)-1);
		}
		var Div(Kernel& k,const var& f,const var& g){
			if(nnz(g)==0)
				return list($.Infinity,new Integer(0L));
			if(nnz(f)==0)
				return list(f,f);
			var lc_g=lc(g);
			sint deg_g=maxindex(g);
			var g_=cdr(ScalarDiv(k,g,lc_g));
			var q=newSparseArray();
			var r=f;
			while(deg_g<=maxindex(r)){
				uint s=maxindex(r)-deg_g;
				q=SparseArray::Add(k,q,newSparseArray(lc(r),s));
				r=SparseArray::Sub(k,cdr(r),Shift(k,ScalarMul(k,g_,lc(r)),s));
			}
			return list(ScalarDiv(k,q,lc_g),r);
		}
		var PseudoRemainder(Kernel& k,const var& f,const var& g){
			if(nnz(g)==0||(nnz(g)==1&&index(g,0)==0))
				return $.Fail;
			var r=f;
			sint s=maxindex(r)-maxindex(g);
			while(s>=0){
				r=SparseArray::Sub(k,ScalarMul(k,cdr(r),lc(g)),Shift(k,ScalarMul(k,cdr(g),lc(r)),s));
				s=maxindex(r)-maxindex(g);
			}
			return r;
		}
	}
}
