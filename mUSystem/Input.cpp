#include "StdAfx.h"
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Input)
{
	var r = Size(x) > 0
		? Read(wcin,At(x,0))
		: Read(wcin);
	return r ? r : Nil;
}
CPROC_INT(System_Get, 1)
{
	if(StrQ(At(x,0))) {
		return Get(CStr(At(x,0)));
	}
	return 0;
}
CPROC_INT(System_ReadList, 1)
{
	if(StrQ(At(x,0))) {
		return ParseFile(CStr(At(x,0)));
	}
	return 0;
}