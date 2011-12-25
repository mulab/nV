cmake_minimum_required(VERSION 2.6)

project(nVLinkFLINT CXX)

set(CMAKE_CXX_FLAGS "-std=c++0x -g -L${nVLinkNTL_SOURCE_DIR}/../../build")
include_directories(${nVLinkFLINT_SOURCE_DIR}/../../Include)
link_directories(${nVLinkFLINT_SOURCE_DIR}/../../Lib)

#add_subdirectory(Bind)
#add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKNTL_SRCS)
add_library(nvlinkflint SHARED ${NVLINKNTL_SRCS})
target_link_libraries(nvlinkflint nvkernel flint)
