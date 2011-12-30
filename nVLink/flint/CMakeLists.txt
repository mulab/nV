new_shared_library(nvlinkflint)

target_link_libraries(nvlinkflint nvkernel
    ${flint_LIBRARIES}
    ${mpfi_LIBRARIES}
    ${mpfr_LIBRARIES}
)

install(FILES LinkFLINT.n DESTINATION Link/flint)
