#pragma once
#include <nV/Interface.h>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#ifdef _MSC_VER
#pragma comment(lib,"lua5.1.lib")
#endif

#undef API
#ifdef _MSC_VER
#if defined(nvlinklua_EXPORTS) || defined(NVLINKLUA_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace nV {
API var from_lua(lua_State*, int);
API void to_lua(lua_State*, sym);
API void to_lua(lua_State*, const Key&);
API void to_lua(lua_State*, const Object&);
API void to_lua(lua_State*, const Tuple&);
API void to_lua(lua_State*, const var&);
}
