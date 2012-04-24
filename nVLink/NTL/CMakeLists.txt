new_shared_library(nvlinkntl)

target_link_libraries(nvlinkntl nvkernel ${ntl_LIBRARIES})

new_nv_script(LinkNTL.n nVLink/NTL)
