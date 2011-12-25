include_directories(${JNI_INCLUDE_DIRS})

add_subdirectory(Bind)
add_subdirectory(Embed)
add_subdirectory(Java)

new_shared_library(nvlinkjava
    Java.cpp
    JavaObject.cpp
    nV_VarPtr.cpp
)

target_link_libraries(nvlinkjava nvkernel nvparser ${JAVA_JVM_LIBRARY})
