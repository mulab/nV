include_directories(${nVLinkEcl_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NVEMBEDECL_SRCS)
add_library(nvembedecl SHARED ${NVEMBEDECL_SRCS})
target_link_libraries(nvembedecl nvlinkecl nvkernel) 
