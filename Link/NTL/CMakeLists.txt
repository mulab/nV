cmake_minimum_required(VERSION 2.6)

project(nVLinkNTL CXX)
set(CMAKE_CXX_FLAGS "-std=c++0x -g -L${nVLinkNTL_SOURCE_DIR}/../../build")
include_directories(${nVLinkNTL_SOURCE_DIR}/../../Include ${NTL_ROOT}/include)
link_directories(${nVLinkNTL_SOURCE_DIR}/../.. ${nVLinkNTL_SOURCE_DIR}/../../Lib)

#add_subdirectory(Bind)
#add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKNTL_SRCS)
add_library(nvlinkntl SHARED ${NVLINKNTL_SRCS})
target_link_libraries(nvlinkntl nvkernel ntl)
