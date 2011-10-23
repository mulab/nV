#include "StdAfx.h"
#include "mU_wrap.h"
#include "nV/Number.h"
#include "CRE.h"
namespace nV{
	namespace CPE{
		void to_sparse_z_helper(mU::sparse_z & r,const var & f,const std::vector<var>& V,uint V_index,std::vector<sint>& expo_stack){
			if(isNumber(f)){
				r.resize(r.size()+1);
				mpz_set(r[r.size()-1].coe,Z(f).mpz);
				r[r.size()-1].exponents.reserve(V.size());
				std::copy(expo_stack.begin(),expo_stack.end(),back_inserter(r[r.size()-1].exponents));
				r[r.size()-1].exponents.resize(V.size(),0);
				return;
			}
			if(less_main(variable(f),V[V_index])){
				expo_stack.push_back(0);
				to_sparse_z_helper(r,f,V,V_index+1,expo_stack);
				expo_stack.pop_back();
			}
			else{//only variable(f)==V[V_index] is possible
				for(uint i=0;i<nnz(f);i++){
					expo_stack.push_back(exponent(f,i));
					to_sparse_z_helper(r,coeff(f,i),V,V_index+1,expo_stack);
					expo_stack.pop_back();
				}
			}
		}
		void to_sparse_z(mU::sparse_z & r,const var & f,const std::vector<var>& V){
			if(!isNumber(f))
				r.monomials.reserve(nnz(f));
			std::vector<sint> expo_stack;
			expo_stack.reserve(V.size());
			to_sparse_z_helper(r,f,V,0,expo_stack);
		}
		var from_sparse_z_helper(const mU::sparse_z & f,const std::vector<var>& V,uint level,uint start,uint end){
			if(level>=V.size()){//we have start+1==end here
				Integer* r=new Integer;
				mpz_set(r->mpz,f[start].coe);
				return r;
			}
			std::vector<uint> changepoints;
			changepoints.push_back(start);
			for(uint i=start+1;i<end;i++){
				if(f[i].exponents[level]!=f[i-1].exponents[level])
					changepoints.push_back(i);
			}
			changepoints.push_back(end);
			uint nnz=changepoints.size()-1;
			if(nnz==1&&f[start].exponents[level]==0)//normailize
				return from_sparse_z_helper(f,V,level+1,start,end);
			Tuple* C=list(nnz);
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			Se.resize(nnz);
			for(uint i=0;i<nnz;i++){
				C->tuple[i+1]=from_sparse_z_helper(f,V,level+1,changepoints[i],changepoints[i+1]);
				Se[i]=f[changepoints[i]].exponents[level];
			}
			return newCPE(V[level],C,expo);
		}
		var from_sparse_z(const mU::sparse_z & f,const std::vector<var>& V){
			if(f.monomials.empty())
				return new Integer(0L);
			return from_sparse_z_helper(f,V,0,0,f.size());
		}
		void to_sparse_q_helper(mU::sparse_q & r,const var & f,const std::vector<var>& V,uint V_index,std::vector<sint>& expo_stack){
			if(isNumber(f)){
				r.resize(r.size()+1);
				if(f.object().type==$.Rational)
					mpq_set(r[r.size()-1].coe,Q(f).mpq);
				else if(f.object().type==$.Integer){
					mpq_set_num(r[r.size()-1].coe,Z(f).mpz);
					mpq_set_den(r[r.size()-1].coe,mU::z_unit);
				}
				else{//Assume f is Real
					mpq_set_f(r[r.size()-1].coe,R(f).mpf);
				}
				r[r.size()-1].exponents.reserve(V.size());
				std::copy(expo_stack.begin(),expo_stack.end(),back_inserter(r[r.size()-1].exponents));
				r[r.size()-1].exponents.resize(V.size(),0);
				return;
			}
			if(less_main(variable(f),V[V_index])){
				expo_stack.push_back(0);
				to_sparse_q_helper(r,f,V,V_index+1,expo_stack);
				expo_stack.pop_back();
			}
			else{//only variable(f)==V[V_index] is possible
				for(uint i=0;i<nnz(f);i++){
					expo_stack.push_back(exponent(f,i));
					to_sparse_q_helper(r,coeff(f,i),V,V_index+1,expo_stack);
					expo_stack.pop_back();
				}
			}
		}
		void to_sparse_q(mU::sparse_q & r,const var & f,const std::vector<var>& V){
			if(!isNumber(f))
				r.monomials.reserve(nnz(f));
			std::vector<sint> expo_stack;
			expo_stack.reserve(V.size());
			to_sparse_q_helper(r,f,V,0,expo_stack);
		}
		var from_sparse_q_helper(const mU::sparse_q & f,const std::vector<var>& V,uint level,uint start,uint end){
			if(level>=V.size()){//we have start+1==end here
				if(mpz_cmp_ui(mpq_denref(f[start].coe),1)){//return zero if equal
					 Rational* r=new Rational;
					 mpq_set(r->mpq,f[start].coe);
					 return r;
				}
				else{
					Integer* r=new Integer;
					mpq_get_num(r->mpz,f[start].coe);
					return r;
				}
			}
			std::vector<uint> changepoints;
			changepoints.push_back(start);
			for(uint i=start+1;i<end;i++){
				if(f[i].exponents[level]!=f[i-1].exponents[level])
					changepoints.push_back(i);
			}
			changepoints.push_back(end);
			uint nnz=changepoints.size()-1;
			if(nnz==1&&f[start].exponents[level]==0)//normailize
				return from_sparse_q_helper(f,V,level+1,start,end);
			Tuple* C=list(nnz);
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			Se.resize(nnz);
			for(uint i=0;i<nnz;i++){
				C->tuple[i+1]=from_sparse_q_helper(f,V,level+1,changepoints[i],changepoints[i+1]);
				Se[i]=f[changepoints[i]].exponents[level];
			}
			return newCPE(V[level],C,expo);
		}
		var from_sparse_q(const mU::sparse_q & f,const std::vector<var>& V){
			if(f.monomials.empty())
				return new Integer(0L);
			return from_sparse_q_helper(f,V,0,0,f.size());
		}
	}
	namespace Polynomial{
		void to_poly_z(mU::poly_z &f, const var& gg){
			const Tuple& g=gg.tuple();
			uint n = g.size-1;
			f.resize(n);
			for(uint i = 0; i < n; ++i)
			{
				mpz_set(f[i], Z(g[i+1]).mpz);
			}
		}
		var from_poly_z(const mU::poly_z& g){
			uint n = g.size();
			Tuple* r=list(n);
			for(uint i = 0; i < n; ++i)
			{
				r->tuple[i+1]=new Integer;
				mpz_set(Z(r->tuple[i+1]).mpz, g[i]);
			}
			return r;
		}

