new_shared_library(nvlinkyafu)

target_link_libraries(nvlinkyafu nvkernel ${yafu_LIBRARIES})

new_nv_script(yafu.u nVLink/yafu)
new_nv_script(LinkYAFU.n nVLink/yafu)