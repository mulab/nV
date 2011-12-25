cmake_minimum_required(VERSION 2.6)

project(nVLinkLua CXX)

include(LuaConfig.cmake)
include_directories(${nVLinkLua_SOURCE_DIR} ${nVLinkLua_SOURCE_DIR}/../../Include ${BOOST_ROOT})
link_directories(${nVLinkLua_SOURCE_DIR} ${nVLinkLua_SOURCE_DIR}/../.. ${nVLinkLua_SOURCE_DIR}/../../Lib ${BOOST_ROOT}/lib)

#add_subdirectory(Bind)
#add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. NVLINKLUA_SRCS)
add_library(nvlinklua SHARED ${NVLINKLUA_SRCS})
target_link_libraries(nvlinklua nvkernel lua5.1)