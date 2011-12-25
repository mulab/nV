#include "common.h"
#include <mU/Polynomial_impl.h>
#include "polyfloat.h"
#include "wrap.h"

namespace mU {
//////////////////////////////////////

	var tag_modulus,tag_cyclotomic,tag_root;

	void to_poly_q(poly_q & f,Var g)
	{
		size_t n=Size(g);
		f.resize(n);
		static var unit=Int(1L);
		for(size_t i=0;i<n;i++)
		{
			if(IntQ(At(g,i)))
			{
				mpq_set_num(f[i],CInt(At(g,i)));
				mpq_set_den(f[i],CInt(unit));
			} else if (RatQ(At(g,i))) {
				mpq_set(f[i],CRat(At(g,i)));
			} else {
				wcerr << _W("g[") << i << _W("] ");
				Print(At(g,i), wcerr);
				wcerr << _W(" is a Integer or Rational!") << std::endl;
				mpq_set_num(f[i], CInt(unit));
				mpq_set_den(f[i], CInt(unit));
			}
		}
		return ;
	}

	void from_poly_q(var & f, const poly_q & g)
	{
		size_t n=g.size();
		f=Vec();
		Resize(f,n);
		for(size_t i=0;i<n;i++)
		{
			At(f,i)=Rat();
			mpq_set(CRat(At(f,i)),g[i]);
		}
		return ;
	}

	void to_poly_z(poly_z &f, Var g)
	{
		size_t n = Size(g);
		f.resize(n);
		for(size_t i = 0; i < n; ++i)
		{
			mpz_set(f[i], CInt(At(g,i)));
		}
	}

	void from_poly_z(var & f, const poly_z& g)
	{
		size_t n = g.size();
		f=Vec();
		Resize(f,n);
		for(size_t i = 0; i < n; ++i)
		{
			At(f,i) = Int();
			mpz_set(CInt(At(f,i)), g[i]);
		}
	}

	//make sure r should be empty before converting, indices should be resized to n at first
	//don't forget to normalize r after this proceed
	void to_sparse_z(sparse_z & r,Var s,uint n,std::vector<uint> & indices)
	{
		if(n==0)
		{
			r.resize(r.size()+1);
			mpz_set(r[r.size()-1].coe,CInt(s));
			r[r.size()-1].exponents=indices;
			std::reverse(r[r.size()-1].exponents.begin(),r[r.size()-1].exponents.end());
			return ;
		}
		for(uint i=0;i<Size(s);i++)
		{
			indices[n-1]=i;
			to_sparse_z(r,At(s,i),n-1,indices);
		}
		if(n==indices.size())
		{
			r.normalize();
		}
		return ;
	}

	void from_sparse_z(var & s,const sparse_z & r,uint n,uint index)
	{
		if(index==n)
		{
			s=Int();
			if(r.size()==0)
			{
				mpz_set_ui(CInt(s),0);
				return ;
			}
			mpz_set(CInt(s),r[0].coe);
			return ;
		}
		uint degx=r.deg(index);
		uint size=r.size();
		uint i_x;
		std::vector<sparse_z> ri;
		ri.resize(degx+1);
		s=Vec();
		Resize(s,degx+1);
		for(uint i=0;i<size;i++)
		{
			i_x=r[i].exponents[index];
			ri[i_x].resize(ri[i_x].size()+1);
			mpz_set(ri[i_x][ri[i_x].size()-1].coe,r[i].coe);
			ri[i_x][ri[i_x].size()-1].exponents=r[i].exponents;
		}
		for(uint i=0;i<=degx;i++)
		{
			from_sparse_z(At(s,i),ri[i],n,index+1);
		}
		clear_sparse_z_list(ri);
		return ;
	}

	void to_sparse_q(sparse_q & r,Var s,uint n,std::vector<uint> & indices)
	{
		if(n==0)
		{
			r.resize(r.size()+1);
			if(IntQ(s))
			{
				mpq_set_num(r[r.size()-1].coe,CInt(s));
				mpq_set_den(r[r.size()-1].coe,z_unit);
			}
			else
			{
				mpq_set(r[r.size()-1].coe,CRat(s));
			}
			r[r.size()-1].exponents=indices;
			std::reverse(r[r.size()-1].exponents.begin(),r[r.size()-1].exponents.end());
			return ;
		}
		for(uint i=0;i<Size(s);i++)
		{
			indices[n-1]=i;
			to_sparse_q(r,At(s,i),n-1,indices);
		}
		if(n==indices.size())
		{
			r.normalize();
		}
		return ;
	}

	void from_sparse_q(var & s,const sparse_q & r,uint n,uint index)
	{
		if(index==n)
		{
			s=Rat();
			if(r.size()==0)
			{
				mpq_set_ui(CRat(s),0,1);
				return ;
			}
			mpq_set(CRat(s),r[0].coe);
			return ;
		}
		uint degx=r.deg(index);
		uint size=r.size();
		uint i_x;
		std::vector<sparse_q> ri;
		ri.resize(degx+1);
		s=Vec();
		Resize(s,degx+1);
		for(uint i=0;i<size;i++)
		{
			i_x=r[i].exponents[index];
			ri[i_x].resize(ri[i_x].size()+1);
			mpq_set(ri[i_x][ri[i_x].size()-1].coe,r[i].coe);
			ri[i_x][ri[i_x].size()-1].exponents=r[i].exponents;
		}
		for(uint i=0;i<=degx;i++)
		{
			from_sparse_q(At(s,i),ri[i],n,index+1);
		}
		clear_sparse_q_list(ri);
		return ;
	}
//////////////////////////////////////
}//namespace mU
