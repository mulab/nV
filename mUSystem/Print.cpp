#include "StdAfx.h"
#include <mU/String.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Print)
{
	Write(wcout,Size(x) == 1 && !VecQ(At(x,0)) ? static_cast<Var>(At(x,0)) : x);
	return Null;
}
CAPI CPROC(System_Pretty)
{
	return Pretty(At(x,0));
}
CAPI CPROC(System_StringJoin)
{
	size_t n = Size(x);
	var c = At(x,0);
	if(StrQ(c))
	{
		var r = Str();
		CStr(r) = CStr(c);
		for(size_t i = 1; i < n; ++i)
			S::AddTo(r,At(x,i));
		return r;
	}
	return 0;
}
CPROC_INT(System_Put, 2)
{
	var a = At(x,0);
	var b = At(x,1);
	if(StrQ(b))
	{
		wofstream f(string(CStr(b).begin(),CStr(b).end()).c_str());
		if(f)
		{
			Println(a,f);
			f.close();
			return True;
		}
	}
	return False;
}
CPROC_INT(System_PutAppend, 2)
{
	var a = At(x,0);
	var b = At(x,1);
	if(StrQ(b))
	{
		wofstream f(string(CStr(b).begin(),CStr(b).end()).c_str(),wios::app);
		if(f)
		{
			Println(a,f);
			f.close();
			return True;
		}
	}
	return False;
}