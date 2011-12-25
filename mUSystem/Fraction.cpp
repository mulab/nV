#include "common.h"
#include <mU/Number.h>
#include <mU/System.h>

using namespace mU;

CPROC_INT(System_RationalQ, 1)
{
	return RatQ(At(x,0)) ? True : False;
}
CPROC_INT(System_RationalNumerator, 1)
{
	if (RatQ(At(x,0))) {
		var r = Int();
		Q::Num(r, At(x,0));
		return r;
	}
	return 0;
}
CPROC_INT(System_RationalDenominator, 1)
{
	if (RatQ(At(x,0))) {
		var r = Int();
		Q::Den(r, At(x,0));
		return r;
	}
	return 0;
}
