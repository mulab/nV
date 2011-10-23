file(GLOB MULINK_SRCS "*.cpp")
add_library(mulink SHARED ${MULINK_SRCS})
add_precompiled_header(mulink ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)
target_link_libraries(mulink nvkernel mukernel nvparser)
