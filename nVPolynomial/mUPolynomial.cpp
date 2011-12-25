#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>
#include "mU/wrap.h"
#include "mU/polyfloat.h"

/**
\file
\brief 整系数一元多项式因子分解
\todo 特别地，要加上不可约性的检测，快速分解诸如x^10000-2; x^n-1型多项式的分解
*/

namespace mU {
	
/**
\brief 整系数多项式因子分解.
\param f 整系数一元多项式.
\return f的分解结果.
*/
var UniFacZ(Var f)
{
	var result=Vec();
	poly_z fz;
	std::vector<poly_z> faclist;
	std::vector<uint> exponents;
	var b = Int();
	to_poly_z(fz,f);
	UniFacZ(fz,CInt(b),faclist,exponents);
	Push(result,b);
	var factor;
	for(size_t i=0;i<faclist.size();i++)
	{
		factor=Vec();
		from_poly_z(factor,faclist[i]);
		Push(result,Vec(factor,Int(exponents[i])));
	}
	clear_poly_z_list(faclist);
	return result;
}

var UniFacZp(Var f,Var p)
{
	var result=Vec();
	poly_z fz;
	std::vector<poly_z> faclist;
	std::vector<uint> exponents;
	static mpz_t p_z;
	mpz_init(p_z);
	mpz_set(p_z,CInt(p));
	var b = Int();
	to_poly_z(fz,f);
	UniFacZp(fz,p_z,CInt(b),faclist,exponents);
	Push(result,b);
	var factor;
	for(size_t i=0;i<faclist.size();i++)
	{
		factor=Vec();
		from_poly_z(factor,faclist[i]);
		Push(result,Vec(factor,Int(exponents[i])));
	}
	clear_poly_z_list(faclist);
	mpz_clear(p_z);
	return result;
}

}

using namespace mU;

CAPI void mUInstall()
{
	static bool Initialized = false;
	if(Initialized) return;
	
	mpz_init(z_unit);mpz_set_ui(z_unit,1);
	mpz_init(z_zero);mpz_set_ui(z_zero,0);
	mpq_init(q_unit);mpq_set_ui(q_unit,1,1);
	mpq_init(q_zero);mpq_set_ui(q_zero,0,1);
	tag_cyclotomic=Sym(_W("Cyclotomic"),System);
	tag_modulus=Sym(_W("Modulus"),System);
	tag_root=Sym(_W("Root"),System);

	mU::random::randomize();
	UniSolveInitialize();
	ComplexInitialize();

	Initialized = true;
}
CAPI void mUUninstall() {
	wcout << "#Polynomial# Uninstall Successfully!" << std::endl;
}

CPROC_ATOMIC(System_RootIntervals)
{
	//rootintervals
	var f_var=At(x,0);
	var v_var=Variables(f_var);
	var result_var=Vec();
	v_var=At(v_var,0);
	f_var=Coefficients(Expand(f_var),v_var);
	poly_z f;
	to_poly_z(f,f_var);
	std::vector<interval_q> intervallist;
	UniRealRootSeparationZ_CF(intervallist,f);
	Resize(result_var,intervallist.size());
	for(mU::uint i=0;i<intervallist.size();++i)
	{
		At(result_var,i)=Vec(Rat(),Rat());
		mpq_set(CRat(At(At(result_var,i),0)),intervallist[i].first);
		mpq_set(CRat(At(At(result_var,i),1)),intervallist[i].second);
	}
	clear_inverval_q_list(intervallist);
	return result_var;
}

CPROC_ATOMIC(System_GroebnerBasis)
{
	var polylist_var=At(x,0);
	var v_var;
	if(Size(x)>1)
	{
		v_var=At(x,1);
	}
	else
	{
		v_var=Variables(polylist_var);
	}
	var result_var=Vec();
	var temp;
	std::vector<sparse_q> polylist,gb;
	mU::uint vars=Size(v_var);
	std::vector<mU::uint> indices(vars,0);
	polylist.resize(Size(polylist_var));
	for(mU::uint i=0;i<Size(polylist_var);++i)
	{
		temp=CoefficientList(Expand(At(polylist_var,i)),v_var);
		to_sparse_q(polylist[i],temp,vars,indices);
	}
	MultiGroebnerBasisQ(gb,polylist,vars);
	Resize(result_var,gb.size());
	for(mU::uint i=0;i<gb.size();++i)
	{
		from_sparse_q(temp,gb[i],vars,0);
		At(result_var,i)=FromCoefficientList(temp,v_var);
	}
	clear_sparse_q_list(polylist);
	clear_sparse_q_list(gb);
	return result_var;
}

