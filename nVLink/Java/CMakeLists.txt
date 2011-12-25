cmake_minimum_required(VERSION 2.6)

project(nVLinkJava CXX)

include_directories(${nVLinkJava_SOURCE_DIR}/../../Include)
link_directories(${nVLinkJava_SOURCE_DIR}/../.. ${nVLinkJava_SOURCE_DIR}/../../Lib)
#find_package(JNI)
include(JNIConfig.cmake)

add_subdirectory(Bind)
add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKJAVA_SRCS)
add_library(nvlinkjava SHARED ${NVLINKJAVA_SRCS})
target_link_libraries(nvlinkjava nvkernel jvm)