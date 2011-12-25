#include <nV/Kernel.h>
#include <nV/utils.h>
#include <algorithm>

namespace nV {
void Kernel::print(const var& x, wostream& o) {
	var t = tuple($.Print, x);
	//values[$.Print].cast<Value>()(*this, t.tuple());
	value(t.tuple());
}
Kernel& Kernel::logging(wcs x) {
    *log << _W('[') << date() << _W("][");
    nV::print(x, *log);
    *log << _W("]: ");
    return *this;
}
}