cmake_minimum_required(VERSION 2.6)

project(nVLinkYAFU CXX)

include_directories(${nVLinkYAFU_SOURCE_DIR}/../../Include)
link_directories(${nVLinkYAFU_SOURCE_DIR}/../../Lib)

#add_subdirectory(Bind)
#add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKYAFU_SRCS)
add_library(nvlinkyafu SHARED ${NVLINKYAFU_SRCS})
target_link_libraries(nvlinkyafu nvkernel yafu)
