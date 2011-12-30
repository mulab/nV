#include <nV/Grammar.h>
#include <nV/System.h>

using namespace nV;

CAPI void VALUE(Full)(Kernel& k, var& r, Tuple& x) {
    wostringstream oss;
    for (uint i = 1; i < x.size; ++i)
        x[i].print(oss);
    r = new String(oss.str());
}
CAPI void VALUE(Print)(Kernel& k, var& r, Tuple& x) {
    for (uint i = 1; i < x.size; ++i)
		grammar.print(k, wcout, k.value(tuple(SYS(Pretty), x[i])));
    wcout << endl;
    r = null;
}
CAPI void VALUE(ToString)(Kernel& k, var& r, Tuple& x) {
	wostringstream oss;
	for (uint i = 1; i < x.size; ++i)
		grammar.print(k, oss, k.value(tuple(SYS(Pretty), x[i])));
	r = new String(oss.str());
}
