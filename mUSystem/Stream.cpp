#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Buffer)
{
	return Size(x) ? Buffer(CStr(At(x,0))) : Buffer();
}
CAPI CPROC(System_File)
{
	return File(CStr(At(x,0)));
}
CAPI CPROC(System_Link)
{
	if(Size(x) > 1)
		return Link(CStr(At(x,0)),Z::UI(At(x,1)));
	return Link(Z::UI(At(x,0)));
}
CAPI CPROC(System_Read)
{
	var r = Size(x) > 1
		? Read(CStream(At(x,0)),At(x,1))
		: Read(CStream(At(x,0)));
	return r ? r : Nil;
}
CAPI CPROC(System_Write)
{
	var y = Vec();
	CVec(y).assign(CVec(x).begin() + 1,CVec(x).end());
	Write(CStream(At(x,0)),y);
	return Null;
}
CAPI CPROC(System_Server)
{
	if(Size(x) > 1)
		return Server(CStr(At(x,0)),Z::UI(At(x,1)));
	return Server(Z::UI(At(x,0)));
}
CAPI CPROC(System_Request)
{
	return Request(At(x,0));
}
CAPI CPROC(System_Invoke)
{
	return Size(x) > 2
		? Invoke(At(x,0),At(x,1),At(x,2))
		: Invoke(At(x,0),At(x,1));
}
CAPI CPROC(System_Parse)
{
	var r = Parse(CStream(At(x,0)));
	return r ? r : Nil;
}
CAPI CPROC(System_Pipe)
{
	return Pipe(CStr(At(x,0)));
}

