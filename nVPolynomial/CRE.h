#pragma once
#include <nV/Number.h>
#include <nV/Array.hpp>
#include <set>
#include <nV/Polynomial.h>
#include "SparseArray.h"
#include "utils.h"
namespace nV{
	namespace CPE {

		/*CPE for Canonical Polynomial Expressions
			See section Polynomials in manual of maxima
		*/

		inline bool more_main(const var& x,const var& y){
			return x>y;
		}
		inline bool less_main(const var& x,const var& y){
			return x<y;
		}
		struct less_main_T{
			inline bool operator()(const var& x,const var& y){
				return more_main(x,y);
			}
		};
		typedef std::set<var,less_main_T> vset;

		/* Basic Arithmetic */
		API var Add(Kernel& k, const var& x, const var& y);
		API var Neg(Kernel& k, const var& x);
		API var Sub(Kernel& k, const var& x, const var& y);
		API var Shift(Kernel& k,const var& P,sint e);//P(x)*x^e
		API var Mul(Kernel& k, const var& x, const var& y);
		API var Mul(Kernel& k,const var& P,const var& c,const sint& e);//P(x)*(c*x^e)
		API var Square(Kernel& k, const var& x);
		API var Power(Kernel& k,const var& x,uint n);
		// functions used by Power
		API var Power0(Kernel& k,const var& x,uint n);
		//API var Power1(Kernel& k,const var& x,uint n);
		API std::pair<var,var> split(const var b);
		API var PowerToN(Kernel& k,const var bi,const var& x,uint n);
		API var PowerToN(Kernel& k,const var& x,uint n);
		API var Derivate(Kernel& k,const var& P);//Derivate in main variable

		/* Reorder of variables.Maybe support custom sort function in future
		   Note answer may be wrong mixing sorting function */
		API var to_main(Kernel& k,const var& P,const var& x);//represent P with x as main variable,other are sorted natively
		API var from_main(Kernel& k,const var& P,const var& x);//restore a to_mained function to normal order

		/* Divide */
		//Check divisible,return quotient when divisible
		API var DivExact(Kernel& k,const var&f,const var&g);
		//return quotient and remainder.Use the first common variable as main variable when not divisible
		API var Div(Kernel& k,const var&f,const var&g);
		API var Div(Kernel& k,const var&f,const var&g,const var& x);//P[x,y,z...] is treated as R[y,z,...][x]
		API var PseudoRemainder(Kernel& k,const var&f,const var&g,const var& x);

		/* conversion between univariate polynomial and full coeff list */
		API var ToFullCL(Kernel& k, const var& P);
		API var FromFullCL(Kernel& k, const var &variable_x,const var& x);

		/* infomations */
		enum UniType {nonCPE_t=-2,number_t,multivariate_t,Integer_t,Rational_t,Real_t};
		UniType unitype(const var& x);
		sym coeff_type(const var& P);
		inline bool isExact(const var& P){
			return coeff_type(P)!=$.Real;
		}
		API vset Variables(Kernel& k, const var& x);
		//P is in CPE,V is in CPE
		API var Coefficient(Kernel& k, const var& P,const var& V);
		API var Coefficient(Kernel& k, const var& P,const var& x,uint i);
		API var ConstantTerm(Kernel& k,const var&P);
		API var ConstantTerm(Kernel& k,const var&P,const var& x);
		API sint degree(Kernel& k,const var& P, const var &varia);
		API sint degree(Kernel& k,const var& P, const std::vector<var>& V,uint start=0);//V should be sorted and distince,start denotes where to start the comparision of variable
		API var lc(Kernel& k,const var& P, const var& x);
		var lc_deep(const var& P);
		void leafs(const var& P,std::vector<var>& L);//push all coeffs of P to L
		var exact(const var& P);//convert Real coefficients to Rational
		var LCM_of_coeff_den(const var& P);//return LCM of all coeffs,P have only Integer or Rational coeffs
		var integralize(Kernel& k,const var& P);
		var ppZdecompose(Kernel& k,const var& P);

		/* helper functions for Factor */
		//P mode I(start:)^k assume I is sorted
		API var ModI(Kernel& kernel, const var& P,const std::vector<var>& I,sint k,uint start=0);
		API var ModP(Kernel& k, const var& x,const var& p);//x(mod p)
		API var Mod2P(Kernel& k, const var& x,const var& p);//x(mod p)
		API var Eval(Kernel& k,const var &P,const var& varia,const var& x);//P(x)
		API var replace_xi(Kernel& k,const var& P,const var &x ,uint i,const var& c);

