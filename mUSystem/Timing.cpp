#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/utils.h>
#include <mU/System.h>

using namespace mU;

CAPI CPROC(System_Timing)
{
	if (Size(x) == 2 && IntQ(At(x,1))) {
		timer t;
		t.start();
		var r;
		mU::uint n = Z::UI(At(x,1));
		for (size_t i = 0; i < n; ++i)
			r = Eval(At(x,0));
		t.end();
		return Vec(Int(t.value),r);
	}
	timer t;
	t.start();
	var r = Eval(At(x,0));
	t.end();
	return Vec(Int(t.value),r);
}
CAPI CPROC(System_Path)
{
	return Size(x) ? Str(path() + CStr(At(x,0))) : Str(path());
}
// CAPI CPROC(System_Install)
// {
// 	return Install(CStr(At(x,0)).c_str());
// }
// CAPI CPROC(System_Uninstall)
// {
// 	return Uninstall(At(x,0)) ? True : False;
// }
CAPI CPROC(System_Run)
{
	return shell(CStr(At(x,0)).c_str()) ? True : False;
}
//CAPI CPROC(System_Task)
//{
//	return Task(At(x,0));
//}
//CAPI CPROC(System_Kill)
//{
//	return Kill(At(x,0)) ? True : False;
//}
CAPI CPROC(System_NumberQ)
{
	return NumberQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_AtomQ)
{
	return AtomQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_IntegerQ)
{
	return IntQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_SymbolQ)
{
	return SymQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_StringQ)
{
	return StrQ(At(x,0)) ? True : False;
}
CAPI CPROC(System_Unset)
{
	Unset(At(x,0));
	return Null;
}
CAPI CPROC(System_Clear)
{
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
		Clear(At(x,i));
	return Null;
}
CAPI CPROC(System_Evaluate)
{
	return Eval(At(x,0));
}
CPROC_INT2(System_Verbose, 0, 1)
{
	if (Size(x) == 0)
		return Int(Verbose);
	if (IntQ(At(x,0)))
		Verbose = Z::SI(At(x,0));
	return Null;
}
CPROC_INT2(System_DownValuesDefaultType, 0, 1)
{
	if (Size(x) == 0)
		return Int(def_t::default_type);
	if (IntQ(At(x,0)))
		def_t::default_type = Z::SI(At(x,0));
	return Null;
}