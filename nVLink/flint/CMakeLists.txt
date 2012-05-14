new_shared_library(nvlinkflint)

target_link_libraries(nvlinkflint nvkernel
    ${flint_LIBRARIES}
    ${mpfi_LIBRARIES}
    ${mpfr_LIBRARIES}
)

new_nv_script(LinkFLINT.n nVLink/flint)

