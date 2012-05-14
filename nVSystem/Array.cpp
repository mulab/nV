#include <nV/System.h>
#include <nV/Array.h>

using namespace nV;

CAPI void VALUE(IntArray)(Kernel& k, var& r, Tuple& x) {
	r = new Array<sint>;
	A<sint>(r).vec.resize(x.size - 1);
	for (size_t i = 1; i < x.size; ++i)
		A<sint>(r).vec[i - 1] = x[i].cast<Integer>().toSI();
}