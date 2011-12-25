if (USE_JAVA)
    add_subdirectory(Java)
endif ()

if (USE_NTL_FLINT)
    add_subdirectory(NTL)
    add_subdirectory(flint)
endif ()

if (USE_YAFU)
    add_subdirectory(yafu)
endif ()

if (USE_PYTHON)
    add_subdirectory(Python)
endif ()

if (USE_LUA)
    add_subdirectory(Lua)
endif ()

if (USE_ECL)
    add_subdirectory(Ecl)
endif ()

if (USE_MU0)
    add_subdirectory(mU0)
endif ()
