new_shared_library (nvembedecl)

target_link_libraries(nvembedecl nvlinkecl nvkernel)

install(FILES EclTest.n DESTINATION Link/Ecl/Embed)
