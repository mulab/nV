new_shared_library(mulink)

add_common_to_target(mulink)
target_link_libraries(mulink nvkernel mukernel nvparser)

new_mu_script(EmbednV.m mULink)
new_nv_script(EmbedmU.n mULink)

