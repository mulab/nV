#include <nV/System.h>

using namespace nV;

CAPI void VALUE(Function)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[0].isSymbol()) {
		x[1] = k.eval(x[1]);
		return;
	}
    if (x.size > 1 && x[0].isTuple($.Function)) {
        const Tuple& t = x[0].tuple();
        if (t.size == 2) {
            k.beginSelf(&x);
            r = k.eval(t[1]);
            k.endSelf();
            return;
        }
        if (t.size == 3) {
			if (t[1].isTuple($.List)) {
				k.beginLocal();
				if (t[1].tuple().size > x.size) {
					for (uint i = 1; i < x.size; ++i)
						k.assign(t[1].tuple()[i], x[i]);
					var args = nV::list(t[1].tuple().size - x.size);
					for (uint i = x.size; i < t[1].tuple().size; ++i) {
						args.tuple()[i - x.size + 1] = t[1].tuple()[i];
						k.assign(t[1].tuple()[i], t[1].tuple()[i]);
					}
					r = tuple($.Function, args, k.eval(t[2]));
				}
				else {
					for (uint i = 1; i < t[1].tuple().size; ++i)
						k.assign(t[1].tuple()[i], x[i]);
					r = k.eval(t[2]);
				}
				k.endLocal();
				return;
			}
			else {
				k.beginLocal();
                k.assign(t[1], x[1]);
				r = k.eval(t[2]);
				k.endLocal();
				return;
			}
        }
    }
}