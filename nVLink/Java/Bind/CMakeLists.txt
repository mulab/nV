include_directories(${nVLinkJava_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NVBINDJAVA_SRCS)
add_library(nvbindjava SHARED ${NVBINDJAVA_SRCS})
target_link_libraries(nvbindjava nvlinkjava nvkernel nvparser)