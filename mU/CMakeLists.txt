new_executable (mu)

target_link_libraries(mu mukernel)

# configure 'mU.ini'

if (USE_NTL_FLINT)
    set (INI_USE_NTL_FLINT True)
else ()
    set (INI_USE_NTL_FLINT False)
endif ()

if (USE_YAFU)
    set (INI_USE_YAFU True)
else ()
    set (INI_USE_YAFU False)
endif ()

configure_file (mU.ini.in mU.ini)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mU.ini DESTINATION bin)
