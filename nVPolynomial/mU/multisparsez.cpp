#include "common.h"
/**
\file
\brief 多元多项式基本函数 in Z[x]
*/
#include <mU/Polynomial_impl.h>
// #ifdef DEBUG_MU
// int RESIZE_COUNTER=0;
// int ADD_COUNTER=0;
// #endif
namespace mU {
#ifdef DEBUG_MU
	//uint sparse_z::NUM_OF_SPARSE_Z=0;
	void print_sparse_z(const sparse_z& P){
		//wcerr<<P.id<<" : |";
		for(uint i=0;i<P.size();i++){
			for(uint j=0;j<P[i].exponents.size();j++)
				std::wcerr<<P[i].exponents[j]<<" ";
			std::wcerr<<"-> ";
			print_mpz_ptr(P[i].coe);
			//wcerr<<"( "<<P[i].coe<<" ) ";
			std::wcerr<<"| ";
		}
	}
	void print_mpz_ptr(const __mpz_struct * i){
		char* gbuf = mpz_get_str(0, 10, i);
		std::wcerr << gbuf;
		(*__gmp_free_func)(gbuf, strlen(gbuf) + 1);
	}
#endif

namespace{


int (__cdecl *com_ptr)(const mono_z & f,const mono_z & g);

bool com_for_sort(const mono_z & f,const mono_z & g)
{
	return (com_ptr(f,g)<0);
}

}

void sparse_z::normalize(order_z order)
{
	while(1)
	{
		bool flag=0;
		for(size_t i=0;i<this->monomials.size();i++)
		{
			if(mpz_cmp_ui(this->monomials[i].coe,0)==0)
			{
				mpz_clear(this->monomials[i].coe);
				this->monomials.erase(this->monomials.begin()+i);
				flag=1;
				break;
			}
		}
		if(!flag)break;
	}
	com_ptr=order;
	std::sort(this->monomials.begin(),this->monomials.end(),com_for_sort);
	return ;
}

void sparse_z::resize(size_t n)
{
	size_t m=this->monomials.size();
// #ifdef DEBUG_MU
// 	wcerr<<RESIZE_COUNTER<<" ";
// 	mU::print_sparse_z(*this);
// #endif
	if(n>m)
	{
		this->monomials.resize(n);
		for(size_t i=m;i<n;i++)
		{
			mpz_init(this->monomials[i].coe);
		}
	}
	else if(n<m)
	{
		for(size_t i=n;i<m;i++)
		{
			mpz_clear(this->monomials[i].coe);
		}
		this->monomials.resize(n);
	}
// #ifdef DEBUG_MU
// 	wcerr<<m<<" ==> "<<n<<endl;;
// 	RESIZE_COUNTER++;
// #endif
	return ;
}

void sparse_z::print()
{
	std::cout<<" ( ";
	for(size_t i=0;i<this->monomials.size();i++)
	{
		mpz_out_str(0,10,this->monomials[i].coe);
		std::cout<<"<";
		for(size_t j=0;j<this->monomials[i].exponents.size();j++)
		{
			std::cout<<" "<<this->monomials[i].exponents[j]<<" ";
		}
		std::cout<<">";
		if(i!=this->monomials.size()-1)std::cout<<", ";
	}
	std::cout<<" ) \n";
}

void sparse_z::print() const
{
	std::cout<<" ( ";
	for(size_t i=0;i<this->monomials.size();i++)
	{
		mpz_out_str(0,10,this->monomials[i].coe);
		std::cout<<"<";
		for(size_t j=0;j<this->monomials[i].exponents.size();j++)
		{
			std::cout<<" "<<this->monomials[i].exponents[j]<<" ";
		}
		std::cout<<">";
		if(i!=this->monomials.size()-1)std::cout<<", ";
	}
	std::cout<<" ) \n";
}

void sparse_z::combine_ordered()
{
	size_t i,j,current,sr;
	sr=this->size();
	if(sr==0)
		return;
	i=0;j=0;current=0;
	uint n=this->monomials[0].exponents.size();
	while(1)
	{
		while(j<sr&&equal_mono_z_type((*this)[j],(*this)[i],n))j++;
		if(j-i>1)
		{
			for(current=i+1;current<j;current++)
			{
				mpz_add((*this)[i].coe,(*this)[i].coe,(*this)[current].coe);
			}
			for(current=i+1;current<j;current++)
			{
				mpz_clear((*this)[i+1].coe);
				this->monomials.erase(this->monomials.begin()+(i+1));
			}
			sr=this->size();
		}
		i++;j=i;sr=this->size();
		if(i==sr)break;
	}
}

uint sparse_z::deg(uint variable)
{
	uint deg_temp=0;
	for(size_t i=0;i<this->size();i++)
	{
		if(deg_temp<this->monomials[i].exponents[variable])deg_temp=this->monomials[i].exponents[variable];
	}
	return deg_temp;
}

uint sparse_z::deg(uint variable) const
{
	uint deg_temp=0;
	for(size_t i=0;i<this->size();i++)
	{
		if(deg_temp<this->monomials[i].exponents[variable])deg_temp=this->monomials[i].exponents[variable];
	}
	return deg_temp;
}

void copy_sparse_z(sparse_z & r,const sparse_z & s)
{
	r.resize(s.size());
	for(size_t i=0;i<r.size();i++)
	{
		r[i].exponents=s[i].exponents;
		mpz_set(r[i].coe,s[i].coe);
	}
	return ;
}

void clear_sparse_z_list(std::vector<sparse_z> & x)
{
	for(size_t i=0;i<x.size();i++)
	{
		x[i].resize(0);
	}
	x.resize(0);
}

void sparse_z_to_poly_z(poly_z & r,const sparse_z & s)
{
	if(s.size()==0)
	{
		r.resize(0);
		return ;
	}
	r.resize(s.deg()+1);
	for(size_t i=0;i<r.size();i++)mpz_set_ui(r[i],0);
	for(size_t i=0;i<s.size();i++)
	{
		mpz_set(r[s[i].exponents[0]],s[i].coe);
	}
	return ;
}

void poly_z_to_sparse_z(sparse_z & r, const poly_z & s, uint totalvar, uint variable)
{
	r.resize(s.size());
	for(size_t i=0;i<s.size();i++)
	{
		mpz_set(r[i].coe,s[i]);
		r[i].exponents.resize(totalvar);
		r[i].exponents[variable]=i;
	}
	r.normalize();
	return ;
}

int lex_z(const mono_z & f,const mono_z & g)
{
	for(size_t i=0;i<f.exponents.size();i++)
	{
		if(f.exponents[i]<g.exponents[i])return -1;
		if(f.exponents[i]>g.exponents[i])return 1;
	}
	return 0;
}

bool equal_mono_z_type(const mono_z & f,const mono_z & g,uint n_bound)
{
	for(uint i=0;i<n_bound;i++)
	{
		if(f.exponents[i]!=g.exponents[i])return 0;
	}
	return 1;
}
void MultiAddZ(sparse_z & rr,const sparse_z & f,const sparse_z & g,order_z order)
{
// #ifdef DEBUG_MU
// 	wcerr<<"Add\n";
// 	wcerr<<"f = ";mU::print_sparse_z(f);wcerr<<endl;
// 	wcerr<<"g = ";mU::print_sparse_z(g);wcerr<<endl;
// #endif
	size_t sf,sg,i,j;
	sf=f.size();sg=g.size();
	sparse_z r;
	r.reserve(std::max(sf,sg));
	i=0;j=0;
	while(1)
	{
		if(i==sf)
		{
			for(size_t k=j;k<sg;k++)
			{
				r.resize(r.size()+1);
				r.right().exponents=g[k].exponents;
				mpz_set(r.right().coe,g[k].coe);
			}
			break;
		}
		if(j==sg)
		{
			for(size_t k=i;k<sf;k++)
			{
				r.resize(r.size()+1);
				r.right().exponents=f[k].exponents;
				mpz_set(r.right().coe,f[k].coe);
			}
			break;
		}
		int flag=order(f[i],g[j]);
		if(flag<0)
		{
			r.resize(r.size()+1);
			r.right().exponents=f[i].exponents;
			mpz_set(r.right().coe,f[i].coe);
			i++;
			continue;
		}
		if(flag>0)
		{
			r.resize(r.size()+1);
			r.right().exponents=g[j].exponents;
			mpz_set(r.right().coe,g[j].coe);
			j++;
			continue;
		}
		if(flag==0)
		{
			mpz_t temp;
			mpz_init(temp);
			mpz_add(temp,f[i].coe,g[j].coe);
			if(mpz_sgn(temp)!=0){//not zero
				r.resize(r.size()+1);
				mpz_set(r.right().coe,temp);
				r.right().exponents=f[i].exponents;
			}
			i++;j++;
			continue;
		}
	}
	rr.resize(0);
	rr=r;
}
void MultiNegZ(sparse_z & rr,const sparse_z & f){
	sparse_z r;
	r.resize(f.size());
	for(size_t i=0;i<f.size();i++){
		mpz_neg(r[i].coe,f[i].coe);
		r[i].exponents=f[i].exponents;
	}
	rr.resize(0);
	rr=r;
}

void MultiSubZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order)
{
	sparse_z ng;
	MultiNegZ(ng,g);
	MultiAddZ(r,f,ng,order);
}

