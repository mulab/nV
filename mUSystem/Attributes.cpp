#include "StdAfx.h"
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Attributes)
{
	var r = Vec();
	std::map<Var,attr_t>::const_iterator
		iter = Attributes.find(At(x,0));
	if(iter != Attributes.end())
		CVec(r).assign(iter->second.begin(),iter->second.end());
	return r;
}
CAPI CPROC(System_SetAttributes)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			Attributes[a].insert(At(b,i));
	}
	else
		Attributes[a].insert(b);
	return Null;
}