new_shared_library(nvlinkntl)

target_link_libraries(nvlinkntl nvkernel ${ntl_LIBRARIES})

install(FILES LinkNTL.n DESTINATION Link/NTL)