void mono_z_mul(mono_z & r,const mono_z & f,const mono_z & g)
{
	mpz_mul(r.coe,f.coe,g.coe);
	size_t sf=f.exponents.size();
	r.exponents.resize(sf);
	for(size_t i=0;i<sf;i++)
	{
		r.exponents[i]=f.exponents[i]+g.exponents[i];
	}
	return ;
}

//make sure f & g should be normalized before being multiplied.
void MultiMulZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order)
{
	size_t sf,sg,i,j,current;
	sparse_z rr;
	sf=f.size();sg=g.size();
	rr.resize(sf*sg);
	if(sf==0||sg==0)
	{
		r.resize(0);return ;
	}
	current=0;
	for(i=0;i<sf;i++)
	{
		for(j=0;j<sg;j++)
		{
			mono_z_mul(rr[current],f[i],g[j]);
			current++;
		}
	}
	rr.normalize(order);
	rr.combine_ordered();

	r.resize(0);
	r=rr;rr=sparse_z();
	return ;
}

void MultiMulZ(sparse_z & r,const sparse_z & f,mpz_ptr a,order_z order)
{
	uint sf=f.size();
	copy_sparse_z(r,f);
	for(uint i=0;i<sf;++i)
	{
		mpz_mul(r[i].coe,f[i].coe,a);
	}
	r.normalize();
}
void MultiPowZ(sparse_z & r,const sparse_z & f,uint n,order_z order)
{
	//r=1
	r.resize(1);
	mpz_set_ui(r[0].coe,1);
	r[0].exponents.resize(f[0].exponents.size());
	for(size_t i=0;i<r[0].exponents.size();i++)r[0].exponents[i]=0;
	
	for(uint i=1;i<=n;i++)
		MultiMulZ(r,r,f,order);
	
	// uint m=n;
	// sparse_z z;
	// copy_sparse_z(z,f);
	// r.resize(1);
	// mpz_set_ui(r[0].coe,1);
	// r[0].exponents=f[0].exponents;
	// for(size_t i=0;i<r[0].exponents.size();i++)r[0].exponents[i]=0;
	// while(1)
	// {
	// 	if(m%2==1)
	// 	{
	// 		MultiMulZ(r,r,z,order);
	// 		MultiMulZ(z,z,z,order);
	// 	}
	// 	else
	// 	{
	// 		MultiMulZ(z,z,z,order);
	// 	}
	// 	m>>=1;
	// 	if(m==0)
	// 	{
	// 		z.resize(0);
	// 		return ;
	// 	}
  // }
}

