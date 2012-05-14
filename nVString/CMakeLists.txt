new_shared_library(nvstring)

target_link_libraries(nvstring nvkernel)

new_nv_script(String.n String)
