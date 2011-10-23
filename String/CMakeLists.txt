AUX_SOURCE_DIRECTORY(. STRING_SRCS)
add_library(nvstring SHARED ${STRING_SRCS})
target_link_libraries(nvstring nvkernel) 
