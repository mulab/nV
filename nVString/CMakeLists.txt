new_shared_library(nvstring)

target_link_libraries(nvstring nvkernel)

install(FILES String.n DESTINATION String)
