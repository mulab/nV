new_shared_library (nvembedecl)

target_link_libraries(nvembedecl nvlinkecl nvkernel)

new_nv_script(EclTest.n nVLink/Ecl/Embed)
new_nv_script(EmbedEcl.n nVLink/Ecl/Embed)
