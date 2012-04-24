new_shared_library(nvmatrix)

include_directories(.)
add_common_to_target(nvmatrix)
target_link_libraries(nvmatrix nvkernel nvnumber mukernel musystem) 

new_mu_script(mUMatrix.m Matrix)
