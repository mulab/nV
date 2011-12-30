new_shared_library(nvembedjava)

target_link_libraries(nvembedjava nvlinkjava nvkernel)

install(FILES
    awtTest.u
    EmbedJava.n
    plotTest.u

    DESTINATION Link/Java/Embed)
