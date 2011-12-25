new_shared_library(nvparser)

target_link_libraries(nvparser nvkernel nvpolynomial)
add_definitions(-DPARSER_EXPORTS)