		var UniGcdZ(Kernel& k,const var& x,const var& y){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniGcdZ(h,f,g);
			return from_poly_z(h);
		}
		var UniGcdZp(Kernel& k,const var& x,const var& y,const var& p){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniGcdZp(h,f,g,Z(p).mpz);
			return from_poly_z(h);
		}
		var UniGcdZpExt(Kernel& k,const var& x,const var& y,const var& p){
			mU::poly_z f,g,s,t,d;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniGcdZp_Ext(d,s,t,f,g,Z(p).mpz);
			return list(from_poly_z(d),from_poly_z(s),from_poly_z(t));
		}
		var UniDivZp(Kernel& k,const var& x,const var& y,const var& p){
			mU::poly_z f,g,q,r;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniDivModZp(q,r,f,g,Z(p).mpz);
			return list(from_poly_z(q),from_poly_z(r));
		}
		var UniFacZp(Kernel& k,const var& f,const var& p){
			mU::poly_z fz;
			std::vector<mU::poly_z> faclist;
			std::vector<uint> exponents;
			var b = new Integer;
			to_poly_z(fz,f);
			mU::UniFacZp(fz,Z(p).mpz,Z(b).mpz,faclist,exponents);
			uint n=faclist.size();
			Tuple* r=list(n+1);
			r->tuple[1]=b;
			for(size_t i=0;i<n;i++)
				r->tuple[i+2]=list(from_poly_z(faclist[i]),new Integer(exponents[i]));
			clear_poly_z_list(faclist);
			return r;
		}
		var UniFacZ(Kernel& k,const var& f){
			mU::poly_z fz;
			std::vector<mU::poly_z> faclist;
			std::vector<uint> exponents;
			var b = new Integer;
			to_poly_z(fz,f);
			mU::UniFacZ(fz,Z(b).mpz,faclist,exponents);
			uint n=faclist.size();
			Tuple* r=list(n+1);
			r->tuple[1]=b;
			for(size_t i=0;i<n;i++)
				r->tuple[i+2]=list(from_poly_z(faclist[i]),new Integer(exponents[i]));
			clear_poly_z_list(faclist);
			return r;
		}
		var UniAddZ(Kernel& k,const var& x,const var& y){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniAddZ(h,f,g);
			return from_poly_z(h);
		}
		var UniAddZp(Kernel& k,const var& x,const var& y,const var& p){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniAddZ(h,f,g);
			mU::UniPolynomialMod(h,h,Z(p).mpz);
			return from_poly_z(h);
		}
		var UniMulZ(Kernel& k,const var& x,const var& y){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniMulZ(h,f,g);
			return from_poly_z(h);
		}
		var UniMulZp(Kernel& k,const var& x,const var& y,const var& p){
			mU::poly_z h,f,g;
			to_poly_z(f,x);
			to_poly_z(g,y);
			mU::UniMulZ(h,f,g);
			mU::UniPolynomialMod(h,h,Z(p).mpz);
			return from_poly_z(h);
		}
	}
}
using namespace nV;
#define Export1(function)																						\
	CAPI void CVALUE(System,function)(Kernel& k, var& r, Tuple& x) {	\
		if(x.size!=2)																										\
			return;																												\
		r=Polynomial::function(k,x[1]);																	\
	}
#define Export2(function)																						\
	CAPI void CVALUE(System,function)(Kernel& k, var& r, Tuple& x) {	\
		if(x.size!=3)																										\
			return;																												\
		r=Polynomial::function(k,x[1],x[2]);														\
	}
#define Export3(function)																						\
	CAPI void CVALUE(System,function)(Kernel& k, var& r, Tuple& x) {	\
		if(x.size!=4)																										\
			return;																												\
		r=Polynomial::function(k,x[1],x[2],x[3]);												\
	}
Export2(UniGcdZ)
Export3(UniGcdZp)
Export3(UniGcdZpExt)
Export3(UniDivZp)
Export2(UniAddZ)
Export2(UniMulZ)
Export3(UniAddZp)
Export3(UniMulZp)
Export1(UniFacZ)
Export2(UniFacZp)
