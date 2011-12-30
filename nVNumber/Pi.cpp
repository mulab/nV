#include "common.h"
#include <nV/Number.h>
#include "Pi.Impl.h"

namespace nV {
namespace Number {
var Pi(uint n) {
	Real* r = new Real(n);
	pi_chudnovsky(r->mpf, static_cast<long>(LOG_2_10 * r->precision()));
	return r;
}
}
}
