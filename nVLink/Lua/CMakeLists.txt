include_directories(${LUA_INCLUDE_DIR} .)

new_shared_library (nvlinklua)
target_link_libraries(nvlinklua nvkernel
    ${LUA_LIBRARIES}
    ${luabind_LIBRARIES})

install(FILES LinkLua.n DESTINATION Link/Lua)
