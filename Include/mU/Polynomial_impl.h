#pragma once
#include <gmp.h>
#include <ctime>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cmath>
#ifdef _MSC_VER
#pragma comment(lib,"gmp")
#endif

#undef API
#undef IMPORT
#ifdef _MSC_VER
#define IMPORT __declspec(dllimport)
#ifdef POLYNOMIAL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#define IMPORT
#endif

#ifndef _MSC_VER
#ifdef __amd64__
#define __cdecl
#else
#define __cdecl __attribute__((cdecl))
#endif
#endif

#undef IMPORT
#define IMPORT

#ifndef _WIN32
#define DEBUG_MU
#endif
#ifdef DEBUG_MU
#include <gmp.h>
#include <cstring>
using std::wcerr;
using std::endl;
extern void (*__gmp_free_func)(void*, size_t);
#endif

namespace mU {
#ifndef _WIN32
#define uint another_uint_type
#endif
	typedef unsigned long uint;
	
	IMPORT extern mpz_t z_unit,z_zero;
	IMPORT extern mpq_t q_unit,q_zero;
	//////////////////////////////////////
//functions of univariate polynomials
//polyrandom.cpp
	class random
	{
	public:
		static gmp_randstate_t rstate;
		API static void randomize();
		static void randominteger(mpz_ptr r,mpz_ptr a,mpz_ptr b);
		static void randominteger(int & r,int a,int b);
	};

//uniarithmetic.cpp
	struct poly_z : public std::vector<mpz_ptr>
	{
		API void resize(size_t n);
		int deg();
		void normalize();
		mpz_ptr* elts();
		mpz_ptr const* elts() const;
		void print();
		void print() const;
	};
	API bool is_x_power_z(const poly_z & f);
	API void copy_poly_z(poly_z & r,const poly_z & f);
	API void poly_z_dform(poly_z & r,const poly_z & f);
	API void UniNegZ(poly_z & r,const poly_z & f);
	API void UniMulZ(poly_z & r,const poly_z & f, const poly_z & g);
	API void UniMulZp(poly_z & r,const poly_z & f, const poly_z & g,mpz_ptr p);
	API void UniAddZ(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniAddZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniSubZ(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniSubZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniDivExactZ(poly_z & q,const poly_z & f,const poly_z & g);
	API void UniReExactZ(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniPseudoModZ(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniEvalZ(mpz_ptr r,const poly_z & f,mpz_ptr x);
	API void UniEvalZp(mpz_ptr r,const poly_z & f,mpz_ptr x,mpz_ptr p);
	API void UniPolynomialMod(poly_z & r,const poly_z & f,mpz_ptr m);
	API void UniDivModZp(poly_z & q,poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniDivZp(poly_z & q,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniModZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API bool poly_z_divisible(const poly_z & f,const poly_z & g);
	API void resize_z_list(std::vector<mpz_ptr> & r,uint n);
	API void clear_poly_z_list(std::vector<poly_z> & r);
	class poly_q
	{
	public:
		inline uint size() {return this->rep.size();}
		inline uint size() const {return this->rep.size();}
		inline mpq_ptr & operator [](uint i) {return (mpq_ptr &)this->rep[i];}
		inline const mpq_ptr & operator [](uint i) const {return (const mpq_ptr &)this->rep[i];}
		void resize(uint n);
		void normalize();
		void print();
		void print() const;
	private:
		std::vector<mpq_ptr> rep;
	};
	API bool is_x_power_q(const poly_q & f);
	API void x_power_q(poly_q & r,uint n);
	API void poly_z_to_poly_q(poly_q & r,const poly_z & f);
	API void poly_q_to_poly_z(poly_z & r,const poly_q & f);
	API void poly_q_dform(poly_q & r,const poly_q & f);
	API void poly_q_simpleform(mpq_ptr a,poly_z & r,const poly_q & f);
	API void poly_q_simpleform(mpq_ptr a,poly_q & r,const poly_q & f);
	API void copy_poly_q(poly_q & r,const poly_q & f);
	API void clear_poly_q_list(std::vector<poly_q> & r);
	API void UniNegQ(poly_q & r,const poly_q & f);
	API void UniEvalQ(mpq_ptr r,const poly_q & f,mpq_ptr x);
	API void UniMulQ(poly_q & r, const poly_q & f, const poly_q & g);
	API void UniDivModQ(poly_q & q,poly_q & r,const poly_q & f,const poly_q & g);
	API void UniDivQ(poly_q & q,const poly_q & f,const poly_q & g);
	API void resize_q_list(std::vector<mpq_ptr> & r,uint n);

//unigcd.cpp
	API void IntegerLength_BigBase(mpz_ptr r,mpz_ptr n,mpz_ptr b);
	API void UniMaxNormZ(mpz_ptr r,const poly_z & f);
	API void UniOneNormZ(mpz_ptr r,const poly_z & f);
	API void UniContZ(mpz_ptr r,const poly_z & f);
	API void UniPPZ(poly_z & r,const poly_z & f);
	API void UniGcdZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniGcdZp_Ext(poly_z & r,poly_z & s,poly_z & t,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniGcdZ_SmallPrime1(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniGcdZ(poly_z & r,const poly_z & f,const poly_z & g);
	API void UniLcmZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniLcmZ(poly_z & r,const poly_z & f,const poly_z & g);	

//unifaczp.cpp
	API void Power(poly_z & f,uint n,mpz_ptr p);
	API void PowerMod(poly_z & f,mpz_ptr n,const poly_z & g,mpz_ptr p);
	API void PowerMod_Pth_Power(poly_z & f,const poly_z & g,mpz_ptr p);
	API void UniDisDegFacZp(std::vector<poly_z> & disfactorlist, std::vector<uint> & deglist,const poly_z & f,mpz_ptr p);
	API void UniRandomZp(poly_z & r,uint deg_bound,mpz_ptr p);
	API void UniEqlDegFacZp(std::vector<poly_z> & eqlfactorlist,const poly_z & f,mpz_ptr p,uint d);
	API void UniFacZp_fsqfmonic(std::vector<poly_z> & faclist,const poly_z & f,mpz_ptr p);
	API void UniSqrFreeDecomZp(const poly_z & f,std::vector<poly_z> & sqflist,mpz_ptr p);
	API void UniFacZp(const poly_z & f,mpz_ptr p,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents);

//unismallprime.cpp
	API void BerlekampZp_SmallPrime(const poly_z & f,mpz_ptr p,std::vector<poly_z> & faclist);

//unifacz.cpp
	API void UniSqrFreeDecomZ(const poly_z & f,std::vector<poly_z> & sqflist);
	API void UniFacZ_Hensel_FactorCombination(const poly_z & f,std::vector<poly_z> & faclist);
	API void UniFacZ(const poly_z & f,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents);

//unicyclotomic.cpp
	API void UniCyclotomicZ(poly_z & r,uint n);
	API uint UniCyclotomicZQ_InversePhi(const poly_z & f);
	API uint UniShiftedCyclotomicZQ(poly_z & r,const poly_z & f);

//uniexactsolution.cpp
	API void UniZpRootZp(std::vector<mpz_ptr> & rootlist,const poly_z & f,mpz_ptr p);
	API void UniZRootZ_ByZp(std::vector<mpz_ptr> & rootlist,const poly_z & f);
	API void UniZRootZ_ByFactor(std::vector<mpz_ptr> & rootlist,const poly_z & f);
	API void UniZRootZ_BySeparation(std::vector<mpz_ptr> & rootlist,const poly_z & f);
	API void UniZRootZ(std::vector<mpz_ptr> & rootlist, const poly_z & f);

//unidecompose.cpp
	API void UniNthRootQ_Euler(poly_q & r,const poly_q & g,mpq_ptr a,uint s);
	API void UniReversalQ(poly_q & r,const poly_q & s,uint n);
	API void UniFuncionalDecompositionQ(std::vector<poly_q> & partlist,const poly_q & f);
	API void UniFuncionalDecompositionZ(mpq_ptr a,std::vector<poly_z> & partlist,const poly_z & f);

//unirootseparation.cpp
	class interval_q
	{
	public:
		mpq_t first,second;
		void init() {mpq_init(first);mpq_init(second);};
		void destroy() {mpq_clear(first);mpq_clear(second);};
		friend bool operator < (const interval_q & f, const interval_q & g)
		{
			int c1=mpq_cmp(f.first,g.first),c2=mpq_cmp(f.second,g.second);
			return c1<0||(c1==0&&c2<0);
		}
		void print()
		{
			std::cout<<"[";
			mpq_out_str(0,10,first);
			std::cout<<",";
			mpq_out_str(0,10,second);
			std::cout<<"]\n";
		}
	};
	API void clear_inverval_q_list(std::vector<interval_q> &x);
	API void UniUpperBoundOfRootZ(mpz_ptr b, const poly_z & f);
	API void UniSturmSequenceZ(std::vector<poly_z> & sturm_seq, const poly_z & f);
	API uint UniSturmChangeSignZ(std::vector<poly_z> & sturm_seq, mpz_ptr x);
	API void UniRealRootSeparationZ(std::vector<interval_q> & intervallist, const poly_z & f);
	API void UniRealRootSeparationZ_CF(std::vector<interval_q> & intervallist, const poly_z & f);

//////////////////////////////////////
//functions of multivariate polynomials
//multisparsez.cpp
	class mono_z
	{
	public:
		mpz_t coe;
		std::vector<uint> exponents;
	};
	typedef int (__cdecl *order_z)(const mono_z &,const mono_z &);
	API int lex_z(const mono_z & f,const mono_z & g);
	API bool equal_mono_z_type(const mono_z & f,const mono_z & g,uint n_bound);
//to do: to separate deleting 0 terms and sorting in normalizing
	class sparse_z
	{
	public:
		void normalize(order_z order=lex_z);
		inline uint size(){return monomials.size();}
		inline uint size() const {return monomials.size();}
		API void resize(size_t n);
		inline mono_z & right(){return (mono_z &)this->monomials[this->monomials.size()-1];}
		inline mono_z & right() const {return (mono_z &)this->monomials[this->monomials.size()-1];}
		inline mono_z & operator [](uint i) {return (mono_z &)this->monomials[i];}
		inline const mono_z & operator [](uint i) const {return (const mono_z &)this->monomials[i];}
		inline void reserve(uint n){monomials.reserve(n);}
		void combine_ordered();
		void print();
		void print() const;
		uint deg(uint variable=0);
		uint deg(uint variable=0) const;
#ifdef DEBUG_MU
		// sparse_z(){
		// 	id=NUM_OF_SPARSE_Z;
		// 	NUM_OF_SPARSE_Z++;
		// 	std::wcerr<<"sparse_z "<<id<<" created\n";
		// }
		// static uint NUM_OF_SPARSE_Z;
		// uint id;
#endif
		std::vector<mono_z> monomials;
	};
#ifdef DEBUG_MU
	void print_sparse_z(const sparse_z& P);
	void print_mpz_ptr(const __mpz_struct * i);
#endif
	API void copy_sparse_z(sparse_z & r,const sparse_z & s);
	API void clear_sparse_z_list(std::vector<sparse_z> & x);
	API void sparse_z_to_poly_z(poly_z & r,const sparse_z & s);
	API void poly_z_to_sparse_z(sparse_z & r, const poly_z & s, uint totalvar, uint variable);
	API void mono_z_mul(mono_z & r,const mono_z & f,const mono_z & g);
	API void MultiAddZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
	API void MultiSubZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
	API void MultiMulZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
	API void MultiMulZp(sparse_z & r, const sparse_z & f,const sparse_z & g,mpz_ptr p);
	API void MultiMulZ(sparse_z & r,const sparse_z & f,mpz_ptr a,order_z order=lex_z);
	API void MultiPowZ(sparse_z & r,const sparse_z & f,uint n,order_z order=lex_z);
	API void MultiDivExactZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
	API bool MultiDivisibleZ(const sparse_z & f,const sparse_z & g,order_z order=lex_z);
	API void MultiContZ(mpz_ptr r, const sparse_z & f);
	API void MultiPPZ(sparse_z & r, const sparse_z & f);
	API void MultiContPPZ(mpz_ptr cont, sparse_z & pp, const sparse_z & f);
	API void MultiMaxNormZ(mpz_ptr r, const sparse_z & f);
	API void MultiPolynomialMod(sparse_z & r,const sparse_z & f,mpz_ptr p,order_z order=lex_z);
	API void MultiDivExactZp(sparse_z & r,const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order=lex_z);
	API bool MultiDivisibleZp(const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order=lex_z);

	API void to_main_variable(std::vector<sparse_z> & f,const sparse_z & g,uint variable);
	API void from_main_variable(sparse_z & f,const std::vector<sparse_z> & g,uint variable);
	API void eval_in_one_variable_z(sparse_z & r,const sparse_z & f,mpz_ptr x,uint variable);

//multisparseq.cpp
	class mono_q
	{
	public:
		mpq_t coe;
		std::vector<uint> exponents;
	};
	typedef int (__cdecl *order_q)(const mono_q &,const mono_q &);
	API int lex_q(const mono_q & f,const mono_q & g);
	API bool equal_mono_q_type(const mono_q & f,const mono_q & g,uint n_bound);
//to do: to separate deleting 0 terms and sorting in normalizing
	/* !!!!!!!!!!!!!!!!!!!!!!!NOTE !!!!!!!!!!!!!!!!!!!!!!!!
		The constructor and destructor of sparse_q does not handle the memory management
		 of mpq_t.This management is handled by resize

		 So y=x does not copy real content but only pointers.Note not to change x if you do not
		 want to change y after the copy;
	 */
	class sparse_q
	{
	public:
		void normalize(order_q order=lex_q);
		inline uint size() {return monomials.size();}
		inline uint size() const {return monomials.size();}
		void resize(size_t n);
		inline mono_q & right(){return (mono_q &)this->monomials[this->monomials.size()-1];}
		inline mono_q & right() const {return (mono_q &)this->monomials[this->monomials.size()-1];}
		inline mono_q & operator [](uint i) {return (mono_q &)this->monomials[i];}
		inline const mono_q & operator [](uint i) const {return (const mono_q &)this->monomials[i];}
		void combine_ordered();
		void print();
		void print() const;
		uint deg(uint variable=0);
		uint deg(uint variable=0) const;
		void erase(uint i);
		inline void reserve(uint n){monomials.reserve(n);}
		std::vector<mono_q> monomials;
	};
	API void copy_sparse_q(sparse_q & r,const sparse_q & s);
	API void clear_sparse_q_list(std::vector<sparse_q> & x);
	API void mono_q_mul(mono_q & r,const mono_q & f,const mono_q & g);
	API void MultiAddQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
	API void MultiSubQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
	API void MultiMulQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
	API void MultiMulQ(sparse_q & r,const sparse_q & f,mpq_ptr a,order_q order=lex_q);
	API void MultiDivQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
	API bool MultiDivisibleQ(const sparse_q & f,const sparse_q & g,order_q order=lex_q);
	API void MultiSimpleFormQ(mpq_ptr a, sparse_q & r, const sparse_q & f);

	API void to_main_variable(std::vector<sparse_q> & f,const sparse_q & g,uint variable);
	API void from_main_variable(sparse_q & f,const std::vector<sparse_q> & g,uint variable);

//multirss.cpp

//multigcdzp.cpp
	API void MultiGcdZp_Dense_Interpolation(sparse_z & r,const sparse_z & f, const sparse_z & g,uint n, mpz_ptr p);
	API void MultiGcdZp(sparse_z & r,const sparse_z & f, const sparse_z & g, mpz_ptr p);
	API void MultiLcmZp(sparse_z & r,const sparse_z & f, const sparse_z & g, mpz_ptr p);

//multigcdz.cpp
	API void MultiGcdZ_Dense_Interpolation(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n);
	API void MultiGcdZ_sparse2(sparse_z & r, const sparse_z & skel, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint Tk, uint k, uint D);
	API void MultiGcdZ_Sparse1(sparse_z & r, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint k, uint D);
	API void MultiGcdZ_Sparse(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n);
	API void MultiGcdZ(sparse_z & r, const sparse_z & f,const sparse_z & g);
	API void MultiLcmZ(sparse_z & r, const sparse_z & f,const sparse_z & g);

//multiwu.cpp
	API void CharacterSetZ(std::vector<sparse_z> & cs, const std::vector<sparse_z> & polylist, uint totalvar);
	API void ReduceByAscendingSet(sparse_z & r,const sparse_z & f,const std::vector<sparse_z> & polylist, uint totalvar);

//multigroebner.cpp
	API void MultiGroebnerBasisQ(std::vector<sparse_q> & gb, const std::vector<sparse_q> & polylist, uint totalvar, order_q order=lex_q);
}
