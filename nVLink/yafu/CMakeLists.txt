new_shared_library(nvlinkyafu)

target_link_libraries(nvlinkyafu nvkernel ${yafu_LIBRARIES})

install(FILES LinkYAFU.n yafu.u
    DESTINATION Link/yafu)
