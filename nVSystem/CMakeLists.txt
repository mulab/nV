new_shared_library(nvsystem)

target_link_libraries(nvsystem nvkernel nvparser)
add_definitions(-DSYSTEM_EXPORTS)

install(FILES System.n DESTINATION System)
