#include "LinkLua.h"
#include <nV/utils.h>

namespace nV {
// API extern 
sym $Lua, $LuaState;
class LuaState : public Object {
public:
	lua_State* L;
    virtual ~LuaState() {
        lua_close(L);
    }
	/*
    API virtual LuaState* clone() const;
    API virtual int compare(const Object&) const;
    API virtual bool equal(const Object&) const;
    API virtual size_t hash() const;
    API virtual void print(wostream&) const;
	*/
    LuaState() : Object($LuaState), L(lua_open()) {
		// luaL_dostring(L, "nV={}");
		lua_newtable(L);
		lua_setglobal(L, "nV");
	}
};
}

using namespace nV;

CAPI void Install(Kernel& k) {
	$Lua = SYM(root, Lua);
	$LuaState = SYM($Lua, State);
	//wcout << "#LinkLua# Install Successfully!" << endl;
	
}
CAPI void Uninstall(nV::Kernel& k) {
	wcout << "#LinkLua# Uninstall Successfully!" << endl;
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Lua,f)
#undef VALUE
#define VALUE(f) CVALUE(Lua,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Lua,f,sig)

CAPI void VALUE(Open)(Kernel& k, var& r, Tuple& x) {
	r = new LuaState;
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Lua_State,f)
#undef VALUE
#define VALUE(f) CVALUE(Lua_State,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Lua_State,f,sig)

CAPI void METHOD(openlibs, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	luaL_openlibs(L);
	r = null;
}
CAPI void METHOD(dofile, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	int ret = luaL_dofile(L, wcs2mbs(x[1].cast<String>().str).c_str());
	r = new Integer((sint)ret);
}
CAPI void METHOD(dostring, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	int ret = luaL_dostring(L, wcs2mbs(x[1].cast<String>().str).c_str());
	r = new Integer((sint)ret);
}
CAPI void METHOD(at, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	r = from_lua(L, x[1].cast<Integer>().toSI());
}
CAPI void VALUE(push)(Kernel& k, var& r, Tuple& x) {
	lua_State* L = k.self().cast<LuaState>().L;
	for (uint i = 1; i < x.size; ++i)
		to_lua(L, x[i]);
	r = null;
}
CAPI void METHOD(pop, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	lua_pop(L, x[1].cast<Integer>().toSI());
	r = null;
}
CAPI void METHOD(type, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	r = new Integer((sint)lua_type(L, x[1].cast<Integer>().toSI()));
}
CAPI void METHOD(typename, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	lua_State* L = self.cast<LuaState>().L;
	r = new String(mbs2wcs(lua_typename(L, x[1].cast<Integer>().toSI())));
}