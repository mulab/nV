new_shared_library(nvkernel)

target_link_libraries(nvkernel ${Boost_LIBRARIES})
target_link_libraries(nvkernel ${gmp_LIBRARIES})
add_definitions(-DKERNEL_EXPORTS)
