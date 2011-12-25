new_shared_library(nvgraphics)

target_link_libraries(nvgraphics nvkernel nvnumber)

install(FILES
    mUGraphics.u
    Graphics.n

    DESTINATION Graphics)
