AUX_SOURCE_DIRECTORY(. NVKERNEL_SRCS)
add_library(nvkernel SHARED ${NVKERNEL_SRCS})

if(WIN32)
    target_link_libraries(nvkernel gmp)
elseif(UNIX)
    target_link_libraries(nvkernel gmp dl boost_system)
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()