CPROC_ATOMIC(System_ZRoot)
{
	var f=At(x,0);
	var v=Variables(f);
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_z fpoly;
	var result=Vec();
	to_poly_z(fpoly,f);
	std::vector<mpz_ptr> rootlist;
	UniZRootZ(rootlist,fpoly);
	Resize(result,rootlist.size());
	for(mU::uint i=0;i<rootlist.size();++i)
	{
		At(result,i)=Int();
		mpz_set(CInt(At(result,i)),rootlist[i]);
	}
	resize_z_list(rootlist,0);
	fpoly.resize(0);
	return result;
}

CPROC_ATOMIC(System_Solve)
{
	var f=At(x,0);
	var v;
	if (Size(x) == 1)
		v = Variables(f);
	else {
		v =At(x,1);
		if (!VecQ(v))
			v = Vec(v);
	}
	var result;
	if(Size(v)!=1)//only work for Z[x]
		return result;
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_z fpoly;
	to_poly_z(fpoly,f);
	UniSolve_Factorization_Stage(result,fpoly);
	mU::uint size=Size(result);
	for(mU::uint i=0;i<size;++i)
	{
		At(result,i)=Vec(Ex(tag_Rule,Vec(v,At(result,i))));
	}
	fpoly.resize(0);
	return result;
}

