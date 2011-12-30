#include "common.h"
#include <nV/Polynomial.h>
#include <nV/Number.h>
#include <algorithm>

namespace nV {
var Power(Kernel& k, const var& x, const var& y) {
	if (y.isObject()) {
		if (y.object() == 0.0)
			return new Integer(1L);
		if (y.object() == 1.0)
			return x;
	}
	if (x.isObject()) {
		if (x.object() == 0.0)
			return new Integer(0L);
		if (x.object() == 1.0)
			return x;
	}
	if (isNumber(x) && isNumber(y))
		return Number::Power(x.object(), y.object());
	if (x.isTuple()) {
		if (x.tuple()[0] == $.Power) {
			if (y.isObject($.Integer))
				return Power(k, x.tuple()[1], Times(k, x.tuple()[2], y));
		}
		if (x.tuple()[0] == $.Times) {
			var r = tuple(x.tuple().size);
			r.tuple()[0] = $.Times;
			for (size_t i = 1; i < r.tuple().size; ++i)
				r.tuple()[i] = Power(k, x.tuple()[i], y);
			std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size, lessPrimary);
			return Times(k, r.tuple());
		}
	}
	return list(x, y, $.Power);
}
}


using namespace nV;

CAPI void CVALUE(System, Power)(Kernel& k, var& r, Tuple& x) {
	if (!x[0].isSymbol())
		return;
	if (x.size == 3)
		r = Power(k, x[1], x[2]);
}
