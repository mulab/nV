#include "LinkLua.h"
#include <nV/utils.h>

namespace nV {
// idx -> Key?
var from_lua(lua_State* L, int idx) {
	switch (lua_type(L, idx)) {
	case LUA_TNONE:
		return $.Fail;
	case LUA_TNIL:
		return null;
	case LUA_TBOOLEAN:
		return boolean(lua_toboolean(L, idx) != 0);
	case LUA_TLIGHTUSERDATA:
	case LUA_TNUMBER:
		return new Real(lua_tonumber(L, idx), 0);
	case LUA_TSTRING:
		return new String(mbs2wcs(lua_tostring(L, idx)));
	case LUA_TTABLE:
	case LUA_TFUNCTION:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
		return null;
	}
	return null;
}
void to_lua(lua_State* L, sym x) {
	if (!x) {
		lua_pushnil(L);
		return;
	}
	sym c = x->context;
	if (!c)
		lua_getglobal(L, "nV");
	else
		to_lua(L, c);
	lua_getfield(L, -1, wcs2mbs(x->name()).c_str());
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_setfield(L, -2, wcs2mbs(x->name()).c_str());
		lua_getfield(L, -1, wcs2mbs(x->name()).c_str());
	}
	// lua_pushstring(L, wcs2mbs(x->toS()).c_str());
}
void to_lua(lua_State* L, const Key& x) {
	switch (x.kind()) {
	case Key::Null:
		// lua_pushvalue();
		lua_pushnil(L);
		return;
	case Key::Integer:
		lua_pushinteger(L, x.toUI());
		return;
	case Key::String:
		lua_pushstring(L, wcs2mbs(x.toS()).c_str());
		return;
	}
}
void to_lua(lua_State* L, const Object& x) {
	if (isNumber(x)) {
		lua_pushnumber(L, toD(x));
		return;
	}
	if (x.type == $.String) {
		lua_pushstring(L, wcs2mbs(x.cast<String>().str).c_str());
		return;
	}
	lua_pushnil(L);
}
void to_lua(lua_State* L, const Tuple& x) {
	lua_createtable(L, x.size, 0);
	for (uint i = 0; i < x.size; ++i) {
		to_lua(L, x[i]);
		lua_rawseti(L, -2, i);
		lua_pop(L, 1);
	}
}
void to_lua(lua_State* L, const var& x) {
	switch (x.primary()) {
	case Primary::Null:
		lua_pushnil(L);
		return;
	case Primary::Symbol:
		to_lua(L, x.symbol());
		return;
	case Primary::Key:
		to_lua(L, x.key());
		return;
	case Primary::Object:
		to_lua(L, x.object());
		return;
	case Primary::Tuple:
		to_lua(L, x.tuple());
		return;
	}
}
}