		//functions which use mU code
		API var FactorList(Kernel& kernel,const var& f);
		API var Factor(Kernel& kernel,const var& f);
		API var FactorListZ(Kernel& kernel,const var& f);
		API var Gcd(Kernel& k,const var& f,const var& g);
		API var Lcm(Kernel& k,const var& f,const var& g);
		API var GcdZ(Kernel& k,const var& f,const var& g);
		var GcdQExt(Kernel& k,const var& f,const var& g);//only work for univariate on Q
		API var CharacterSet(Kernel& k,const Tuple& f);
		API	var GroebnerBasis(Kernel& k,const Tuple& f);


		/*
			Structure is ($CPE variable SparseArray)
		*/
		inline bool isCPE(const var &x){
			return isNumber(x)||x.isTuple($CPE);
		}
		API bool isUnivariate(const var& P);
		//bool isNumber(const var& x); Just use isNumber in Common.h is OK
		inline const var& variable(const var& x){
			return x.tuple()[1];
		}
		inline const var& coeff_list(const var& x){
			return x.tuple()[2];
		}
		inline uint nnz(const var& x){
			return SparseArray::nnz(coeff_list(x));
		}
		inline const var& coeff(const var& x){
			return SparseArray::data(coeff_list(x));
		}
		inline const var& coeff(const var&x,uint i){
			return coeff(x).tuple()[i+1];//index start with zero
		}
		inline const var& lc(const var&x){
			return coeff(x,nnz(x)-1);
		}
		inline const std::vector<sint> & exponent(const var&x) {
			return SparseArray::index(coeff_list(x));
		}
		inline const var & exponent_as_var(const var&x) {
			return SparseArray::index_as_var(coeff_list(x));
		}
		inline const sint& exponent(const var&x,uint i) {
			//exponents are the 4th in tuple
			return exponent(x)[i];
		}
		inline sint degree(const var&x){
			return exponent(x,nnz(x)-1);
		}
		/* functions hanle the representation of CPE in Tuple */
		inline var newCPE(const var& variable,const var& coeff,const var& exponent){
			return list(variable,SparseArray::newSparseArray(coeff,exponent),$CPE);
		}
		inline var newCPE(const var& variable,const var& coeff_list){
			return list(variable,coeff_list,$CPE);
		}
		var newCPE(const var& x,sint i=1,const var& Q=new Integer(1L));//construct Q*x^i
		var insertCPE(const var& P,uint k,const var& c,sint e);
		var eraseCPE(const var& P,uint k);
		inline var consCPE(const var& c,sint e,const var& P){
			return insertCPE(P,0,c,e);
		}
		inline var replaceCPE(const var& P,uint k,const var& c){//repace k-th term in P(x) by c
			return newCPE(variable(P),replace(coeff(P).tuple(),k+1,c),exponent_as_var(P));
		}

		//Nomalize a temporary CPE
		/*var normalize(const var& x){
			if(nnz(x)==0)
			return new Integer(0L);
			if(nnz(x)==1&&exponent(x,0)==0)
			return coeff(x,0);
			}*/
	}
	namespace CRE{
		/*CPE for Canonical Rational Expressions
			See section Polynomials in manual of maxima
		*/

		API var Add(Kernel& k, const var& x, const var& y);
		API var Neg(Kernel& k, const var& x);
		API var Sub(Kernel& k, const var& x, const var& y);
		API var Mul(Kernel& k, const var& x, const var& y);
		API var Inv(Kernel& k, const var& x);
		API var Power(Kernel& k,const var& x,sint n);
		API var Div(Kernel& k,const var&f,const var&g);
		API var Div(Kernel& k,const var&f,const var&g,const var& x);//f,g treated as (R[y,z,...])[x]

		/*denominator and numerator are CPE,
			den is not number,lc_deep(den)>0,num/=0,
			gcd(den,num)=1 if den and num are both exact or keep unchange
		*/
		inline bool isCRE(const var &x){
			return CPE::isCPE(x)||x.isTuple($CRE);
		}
		var normalize(Kernel&,const var& R);//relative prime condition will not be checked
		inline const var& num(const var& R){
			return R.tuple()[1];
		}
		inline const var& den(const var& R){
			return R.tuple()[2];
		}
		inline var newCRE(const var& a,const var& b){
			return list(a,b,$CRE);
		}
	}
}
