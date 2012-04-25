# Usage:
#     add_common_to_target (xx)
#         'xx' must be a target. There must be 'common.h' and
#         'common.cpp' in this folder.
#
#     new_shared_library (xx)
#     new_shared_library (xx <file list>)
#         create a new target of shared library, named 'xx'
#         <file list> specifies all source files of the target
#
#         if <file list> is not provided, current folder
#         will be searched recursively to find all source
#         '*.cpp' files
#
#     new_executable (xx)
#     new_executable (xx <file list>)
#         same with new_shared_library, but creates a new
#         target of executable


# PCHSupport requires CMAKE_BUILD_TYPE to be set
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "CMAKE_BUILD_TYPE is not set, but it's required by PCHSupport")
    message(FATAL_ERROR "make sure you have set CMAKE_BUILD_TYPE in config.cmake")
endif (NOT CMAKE_BUILD_TYPE)

include(PCHSupport)

macro (add_common_to_target target)
    add_native_precompiled_header(
        ${target}
        ${CMAKE_CURRENT_SOURCE_DIR}/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    )
endmacro ()

macro (new_shared_library project)
    if ("${ARGN}")
        set(${project}_sources ${ARGN})
    else ()
        file(GLOB_RECURSE ${project}_sources "*.cpp")
    endif ()

	file(GLOB_RECURSE ${project}_headers "*.h")

    add_library(${project} SHARED ${${project}_sources} ${${project}_headers} ${PROJECT_HEADERS})
    target_link_libraries (${project} ${CMAKE_DL_LIBS})
    
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        install(TARGETS ${project} DESTINATION bin)
    else ()
        install(TARGETS ${project} DESTINATION lib)
    endif ()
endmacro ()

macro (new_executable project)
    if ("${ARGN}")
        set(${project}_sources ${ARGN})
    else ()
        file(GLOB_RECURSE ${project}_sources "*.cpp")
    endif ()

	file(GLOB_RECURSE ${project}_headers "*.h")

    add_executable(${project} ${${project}_sources} ${${project}_headers} ${PROJECT_HEADERS})
    install(TARGETS ${project} DESTINATION bin)
endmacro ()

macro (new_mu_script script package)
    configure_file(${script} ${MUSCRIPT_OUTPUT_PATH}/${package}/${script} COPYONLY)
    install(FILES ${script} DESTINATION ${MUSCRIPT_RELATIVE_DIR}/${package}/)
endmacro ()

macro (new_nv_script script package)
    configure_file(${script} ${NVSCRIPT_OUTPUT_PATH}/${package}/${script} COPYONLY)
    install(FILES ${script} DESTINATION ${NVSCRIPT_RELATIVE_DIR}/${package}/)
endmacro ()