AUX_SOURCE_DIRECTORY(. SYSTEM_SRCS)
add_library(nvsystem SHARED ${SYSTEM_SRCS})
target_link_libraries(nvsystem nvkernel nvparser)
