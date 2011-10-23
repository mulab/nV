#include "StdAfx.h"
#include <mU/Pattern.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_MatchQ)
{
	map_t m;
	return MatchQ(m,Pat(At(x,1)),At(x,0)) ? True : False;
}
CAPI CPROC(System_MemberQ)
{
	return MemberQ(At(x,0),At(x,1)) ? True : False;
}
CAPI CPROC(System_Dispatch)
{
	if(VecQ(At(x,0)))
		return Dispatch(At(x,0));
	return Dispatch(x);
}
CAPI CPROC(System_Replace)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			a = Replace(a,At(b,i));
		return a;
	}
	return Replace(a,b);
}
CAPI CPROC(System_ReplaceAll)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			a = ReplaceAll(a,At(b,i));
		return a;
	}
	return ReplaceAll(a,b);
}
CAPI CPROC(System_ReplaceRepeated)
{
	var r = System_ReplaceAll(x);
	while(!Same(r,At(x,0)))
	{
		At(x,0) = r;
		r = System_ReplaceAll(x);
	}
	return r;
}
CAPI CPROC(System_OddQ)
{
	return OddQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_EvenQ)
{
	return EvenQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_Join)
{
	size_t n = Size(x);
	var c = At(x,0);
	if(VecQ(c))
	{
		size_t m = Size(c);
		for(size_t i = 1; i < n; ++i)
			m += Size(At(x,i));
		var r = Vec(m);
		vec_t::rep_t::iterator iter = CVec(r).begin();
		std::copy(CVec(c).begin(),CVec(c).end(),iter);
		for(size_t i = 1; i < n; ++i)
			std::copy(CVec(At(x,i)).begin(),CVec(At(x,i)).end(),iter += Size(At(x,i - 1)));
		return r;
	}
	return 0;
}