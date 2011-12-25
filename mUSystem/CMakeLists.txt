new_shared_library(musystem)

add_common_to_target(musystem)
target_link_libraries(musystem mukernel)
add_definitions(-DMUSYSTEM_EXPORTS)
