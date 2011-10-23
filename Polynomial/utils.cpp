#include "StdAfx.h"
#include "utils.h"
#include <nV/Number.h>

namespace nV{
	Tuple* insert(const Tuple&x,uint k,const var& a){//0<=k<x.size
		Tuple* r=tuple(x.size+1);
		r->tuple[0]=x[0];//r->[0] not working
		for(uint i=1;i<k;i++)
			r->tuple[i]=x[i];
		r->tuple[k]=a;
		for(uint i=k+1;i<=x.size;i++)
			r->tuple[i]=x[i-1];
		return r;
	}
	Tuple* replace(const Tuple&x,uint k,const var& a){//1<=k<x.size
		Tuple* r=tuple(x.size);
		r->tuple[0]=x[0];//r->[0] not working
		for(uint i=1;i<k;i++)
			r->tuple[i]=x[i];
		r->tuple[k]=a;
		for(uint i=k+1;i<x.size;i++)
			r->tuple[i]=x[i];
		return r;
	}
	Tuple* erase(const Tuple&x,uint k){//0<=k<x.size
		Tuple* r=tuple(x.size-1);
		r->tuple[0]=x[0];//r->[0] not working
		for(uint i=1;i<k;i++)
			r->tuple[i]=x[i];
		for(uint i=k;i<x.size-1;i++)
			r->tuple[i]=x[i+1];
		return r;
	}
	Tuple* cons(const var& a,const Tuple& x){
		return insert(x,1,a);
	}
	var GCD(const std::vector<var>& V){
		if(V.empty())
			return new Integer(0L);
		if(V.size()==1)
			return V[0];
		var d=Number::GCD(Z(V[0]),Z(V[1]));
		for(uint i=2;i<V.size();i++)
			d=Number::GCD(Z(d),Z(V[i]));
		return d;
	}
	var GCD(const std::vector<var>& V1,const std::vector<var>& V2){
		var d1=GCD(V1);
		var d2=GCD(V2);
		return Number::GCD(Z(d1),Z(d2));
	}
	var BinomialTable(Kernel& k, uint n){
		Tuple* r=list(n+1);
		var one=new Integer(1L);
		r->tuple[0+1]=list(one);
		if(n>0)
			r->tuple[1+1]=list(one,one);
		for (uint i=2;i<=n;i++)//(i,j)
		{
			Tuple* r_i=list(i+1);
			r_i->tuple[0+1]=one;
			r_i->tuple[i+1]=one;
			r_i->tuple[1+1]=new Integer(i);
			r_i->tuple[i]=new Integer(i);
			for (uint j=2;j<=i/2;j++)
			{
				r_i->tuple[j+1]=Z(r->tuple[i].tuple()[j])+Z(r->tuple[i].tuple()[j+1]);
				r_i->tuple[i-j+1]=r_i->tuple[j+1];
			}
			r->tuple[i+1]=r_i;
		}
		return r;
	}
	NChooseKEnumeator::NChooseKEnumeator(uint _n,uint _k):n(_n),k(_k){
		end=false;
		for(uint i=0;i<k;i++)
			S.push_back(i);
		h=k-1;
	}
	void NChooseKEnumeator::next(){
		if(S[0]==n-k){
			end=true;
			return;
		}
		if(S[h]>=n-k+h)
			h--;
		else
			h=k-1;
		S[h]++;
		for(uint i=h+1;i<k;i++)
			S[i]=S[i-1]+1;
	}
	var GCD(const Object& x,const Object& y){
		var rr;
		if(x.type==$.Integer){
			if(y.type==$.Integer)
				rr=Number::GCD(Z(x),Z(y));
			else{//y is Rationnal
				rr=Number::GCD(Z(x),Z(numerator(Q(y))));
				rr=rr.object()/denominator(Q(y)).object();
			}
		}
		else{
			if(y.type==$.Integer){
				rr=Number::GCD(Z(y),Z(numerator(Q(x))));
				rr=rr.object()/denominator(Q(x)).object();
			}
			else{
				var d1=denominator(Q(x));
				var d2=denominator(Q(y));
				rr=d1.object()/Number::GCD(Z(d1),Z(d2)).object();
				rr=rr.object()*d2.object();
				rr=Number::GCD(Z(numerator(Q(x))),Z(numerator(Q(y)))).object()/rr.object();
			}
		}
		return rr;
	}
}
using namespace nV;
CAPI void CVALUE(System,NChooseKEnumeator)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	NChooseKEnumeator it(Z(x[1]).toUI(),Z(x[2]).toUI());
	while(!it.is_end()){
		for(uint i=0;i<it.size();i++)
			wcout<<it[i]<<" ";
		wcout<<endl;
		it.next();
	}
}
