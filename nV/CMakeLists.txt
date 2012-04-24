new_executable (nv)

target_link_libraries(nv nvparser)

# configure 'nV.ini'

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

configure_file (nV.ini.in ${CONF_OUTPUT_PATH}/nV.ini)

install (FILES ${CONF_OUTPUT_PATH}/nV.ini DESTINATION ${CONF_RELATIVE_DIR})