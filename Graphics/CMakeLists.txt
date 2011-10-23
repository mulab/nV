AUX_SOURCE_DIRECTORY(. GRAPHICS_SRCS)
add_library(nvgraphics SHARED ${GRAPHICS_SRCS})
target_link_libraries(nvgraphics nvkernel nvnumber) 
