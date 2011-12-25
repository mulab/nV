add_subdirectory (Embed)

new_shared_library (nvlinkecl
    Ecl.cpp
    EclObject.cpp
)

target_link_libraries(nvlinkecl nvkernel ${ecl_LIBRARIES})
