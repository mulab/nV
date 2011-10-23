#pragma once

#include <mU/Kernel.h>
#include <mU/Polynomial_impl.h>

namespace mU {
//////////////////////////////////////

//ExternPolynomial.cpp
extern var tag_modulus,tag_cyclotomic,tag_root;

//////////////////////////////////////
//connection between functions of polynomials and the kernel
//ExternFactorization.cpp
var UniFacZp(Var f,Var p);
var UniFacZ(Var f);

//ExternPolynomial.cpp
void PolynomialInitialize();
void to_sparse_z(sparse_z & r,Var s,uint n,std::vector<uint> & indices);
void from_sparse_z(var & s,const sparse_z & r,uint n,uint i);
void to_sparse_q(sparse_q & r,Var s,uint n,std::vector<uint> & indices);
void from_sparse_q(var & s,const sparse_q & r,uint n,uint index);
void to_poly_z(poly_z &f, Var g);
void from_poly_z(var & f, const poly_z& g);
void to_poly_q(poly_q & f,Var g);
void from_poly_q(var & f, const poly_q & g);

//ExternUnisolve.cpp
void UniSolveInitialize();
void UniSolve_Decomposition_Stage(var & rootlist, const poly_z & f);
void UniSolve_Factorization_Stage(var & rootlist, const poly_z & f);

//////////////////////////////////////
}
