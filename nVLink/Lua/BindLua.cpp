#include "LinkLua.h"
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>
#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib,"libluabindd")
#else
#pragma comment(lib,"libluabind")
#endif
#endif

namespace nV {

void display(const var& x) {
	x.print();
}
var eval(double x) {
	return new Integer(x);
}

}

void bind_var(lua_State* L)
{
	using namespace luabind;

	module(L, "nV")
		[
			class_<nV::var>("var")
			.def(constructor<>())
			.def("print", &nV::display),

			def("print", nV::display),

			def("eval", nV::eval)

		];
}
CAPI int luaopen_nvlinklua(lua_State *L) {
	using namespace luabind;
	
	open(L);

	bind_var(L);

	// luaL_dofile(L, "cln_test.lua");

	return 0;
}