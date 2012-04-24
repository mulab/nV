include_directories(${LUA_INCLUDE_DIR} .)

new_shared_library (nvlinklua)
target_link_libraries(nvlinklua nvkernel
    ${LUA_LIBRARIES}
    ${luabind_LIBRARIES})

new_nv_script(LinkLua.n nVLink/Lua)

