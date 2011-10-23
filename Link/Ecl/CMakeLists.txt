cmake_minimum_required(VERSION 2.6)

project(nVLinkEcl CXX)

include_directories(${nVLinkEcl_SOURCE_DIR}/../../Include)
link_directories(${nVLinkEcl_SOURCE_DIR}/../.. ${nVLinkEcl_SOURCE_DIR}/../../Lib)

add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKECL_SRCS)
add_library(nvlinkecl SHARED ${NVLINKECL_SRCS})
target_link_libraries(nvlinkecl nvkernel ecl)