//make sure f & g should be normalized before being multiplied.
void MultiDivExactZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order)
{
	if(f.size()==0)
	{
		r.resize(0);return ;
	}
	sparse_z rr,rem,t;
	copy_sparse_z(rem,f);
	rr.resize(0);
	uint n=g.size()-1,vars=g[0].exponents.size();
	while(1)
	{
		rr.resize(rr.size()+1);
		mpz_divexact(rr[rr.size()-1].coe,rem[rem.size()-1].coe,g[n].coe);
		rr[rr.size()-1].exponents.resize(vars);
		for(size_t i=0;i<vars;i++)rr[rr.size()-1].exponents[i]=rem[rem.size()-1].exponents[i]-g[n].exponents[i];
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_z_mul(t[i],rr[rr.size()-1],g[i]);
		MultiSubZ(rem,rem,t,order);
		if(rem.size()==0)break;
	}
	reverse(rr.monomials.begin(),rr.monomials.end());
	r.resize(0);r=rr;rr=sparse_z();
	rem.resize(0);t.resize(0);
	return ;
}

bool MultiDivisibleZ(const sparse_z & f,const sparse_z & g,order_z order)
{
	if(f.size()==0)
		return true;
	if(g.size()==0)//f!=0
		return false;
	sparse_z rr,rem,t;
	copy_sparse_z(rem,f);
	uint n=g.size()-1,vars=g[0].exponents.size();
	uint size;
	rr.resize(1);
	rr[0].exponents.resize(vars);
	while(1)
	{
		size=rem.size()-1;
		for(size_t i=0;i<vars;i++)
		{
			if(rem[size].exponents[i]<g[n].exponents[i])
			{
				rr.resize(0);rem.resize(0);t.resize(0);
				return 0;
			}
			rr[0].exponents[i]=rem[size].exponents[i]-g[n].exponents[i];
		}
		if(mpz_divisible_p(rem[size].coe,g[n].coe)==0)
		{
			rr.resize(0);rem.resize(0);t.resize(0);
			return 0;
		}
		mpz_divexact(rr[0].coe,rem[size].coe,g[n].coe);
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_z_mul(t[i],rr[0],g[i]);
		MultiSubZ(rem,rem,t,order);
		if(rem.size()==0)break;
	}
	rr.resize(0);rem.resize(0);t.resize(0);
	return 1;
}

