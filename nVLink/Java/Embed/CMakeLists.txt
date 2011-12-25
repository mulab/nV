include_directories(${nVLinkJava_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NVEMBEDJAVA_SRCS)
add_library(nvembedjava SHARED ${NVEMBEDJAVA_SRCS})
target_link_libraries(nvembedjava nvlinkjava nvkernel)
