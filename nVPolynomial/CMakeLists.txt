new_shared_library(nvpolynomial)

include_directories(.)
add_common_to_target(nvpolynomial)
target_link_libraries(nvpolynomial nvkernel nvnumber mukernel musystem)
add_definitions(-DPOLYNOMIAL_EXPORTS)

install(FILES mUPolynomial.m Polynomial.n DESTINATION Polynomial)

if (USE_NTL_FLINT)
    add_definitions(-DCPE_USE_NTL_FLINT)
endif ()
