#include <nV/System.h>

using namespace nV;

CAPI void VALUE(Length)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
        if (x[1].isTuple()) {
			r = new Integer(x[1].tuple().size);
			return;
		}
		r = new Integer(1L);
    }
}