void MultiContZ(mpz_ptr r, const sparse_z & f)
{
	if(f.size()==0)
	{
		mpz_set_ui(r,1);return ;
	}
	uint sf=f.size();
	bool sign=(mpz_cmp_ui(f[sf-1].coe,0)>0);
	mpz_set(r,f[0].coe);
	if(mpz_cmp_ui(r,1)==0)
	{
		if(!sign)mpz_neg(r,r);
		return ;
	}
	for(uint i=1;i<sf;++i)
	{
		mpz_gcd(r,r,f[i].coe);
		if(mpz_cmp_ui(r,1)==0)
		{
			if(!sign)mpz_neg(r,r);
			return ;
		}
	}
	if(!sign)mpz_neg(r,r);
	return ;
}

void MultiPPZ(sparse_z & r, const sparse_z & f)
{
	static mpz_t contf;
	mpz_init(contf);
	MultiContZ(contf,f);
	uint sf=f.size();
	copy_sparse_z(r,f);
	for(uint i=0;i<sf;++i)
	{
		mpz_divexact(r[i].coe,r[i].coe,contf);
	}
	mpz_clear(contf);
	return ;
}

void MultiContPPZ(mpz_ptr cont, sparse_z & pp, const sparse_z & f)
{
	MultiContZ(cont,f);
	uint sf=f.size();
	copy_sparse_z(pp,f);
	for(uint i=0;i<sf;++i)
	{
		mpz_divexact(pp[i].coe,pp[i].coe,cont);
	}
	return ;
}

void MultiMaxNormZ(mpz_ptr r, const sparse_z & f)
{
	static mpz_t fc;
	mpz_init(fc);
	mpz_set_ui(r,0);
	uint sf=f.size();
	for(uint i=0;i<sf;++i)
	{
		mpz_abs(fc,f[i].coe);
		if(mpz_cmp(r,fc)<0)mpz_set(r,fc);
	}
	mpz_clear(fc);
	return ;
}

void MultiPolynomialMod(sparse_z & r,const sparse_z & f,mpz_ptr p,order_z order)
{
	if(f.size()==0)
	{
		r.resize(0);
		return ;
	}
	uint sf=f.size();
	mpz_t p2;
	mpz_init(p2);
	mpz_div_2exp(p2,p,1);
	r.resize(sf);
	for(uint i=0;i<sf;++i)
	{
		mpz_mod(r[i].coe,f[i].coe,p);
		if(mpz_cmp(r[i].coe,p2)>0)
		{
			mpz_sub(r[i].coe,r[i].coe,p);
		}
		r[i].exponents=f[i].exponents;
	}
	mpz_clear(p2);
	r.normalize(order);
	return ;
}

