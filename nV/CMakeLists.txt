AUX_SOURCE_DIRECTORY(. NV_SRCS)
add_executable(nv ${NV_SRCS})
target_link_libraries(nv nvparser)
