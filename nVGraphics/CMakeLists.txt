new_shared_library(nvgraphics)

target_link_libraries(nvgraphics nvkernel nvnumber)

new_mu_script(mUGraphics.u Graphics)
new_nv_script(Graphics.n Graphics)