//make sure f & g should be normalized before being multiplied.
void MultiDivExactZp(sparse_z & r,const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order)
{
	if(f.size()==0)
	{
		r.resize(0);return ;
	}
	sparse_z rr,rem,t;
	copy_sparse_z(rem,f);
	rr.resize(0);
	uint n=g.size()-1,vars=g[0].exponents.size();
	mpz_t inverse;
	mpz_init(inverse);
	mpz_invert(inverse,g[n].coe,p);
	while(1)
	{
		rr.resize(rr.size()+1);
		mpz_mul(rr[rr.size()-1].coe,rem[rem.size()-1].coe,inverse);
		mpz_mod(rr[rr.size()-1].coe,rr[rr.size()-1].coe,p);
		rr[rr.size()-1].exponents.resize(vars);
		for(size_t i=0;i<vars;i++)rr[rr.size()-1].exponents[i]=rem[rem.size()-1].exponents[i]-g[n].exponents[i];
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_z_mul(t[i],rr[rr.size()-1],g[i]);
		MultiSubZ(rem,rem,t,order);
		MultiPolynomialMod(rem,rem,p,order);
		if(rem.size()==0)break;
	}
	r.resize(0);r=rr;rr=sparse_z();
	rem.resize(0);t.resize(0);
	mpz_clear(inverse);
	MultiPolynomialMod(r,r,p);
	return ;
}

bool MultiDivisibleZp(const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order)
{
	if(f.size()==0)
		return 1;
	if(g.size()==0)
		return 0;
	sparse_z rr,rem,t;
	copy_sparse_z(rem,f);
	uint n=g.size()-1,vars=g[0].exponents.size();
	uint size;
	mpz_t inverse;
	mpz_init(inverse);
	mpz_invert(inverse,g[n].coe,p);
	rr.resize(1);
	rr[0].exponents.resize(vars);
	while(1)
	{
		size=rem.size()-1;
		for(size_t i=0;i<vars;i++)
		{
			if(rem[size].exponents[i]<g[n].exponents[i])
			{
				rr.resize(0);rem.resize(0);t.resize(0);
				mpz_clear(inverse);
				return 0;
			}
			rr[0].exponents[i]=rem[size].exponents[i]-g[n].exponents[i];
		}
		mpz_mul(rr[0].coe,rem[size].coe,inverse);
		mpz_mod(rr[0].coe,rr[0].coe,p);
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_z_mul(t[i],rr[0],g[i]);
		MultiSubZ(rem,rem,t,order);
		MultiPolynomialMod(rem,rem,p,order);
		if(rem.size()==0)break;
	}
	rr.resize(0);rem.resize(0);t.resize(0);
	mpz_clear(inverse);
	return 1;
}

void to_main_variable(std::vector<sparse_z> & f,const sparse_z & g,uint variable)
{
	/* ??? g=(0)->1 --> f={ ()->1 } ,Is this handled properly? YES
	 */
	uint degx,size;
	clear_sparse_z_list(f);
	f.resize(g.deg(variable)+1);
	for(size_t i=0;i<g.size();i++)
	{
		degx=g[i].exponents[variable];
		size=f[degx].size();
		f[degx].resize(size+1);
		mpz_set(f[degx][size].coe,g[i].coe);
		f[degx][size].exponents=g[i].exponents;
		f[degx][size].exponents.erase(f[degx][size].exponents.begin()+variable);
	}
	return ;
}

void from_main_variable(sparse_z & f,const std::vector<sparse_z> & g,uint variable)
{
	f.resize(0);
	uint size;
	for(uint i=0;i<g.size();i++)
	{
		for(uint j=0;j<g[i].size();j++)
		{
			size=f.size();
			f.resize(size+1);
			mpz_set(f[size].coe,g[i][j].coe);
			f[size].exponents=g[i][j].exponents;
			f[size].exponents.insert(f[size].exponents.begin()+variable,1,i);
		}
	}
	f.normalize();
	return ;
}

void eval_in_one_variable_z(sparse_z & r,const sparse_z & f,mpz_ptr x,uint variable)
{
	// DEBUG:: value of x is variable
	std::vector<sparse_z> fc;
	to_main_variable(fc,f,variable);
	uint sf=fc.size();
	uint i=sf-1;
	copy_sparse_z(r,fc[i]);
	if(i==0)
	{
		clear_sparse_z_list(fc);
		return ;
	}
	--i;
	while(1)
	{
		MultiMulZ(r,r,x);
		MultiAddZ(r,r,fc[i]);
		if(i==0)
		{
			clear_sparse_z_list(fc);
			return ;
		}
		--i;
	}
	clear_sparse_z_list(fc);
	return ;
}

}
