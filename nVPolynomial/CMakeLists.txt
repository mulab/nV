include_directories(./)
file(GLOB POLYNOMIAL_SRCS "*.cpp")
AUX_SOURCE_DIRECTORY(mU/ MU_POLYNOMIAL_SRCS)
add_library(nvpolynomial SHARED ${POLYNOMIAL_SRCS} ${MU_POLYNOMIAL_SRCS})
add_precompiled_header(nvpolynomial ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)
target_link_libraries(nvpolynomial nvkernel nvnumber mukernel musystem) 
if(DEFINED USE_NTL_FLINT)
  set_property(
  SOURCE ${POLYNOMIAL_SRCS}
  APPEND PROPERTY COMPILE_DEFINITIONS CPE_USE_NTL_FLINT
  )
endif()
