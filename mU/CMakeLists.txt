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

configure_file (mU.ini.in ${EXECUTABLE_OUTPUT_PATH}/mU.ini)

install(FILES ${EXECUTABLE_OUTPUT_PATH}/mU.ini DESTINATION bin)
