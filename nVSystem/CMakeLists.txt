new_shared_library(nvsystem)

target_link_libraries(nvsystem nvkernel nvparser)
add_definitions(-DSYSTEM_EXPORTS)

new_nv_script(System.n System)
