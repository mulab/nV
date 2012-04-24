new_shared_library(nvnumber)

include_directories(.)
add_common_to_target(nvnumber)
target_link_libraries(nvnumber
    nvkernel
    musystem
    ${Boost_SYSTEM_LIBRARIES}
    ${mpfr_LIBRARIES}
    ${mpfi_LIBRARIES}
)
add_definitions(-DNUMBER_EXPORTS)

new_mu_script(mUNumber.m Number)
new_nv_script(Number.n Number)