CPROC_ATOMIC_INT(System_UniGcdZ, 2)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniGcdZ(h,f,g);
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniGcdZp, 3)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniGcdZp(h,f,g,CInt(At(x,2)));
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniLcmZ, 2)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniLcmZ(h,f,g);
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniLcmZp, 3)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniLcmZp(h,f,g,CInt(At(x,2)));
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniGcdZpExt, 3)
{
	var result=Vec();
	poly_z f,g,s,t,r;
	var rr,ss,tt;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniGcdZp_Ext(r,s,t,f,g,CInt(At(x,2)));
	from_poly_z(rr,r);
	from_poly_z(ss,s);
	from_poly_z(tt,t);
	Push(result,rr);
	Push(result,ss);
	Push(result,tt);
	return result;
}
CPROC_ATOMIC_INT(System_UniDivModZp, 3)
{
	var result=Vec();
	poly_z f,g,q,r;
	var qq,rr;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniDivModZp(q,r,f,g,CInt(At(x,2)));
	from_poly_z(qq,q);
	from_poly_z(rr,r);
	Push(result,qq);
	Push(result,rr);
	return result;
}
CPROC_ATOMIC_INT2(System_PolynomialGCD, 2, 3)
{
	var result;
	var f_var=At(x,0),g_var=At(x,1);
	var v_var=Variables(Vec(f_var,g_var));
	mU::uint totalvar=Size(v_var);
	if(totalvar == 0) {
		return Eval(Ex(Sym(__W("GCD"), System), x));
	}
	if(totalvar==1)
	{
		f_var=Coefficients(Expand(f_var),At(v_var,0));
		g_var=Coefficients(Expand(g_var),At(v_var,0));
	}
	else
	{
		f_var=CoefficientList(Expand(f_var),v_var);
		g_var=CoefficientList(Expand(g_var),v_var);
	}
	if(Size(x)==3)
	{
		var rule=At(x,2);
		if(Head(rule)==tag_Rule&&At(Body(rule),0)==tag_modulus)
		{
			var p=At(Body(rule),1);
			if(mpz_cmp_ui(CInt(p),0)!=0)
			{
				if(totalvar==1)
				{
					poly_z f,g,h;
					to_poly_z(f,f_var);to_poly_z(g,g_var);
					UniGcdZp(h,f,g,CInt(p));
					from_poly_z(result,h);
					result=FromCoefficients(result,At(v_var,0));
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
				else
				{
					sparse_z f,g,h;
					std::vector<mU::uint> indices(totalvar);
					to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
					MultiGcdZp(h,f,g,CInt(p));
					from_sparse_z(result,h,totalvar,0);
					result=FromCoefficientList(result,v_var);
					result=Expand(result);
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
			}
		}
	}
	if(totalvar==1)
	{
		poly_z f,g,h;
		to_poly_z(f,f_var);to_poly_z(g,g_var);
		UniGcdZ(h,f,g);
		from_poly_z(result,h);
		result=FromCoefficients(result,At(v_var,0));
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	else
	{
		sparse_z f,g,h;
		std::vector<mU::uint> indices(totalvar);
		to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
		MultiGcdZ(h,f,g);
		from_sparse_z(result,h,totalvar,0);
		result=FromCoefficientList(result,v_var);
		result=Expand(result);
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	return Int(0L);
}

CPROC_ATOMIC_INT2(System_PolynomialLCM, 2, 3)
{
	var result;
	var f_var=At(x,0),g_var=At(x,1);
	var v_var=Variables(Vec(f_var,g_var));
	mU::uint totalvar=Size(v_var);
	if(totalvar == 0) {
		return Eval(Ex(Sym(__W("LCM"), System), x));
	}
	if(totalvar==1)
	{
		f_var=Coefficients(Expand(f_var),At(v_var,0));
		g_var=Coefficients(Expand(g_var),At(v_var,0));
	}
	else
	{
		f_var=CoefficientList(Expand(f_var),v_var);
		g_var=CoefficientList(Expand(g_var),v_var);
	}
	if(Size(x)==3)
	{
		var rule=At(x,2);
		if(Head(rule)==tag_Rule&&At(Body(rule),0)==tag_modulus)
		{
			var p=At(Body(rule),1);
			if(mpz_cmp_ui(CInt(p),0)!=0)
			{
				if(totalvar==1)
				{
					poly_z f,g,h;
					to_poly_z(f,f_var);to_poly_z(g,g_var);
					UniLcmZp(h,f,g,CInt(p));
					from_poly_z(result,h);
					result=FromCoefficients(result,At(v_var,0));
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
				else
				{
					sparse_z f,g,h;
					std::vector<mU::uint> indices(totalvar);
					to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
					MultiLcmZp(h,f,g,CInt(p));
					from_sparse_z(result,h,totalvar,0);
					result=FromCoefficientList(result,v_var);
					result=Expand(result);
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
			}
		}
	}
	if(totalvar==1)
	{
		poly_z f,g,h;
		to_poly_z(f,f_var);to_poly_z(g,g_var);
		UniLcmZ(h,f,g);
		from_poly_z(result,h);
		result=FromCoefficients(result,At(v_var,0));
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	else
	{
		sparse_z f,g,h;
		std::vector<mU::uint> indices(totalvar);
		to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
		MultiLcmZ(h,f,g);
		from_sparse_z(result,h,totalvar,0);
		result=FromCoefficientList(result,v_var);
		result=Expand(result);
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	return Int(0L);
}

CPROC_ATOMIC_INT(System_Decompose, 1)
{
	var f=At(x,0);
	var v=Variables(f);
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_q fpoly;
	std::vector<poly_q> partlist;
	to_poly_q(fpoly,f);
	UniFuncionalDecompositionQ(partlist,fpoly);
	var result=Vec();
	var g;
	Resize(result,partlist.size());
	for(mU::uint i=0;i<partlist.size();i++)
	{
		from_poly_q(g,partlist[i]);
		g=FromCoefficients(g,v);
		At(result,i)=g;
	}
	clear_poly_q_list(partlist);
	fpoly.resize(0);
	return result;
}

CPROC_ATOMIC(System_Cyclotomic)
{
	var n_var=At(x,0);
	var v_var=At(x,1);
	mU::uint n=mpz_get_ui(CInt(n_var));
	poly_z r;
	UniCyclotomicZ(r,n);
	var r_var=Vec();
	from_poly_z(r_var,r);
	r.resize(0);
	r_var=FromCoefficients(r_var,v_var);
	return r_var;
}

CPROC_ATOMIC(System_CyclotomicQ)
{
	var f_var=At(x,0);
	var v_var=Variables(f_var);
	v_var=At(v_var,0);
	f_var=Coefficients(Expand(f_var),v_var);
	poly_z f,g;
	to_poly_z(f,f_var);
	mU::uint n;
	n=UniShiftedCyclotomicZQ(g,f);
	var n_var=Int();
	mpz_set_ui(CInt(n_var),n);
	if(n!=0)
	{
		var g_var,gg_var;
		g_var=Vec();
		from_poly_z(g_var,g);
		gg_var=FromCoefficients(g_var,v_var);
		f.resize(0);g.resize(0);
		return Ex(tag_cyclotomic,Vec(n_var,gg_var));
	}
	else
	{
		f.resize(0);g.resize(0);
		return n_var;
	}
}

CPROC_ATOMIC_INT(System_FactorList, 1)
{
	var f=At(x,0);
	var v = Variables(f);

	if(Size(v)==0)
	{
		var r=Vec();
		Push(r,f);
		return r;
	}
	//多元情形
	if(Size(v)>1)return 0;

	//非整系数情形
	var fc;
	fc = Coefficients(Expand(f),At(v,0));
	for(size_t i = 0; i < Size(fc); ++i)
	{
		if(!IntQ(At(fc,i)))return 0;
	}
	if(Size(x)==2)
	{
		var rule=At(x,1);
		if(Head(rule)==tag_Rule&&At(Body(rule),0)==tag_modulus)
		{
			var p=At(Body(rule),1);
			if(mpz_cmp_ui(CInt(p),0)!=0)
			{
				var r=UniFacZp(fc,p);
				size_t n = Size(r);
				for(size_t i = 1; i < n; ++i)
				{
					At(At(r,i),0) = FromCoefficients(At(At(r,i),0),At(v,0));
				}
				return r;
			}
		}
	}
	var r = UniFacZ(fc);
	size_t n = Size(r);
	At(r, 0) = Vec(At(r, 0), Int(1L));
	for(size_t i = 1; i < n; ++i)
	{
		At(At(r,i),0) = FromCoefficients(At(At(r,i),0),At(v,0));
	}
	return r;
}

CPROC_ATOMIC_INT(System_Factor, 1)
{
	var r,result;
	r = System_FactorList(x);
	//error
	if(!r)return 0;

	result=Vec();
	mU::uint size=Size(r);
	for(mU::uint i=1;i<size;++i)
	{
		Push(result,Ex(TAG(Power),Vec(At(At(r,i),0),At(At(r,i),1))));
	}
	result=Ex(TAG(Times),result);
	result=Ex(TAG(Times),Vec(At(At(r,0),0),result));
	return result;
}

//todo 不同的字典序
CPROC_ATOMIC(System_CharacterSet)
{
	var v_var;
	var polylist_var;
	mU::uint vars;
	polylist_var=At(x,0);
	if(Size(x)==1)
	{
		v_var=Variables(polylist_var);
	}
	if(Size(x)==2)
	{
		v_var=At(x,1);
	}
	vars=Size(v_var);
	var f_var;
	std::vector<sparse_z> polylist,cs;
	std::vector<mU::uint> indices(vars,0);
	polylist.resize(Size(polylist_var));
	for(mU::uint i=0;i<Size(polylist_var);i++)
	{
		f_var=CoefficientList(Expand(At(polylist_var,i)),v_var);
		to_sparse_z(polylist[i],f_var,vars,indices);
		polylist[i].normalize();
	}
	CharacterSetZ(cs,polylist,vars);
	var result=Vec();
	Resize(result,cs.size());
	for(mU::uint i=0;i<cs.size();i++)
	{
		from_sparse_z(f_var,cs[i],vars,0);
		At(result,i)=FromCoefficientList(f_var,v_var);
	}
	clear_sparse_z_list(polylist);clear_sparse_z_list(cs);
	return result;
}
CPROC_ATOMIC_INT(System_PolynomialQuotient, 3) {
	var f_var=At(x,0),g_var=At(x,1),q_var;
	var v_var=At(x,2);
	f_var=Coefficients(Expand(f_var),v_var);
	g_var=Coefficients(Expand(g_var),v_var);
	poly_q f;
	poly_q g;
	poly_q q;
	to_poly_q(f, f_var);
	to_poly_q(g, g_var);
	//UniDivExactZ(q,f,g);

	UniDivQ(q,f,g);

	from_poly_q(q_var,q);
	q_var=FromCoefficients(q_var,v_var);
	return q_var;
}
CPROC_ATOMIC_INT(System_PolynomialRemainder, 3) {
	var f_var=At(x,0),g_var=At(x,1),r_var;
	var v_var=At(x,2);
	f_var=Coefficients(Expand(f_var),v_var);
	g_var=Coefficients(Expand(g_var),v_var);
	poly_z f;
	poly_z g;
	poly_z r;
	to_poly_z(f, f_var);
	to_poly_z(g, g_var);
	UniReExactZ(r,f,g);
	from_poly_z(r_var,r);
	r_var=FromCoefficients(r_var,v_var);
	return r_var;
}
CPROC_ATOMIC_INT(System_PolynomialQuotientRemainder, 3) {
	var f_var=At(x,0),g_var=At(x,1),q_var,r_var;
	var v_var=At(x,2);
	f_var=Coefficients(Expand(f_var),v_var);
	g_var=Coefficients(Expand(g_var),v_var);
	poly_q f;
	poly_q g;
	poly_q q;
	poly_q r;
	to_poly_q(f, f_var);
	to_poly_q(g, g_var);

	UniDivModQ(q, r, f, g);

	from_poly_q(q_var, q);
	from_poly_q(r_var, r);
	q_var=FromCoefficients(q_var, v_var);
	r_var=FromCoefficients(r_var, v_var);
	return Vec(q_var, r_var);
}
CPROC_ATOMIC_INT(System_UniFacZ, 1)
{
	return UniFacZ(At(x,0));
}
CPROC_ATOMIC_INT(System_UniFacZp, 2)
{
	return UniFacZp(At(x,0),At(x,1));
}
CPROC_ATOMIC_INT(System_UniAddZ, 2)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniAddZ(h,f,g);
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniAddZp, 3)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniAddZp(h,f,g,CInt(At(x,2)));
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniMulZ, 2)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniMulZ(h,f,g);
	from_poly_z(result,h);
	return result;
}
CPROC_ATOMIC_INT(System_UniMulZp, 3)
{
	var result;
	poly_z h,f,g;
	to_poly_z(f,At(x,0));
	to_poly_z(g,At(x,1));
	UniMulZp(h,f,g,CInt(At(x,2)));
	from_poly_z(result,h);
	return result;
}

namespace
{
	bool ContainQ(Var exp, Var variables)
	{
		assert(Type(variables) == TYPE(vec));
		const size_t l = Size(variables);
		for (size_t i = 0; i < l; ++i)
		{
			if (!FreeQ(exp, At(variables, i)))
			{
				return true;
			}
		}

		return false;
	}

	bool PolynomialQ(Var exp, Var variables)
	{
		assert(Type(variables) == TYPE(vec));
		switch (Type(exp))
		{
		case TYPE(obj):
		case TYPE(vec):
		case TYPE(str):
			return false;
		case TYPE(int):
		case TYPE(flt):
		case TYPE(rat):
		case TYPE(sym):
			return true;
		case TYPE(ex):
		{
			Var head = Head(exp);
			Var body = Body(exp);
			if (head == TAG(Plus) || head == TAG(Times))
			{
				const size_t length = Size(body);
				for (size_t i = 0; i < length; ++i)
				{
					if (!PolynomialQ(At(body, i), variables))
					{
						return false;
					}
				}
				return true;
			}
			else if (head == TAG(Power))
			{ 
				if (!ContainQ(exp, variables))
				{
					return true;
				}
				assert(Size(body) == 2);
				return PolynomialQ(At(body, 0), variables) 
						&& Type(At(body, 1)) == TYPE(int)
						&& Z::Cmp(At(body, 1), 0) >= 0;
			}
			else
			{
				return !ContainQ(exp, variables);
			}

		}
		default:
			assert(false);
		}

		return false;	// we should never reach here!
	}
}

CPROC_INT(System_PolynomialQ, 2)
{
#if 0
	var variables = Vec();
	switch (Type(At(x, 1)))
	{
	case TYPE(sym):
		Push(variables, At(x, 1));
		break;
	case TYPE(vec):
	{
		variables = At(x, 1);
		const size_t length = Size(variables);
		for (size_t i = 0; i < length; ++i)
		{
			if (Type(At(variables, i)) != TYPE(sym))
			{
				throw LogicError(L"we expect a symbol or a list of symbols for 2nd argument to PolynomialQ");
			}
		}
		break;
	}
	default:
		throw LogicError(L"we expect a symbol or a list of symbols for 2nd argument to PolynomialQ");
	}
#endif
	var variables = VecQ(At(x, 1)) ? At(x, 1) : Vec(At(x, 1));
	return PolynomialQ(At(x, 0), variables) ? True : False;
}
