#include "StdAfx.h"
#include <mU/Kernel.h>
#include "mU/Matrix.h"

using namespace mU;

CAPI void mUInstall()
{
	static bool Initialized = false;
	if(Initialized) return;
	

	Initialized = true;
}
CAPI void mUUninstall() {
	wcout << "#Matrix# mUUninstall Successfully!" << std::endl;
}

using namespace mU::Matrix;

namespace {
//matrix z2q
inline var mz2q(Var x)
{
	size_t m=Size(x);
	size_t n=Size(At(x,0));
	var r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
		var &c =At(r,i);
		c=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			At(c,j)=Rat();
			Q::SetZ(At(c,j),Entry(x,i,j));
		}
	}
	return r;
}
//vector z2q
inline var vz2q(Var x)
{
	size_t m=Size(x);
	var r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
			At(r,i)=Rat();
			Q::SetZ(At(r,i),At(x,i));
	}
	return r;
}
//scalar z2q
/*inline var z2q(Var x)
{
	var t=Rat();
	return Q::SetZ(t,x);
}*/
}

CAPI CPROC(System_mLinearSolve)
{
	//var t=Rat();
	//Q::SetZ(t,At(x,1));
	return mLinearSolve(At(x,0),At(x,1),At(x,2));
}
CAPI CPROC(System_LinearSolve)
{
	//var t=Rat();
	//Q::SetZ(t,At(x,1));
	//输入要求为A为数类，b为任意表达式，还遗留一个问题，就是无法判断表达式为零的情况
	return LinearSolve(At(x,0),At(x,1));
}
CAPI CPROC(System_StrassenMM)
{
	return StrassenMM(At(x,0),At(x,1));
}
CAPI CPROC(System_MDot)
{
	//可以处理符号矩阵、精确有理数、浮点数的类型
	return Matrix::Dot(At(x,0),At(x,1));
}
CAPI CPROC(System_MInverse)
{
	return qInverse(At(x,0));
}
CAPI CPROC(System_MDet)
{
	return qDet(At(x,0));
}
CAPI CPROC(System_NullSpace)
{
	return qNullSpace(At(x,0));
}
CAPI CPROC(System_Rank)
{
	var r=Int();
	Z::Set(r,(mU::uint)qMatrixRank(At(x,0)));
	return r;
}
CAPI CPROC(System_LeastSquares)
{
	return LeastSquares(At(x,0),At(x,1));
}
