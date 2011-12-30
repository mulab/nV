new_shared_library(mukernel)

add_common_to_target(mukernel)
target_link_libraries(mukernel ${Boost_LIBRARIES})
target_link_libraries(mukernel ${gmp_LIBRARIES})
add_definitions(-DMUKERNEL_EXPORTS)
