new_shared_library(mu0embed)

target_link_libraries(mu0embed mu0link)

new_nv_script(mU0.u nVLink/mU0/Embed)
new_nv_script(mU0Test.u nVLink/mU0/Embed)