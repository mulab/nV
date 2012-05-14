new_shared_library(nvembedjava)

target_link_libraries(nvembedjava nvlinkjava nvkernel)

new_nv_script(EmbedJava.n nVLink/Java/Embed)
new_nv_script(awtTest.u nVLink/Java/Embed)
new_nv_script(plotTest.u nVLink/Java/Embed)
