#include "StdAfx.h"
#include "CRE.h"
#include "mU_wrap.h"

/*This file contains functions which directly use mU code*/

namespace nV{
	namespace CPE{
		var GcdZ_pp(Kernel& k,const var& f1,const var& f2){
			//Assume f1 f2 are not zero
			vset V1=Variables(k,f1);
			vset V2=Variables(k,f2);
			std::vector<var> Common;
			std::set_intersection(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(Common, Common.end()),more_main);
			if(Common.size()==0)//f1 is number,or f2 is number ,or have no common variable
				return new Integer(1L);
			if(V1.size()==1&&V2.size()==1)//univariate with same variable
				return FromFullCL(k,variable(f1),Polynomial::UniGcdZ(k,ToFullCL(k,f1),ToFullCL(k,f2)));
			else{
				std::vector<var> V;
				std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
				mU::sparse_z f1_mU,f2_mU,d_mU;
				to_sparse_z(f1_mU,f1,V);
				to_sparse_z(f2_mU,f2,V);
				mU::MultiGcdZ(d_mU,f1_mU,f2_mU);
				return from_sparse_z(d_mU,V);
			}
		}
		var GcdZ(Kernel& k,const var& f1,const var& f2){
			if(isZero(f1))
				return f2;
			if(isZero(f2))
				return f1;
			vset V1=Variables(k,f1);
			vset V2=Variables(k,f2);
			std::vector<var> Common;
			std::set_intersection(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(Common, Common.end()),more_main);
			if(Common.size()==0){//f1 is number,or f2 is number ,or have no common variable
				std::vector<var> C1,C2;
				leafs(f1,C1);
				leafs(f2,C2);
				return GCD(C1,C2);
			}
			if(V1.size()==1&&V2.size()==1)//univariate with same variable
				return FromFullCL(k,variable(f1),Polynomial::UniGcdZ(k,ToFullCL(k,f1),ToFullCL(k,f2)));
			else{
				std::vector<var> V;
				std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
				mU::sparse_z f1_mU,f2_mU,d_mU;
				to_sparse_z(f1_mU,f1,V);
				to_sparse_z(f2_mU,f2,V);
				mU::MultiGcdZ(d_mU,f1_mU,f2_mU);
				return from_sparse_z(d_mU,V);
			}
		}
		var Gcd(Kernel& k,const var& ff1,const var& ff2){
			/*Note the case of zero should be handled differently with number
				For example gcd(0,2*x+4*x^2)=2*x+4*x^2/=gcd(0,2,4)
				gcd(6,2*x+4*x^2)=gcd(6,2,4)
			*/
			var f1=ff1,f2=ff2;
			if(!isExact(ff1)){
				wcerr<<"operand is not exact number,conversion is applied\n";
				f1=exact(ff1);
			}
			if(!isExact(ff2)){
				wcerr<<"operand is not exact number,conversion is applied\n";
				f2=exact(ff2);
			}
			if(isZero(f1))
				return f2;
			if(isZero(f2))
				return f1;
			var temp;
			temp=ppZdecompose(k,f1);
			var cont1=temp.tuple()[1];
			var pp1=temp.tuple()[2];
			temp=ppZdecompose(k,f2);
			var cont2=temp.tuple()[1];
			var pp2=temp.tuple()[2];
			var r;
			r=GcdZ_pp(k,pp1,pp2);
			r=CPE::Mul(k,r,GCD(cont1.object(),cont2.object()));
			return r;
		}
		var Lcm(Kernel& k,const var& f1,const var& f2){
			return CPE::Mul(k,DivExact(k,f1, Gcd(k,f1,f2)),f2);
		}
		var GroebnerBasis(Kernel& k,const Tuple& f_){
			uint n=f_.size-1;
			std::vector<var> f_vec(f_.tuple+1,f_.tuple+f_.size);
			for(uint i=n-1;i+1>0;i--){
				if(!isExact(f_vec[i])){
					wcerr<<"all polynomials must be Exact\n";
					return $.Fail;
				}
				if(isNumber(f_vec[i])){
					if(!cmpD(f_vec[i].object(), 0.0))//is zero
							f_vec.erase(f_vec.begin()+i);
						else
							return list(new Integer(1L));
				}
			}
			Tuple& f=*(nV::list(f_vec.size(),f_vec.begin()));
			n=f.size-1;
			if(n==0)
				return &f;
			vset V=Variables(k,f[1]);
			for(uint i=2;i<f.size;i++){
				vset temp=Variables(k,f[i]);
				for(vset::iterator it=temp.begin();it!=temp.end();it++)
					V.insert(*it);
			}
			std::vector<var> VV(V.begin(),V.end());
			std::vector<mU::sparse_q> polylist,gb;
			for(uint i=0;i<n;i++){
				if(isNumber(f[i+1])){
					if(!cmpD(f[i+1].object(), 0.0))//is zero
						continue;
					else
						return list(new Integer(1L));
				}
				polylist.resize(polylist.size()+1);
				to_sparse_q(polylist[polylist.size()-1],f[i+1],VV);
			}
			mU::MultiGroebnerBasisQ(gb,polylist,VV.size());
			Tuple* r=list(gb.size());
			for(uint i=0;i<gb.size();i++)
				r->tuple[i+1]=from_sparse_q(gb[i],VV);
			clear_sparse_q_list(polylist);clear_sparse_q_list(gb);
			return r;
		}
	}
	namespace OLD{//This is only for test of to_sparse_q
		using CPE::more_main;
		var MultiAddQ(Kernel& k,const var& f1,const var& f2){
			CPE::vset V1=CPE::Variables(k,f1);
			CPE::vset V2=CPE::Variables(k,f2);
			std::vector<var> V;
			std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
			mU::sparse_q f1_mU,f2_mU,r_mU;
			CPE::to_sparse_q(f1_mU,f1,V);
			CPE::to_sparse_q(f2_mU,f2,V);
			mU::MultiAddQ(r_mU,f1_mU,f2_mU);
			return CPE::from_sparse_q(r_mU,V);
		}
		var MultiAddZ(Kernel& k,const var& f1,const var& f2){
			CPE::vset V1=CPE::Variables(k,f1);
			CPE::vset V2=CPE::Variables(k,f2);
			std::vector<var> V;
			std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
			mU::sparse_z f1_mU,f2_mU,r_mU;
			CPE::to_sparse_z(f1_mU,f1,V);
			CPE::to_sparse_z(f2_mU,f2,V);
			mU::MultiAddZ(r_mU,f1_mU,f2_mU);
			return CPE::from_sparse_z(r_mU,V);
		}
		var MultiDivExactZ(Kernel& k,const var& f1,const var& f2){
			CPE::vset V1=CPE::Variables(k,f1);
			CPE::vset V2=CPE::Variables(k,f2);
			std::vector<var> V;
			std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
			mU::sparse_z f1_mU,f2_mU,r_mU;
			CPE::to_sparse_z(f1_mU,f1,V);
			CPE::to_sparse_z(f2_mU,f2,V);
			mU::MultiDivExactZ(r_mU,f1_mU,f2_mU);
			return CPE::from_sparse_z(r_mU,V);
		}
		var MultiSubQ(Kernel& k,const var& f1,const var& f2){
			CPE::vset V1=CPE::Variables(k,f1);
			CPE::vset V2=CPE::Variables(k,f2);
			std::vector<var> V;
			std::set_union(V1.begin(),V1.end(),V2.begin(),V2.end(),std::inserter(V,V.end()),more_main);
			mU::sparse_q f1_mU,f2_mU,r_mU;
			CPE::to_sparse_q(f1_mU,f1,V);
			CPE::to_sparse_q(f2_mU,f2,V);
			mU::MultiSubQ(r_mU,f1_mU,f2_mU);
			return CPE::from_sparse_q(r_mU,V);
		}
	}
}
