cmake_minimum_required(VERSION 2.6)

project(nVLinkPython CXX)

include_directories(${nVLinkPython_SOURCE_DIR}/../../Include)
link_directories(${nVLinkPython_SOURCE_DIR}/../.. ${nVLinkPython_SOURCE_DIR}/../../Lib)
#find_package(Python)
include(PythonConfig.cmake)

#add_subdirectory(Bind)
#add_subdirectory(Embed)

#AUX_SOURCE_DIRECTORY(. NVLINKPYTHON_SRCS)
SET(PYTHONINNV_SRCS PythoninnV.cpp pythonObject.cpp nVutils.cpp nVinPython.cpp)
SET(NVINPYTHON_SRCS nVinPython.cpp nVutils.cpp PythonObject.cpp)
add_library(nvinpython SHARED ${NVINPYTHON_SRCS})
add_library(pythoninnv SHARED ${PYTHONINNV_SRCS})
target_link_libraries(nvinpython nV nVKernel nVParser nVPolynomial nVString nVSystem python27)
target_link_libraries(pythoninnv nvinpython nvkernel python27)