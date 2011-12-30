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

configure_file (nV.ini.in nV.ini)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/nV.ini DESTINATION bin)
install (FILES nV.reg DESTINATION nV)
