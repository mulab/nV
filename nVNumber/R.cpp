#include "common.h"
#include <nV/Number.h>

namespace nV {
namespace Number {
#define M(name,f)\
var operator name(const Real& x)\
{\
	Real* r = new Real();\
	mpf_##f(r->mpf, x.mpf);\
	return r;\
}

M(-, neg)
#undef M
#define M(name,f)\
var operator name(const Real& x, const Real& y)\
{\
	Real* r = new Real();\
	mpf_##f(r->mpf, x.mpf, y.mpf);\
	return r;\
}

M(+, add)
M(-, sub)
M(*, mul)
M( / , div)

#undef M
#define M(name,f)\
var operator name(const Real& x, uint y)\
{\
	Real* r = new Real();\
	mpf_##f##_ui(r->mpf, x.mpf, y);\
	return r;\
}

M(+, add)
M(-, sub)
M(*, mul)
M( / , div)

#undef M
#define M(name,f)\
var operator name(uint x, const Real& y)\
{\
	Real* r = new Real();\
	mpf_ui_##f(r->mpf, x, y.mpf);\
	return r;\
}

M(-, sub)
M( / , div)

#undef M
}
}
