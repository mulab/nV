cmake_minimum_required(VERSION 2.8)

project(nV CXX)
set (VERSION_MAJOR "0")
set (VERSION_MINOR "5")
set (VERSION_PATCH "0")

# Configurable options

include (config.cmake)

if (MSVC_IDE)
	set(PROJECT_STAGE_DIR "${PROJECT_BINARY_DIR}/stage")
else ()
	set(PROJECT_STAGE_DIR "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif ()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_STAGE_DIR}/bin")
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_STAGE_DIR}/bin")
	set(CONF_RELATIVE_DIR "conf/")
else ()
    set(LIBRARY_OUTPUT_PATH "${PROJECT_STAGE_DIR}/lib")
	set(CONF_RELATIVE_DIR "etc/")
endif ()
set(CONF_OUTPUT_PATH "${PROJECT_STAGE_DIR}/${CONF_RELATIVE_DIR}")
set(MUSCRIPT_RELATIVE_DIR "lib/mU/")
set(NVSCRIPT_RELATIVE_DIR "lib/nV/")
set(MUSCRIPT_OUTPUT_PATH "${PROJECT_STAGE_DIR}/${MUSCRIPT_RELATIVE_DIR}")
set(NVSCRIPT_OUTPUT_PATH "${PROJECT_STAGE_DIR}/${NVSCRIPT_RELATIVE_DIR}")

# Import utility macros

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

include (Utils)

# Overall -I s

include_directories (BEFORE ${PROJECT_SOURCE_DIR}/Include/external)
include_directories (${PROJECT_SOURCE_DIR}/Include)
if (MSVC_IDE)
	include_directories(${PROJECT_BINARY_DIR}/Include/$(Configuration))
else ()
	include_directories(${PROJECT_BINARY_DIR}/Include)
endif ()
include_directories (${EXTRA_INCLUDE})

# Project Headers
if (MSVC_IDE)
	set (MU_HOME ${CMAKE_INSTALL_PREFIX})
	set (NV_HOME ${CMAKE_INSTALL_PREFIX})
	foreach (release_configuration "Release" "RelWithDebInfo" "MinSizeRel")
		configure_file(Include/mU/common.h.in Include/${release_configuration}/mU/common.h)
		configure_file(Include/nV/Config.h.in Include/${release_configuration}/nV/Config.h)
	endforeach ()
	set (MU_HOME ${PROJECT_STAGE_DIR})
	set (NV_HOME ${PROJECT_STAGE_DIR})
	configure_file(Include/mU/common.h.in Include/Debug/mU/common.h)
	configure_file(Include/nV/Config.h.in Include/Debug/nV/Config.h)
else ()
	if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set (MU_HOME ${PROJECT_STAGE_DIR})
		set (NV_HOME ${PROJECT_STAGE_DIR})
		configure_file(Include/mU/common.h.in Include/mU/common.h)
		configure_file(Include/nV/Config.h.in Include/nV/Config.h)
	else ()
		set (MU_HOME ${CMAKE_INSTALL_PREFIX})
		set (NV_HOME ${CMAKE_INSTALL_PREFIX})
		configure_file(Include/mU/common.h.in Include/mU/common.h)
		configure_file(Include/nV/Config.h.in Include/nV/Config.h)
	endif ()
endif ()

file(GLOB_RECURSE PROJECT_HEADERS "Include/*.h" "${PROJECT_BINARY_DIR}/Include/*.h")

# Definitions
add_definitions(-DUNICODE)

# Depenences

find_package (Boost REQUIRED thread system date_time regex)
include_directories(${Boost_INCLUDE_DIRS})
find_library (gmp_LIBRARIES gmp REQUIRED)
find_library (mpfr_LIBRARIES mpfr REQUIRED)
find_library (mpfi_LIBRARIES mpfi REQUIRED)

mark_as_advanced (gmp_LIBRARIES)
mark_as_advanced (mpfr_LIBRARIES)
mark_as_advanced (mpfi_LIBRARIES)

if (USE_JAVA)
    find_package (JNI REQUIRED)
    
    include_directories(${JNI_INCLUDE_DIRS})
endif ()

if (USE_NTL_FLINT)
    find_library (ntl_LIBRARIES ntl REQUIRED)
    find_library (flint_LIBRARIES flint REQUIRED)

    mark_as_advanced (ntl_LIBRARIES)
    mark_as_advanced (flint_LIBRARIES)
endif ()

if (USE_YAFU)
    find_library (yafu_LIBRARIES yafu REQUIRED)

    mark_as_advanced (yafu_LIBRARIES)
endif ()

if (USE_PYTHON)
    find_package (PythonLibs REQUIRED)
endif ()

if (USE_LUA)
    find_package (Lua51 REQUIRED)
    find_library (luabind_LIBRARIES luabind REQUIRED)

    mark_as_advanced (luabind_LIBRARIES)
endif ()

if (USE_ECL)
    find_library (ecl_LIBRARIES ecl REQUIRED)

    mark_as_advanced (ecl_LIBRARIES)
endif ()

if (USE_MU0)
    find_library (mU0_LIBRARIES mU0 REQUIRED)
    find_library (mU0_kernel_LIBRARIES kernel REQUIRED)
    set(mU0_LIBRARIES ${mU0_LIBRARIES} ${mU0_kernel_LIBRARIES})

    mark_as_advanced(mU0_LIBRARIES)
    mark_as_advanced(mU0_kernel_LIBRARIES)
endif ()

# NSIS installer script
if (MSVC_IDE)
	string(REPLACE ".lib" ".dll" Boost_THREAD_LIBRARY_RELEASE_DLL ${Boost_THREAD_LIBRARY_RELEASE})
	file(TO_NATIVE_PATH ${Boost_THREAD_LIBRARY_RELEASE_DLL} Boost_THREAD_LIBRARY_RELEASE_DLL)
	string(REPLACE ".lib" ".dll" Boost_SYSTEM_LIBRARY_RELEASE_DLL ${Boost_SYSTEM_LIBRARY_RELEASE})
	file(TO_NATIVE_PATH ${Boost_SYSTEM_LIBRARY_RELEASE_DLL} Boost_SYSTEM_LIBRARY_RELEASE_DLL)
	string(REPLACE ".lib" ".dll" Boost_DATE_TIME_LIBRARY_RELEASE_DLL ${Boost_DATE_TIME_LIBRARY_RELEASE})
	file(TO_NATIVE_PATH ${Boost_DATE_TIME_LIBRARY_RELEASE_DLL} Boost_DATE_TIME_LIBRARY_RELEASE_DLL)
	string(REPLACE ".lib" ".dll" Boost_REGEX_LIBRARY_RELEASE_DLL ${Boost_REGEX_LIBRARY_RELEASE})
	file(TO_NATIVE_PATH ${Boost_REGEX_LIBRARY_RELEASE_DLL} Boost_REGEX_LIBRARY_RELEASE_DLL)
	
	string(REPLACE ".lib" ".dll" gmp_LIBRARIES_DLL ${gmp_LIBRARIES})
	file(TO_NATIVE_PATH ${gmp_LIBRARIES_DLL} gmp_LIBRARIES_DLL)

	string(REPLACE ".lib" ".dll" mpfr_LIBRARIES_DLL ${mpfr_LIBRARIES})
	file(TO_NATIVE_PATH ${mpfr_LIBRARIES_DLL} mpfr_LIBRARIES_DLL)

	string(REPLACE ".lib" ".dll" mpfi_LIBRARIES_DLL ${mpfi_LIBRARIES})
	file(TO_NATIVE_PATH ${mpfi_LIBRARIES_DLL} mpfi_LIBRARIES_DLL)

	configure_file(EnvVarUpdate.nsh EnvVarUpdate.nsh COPYONLY)
	configure_file(setup.nsi.in setup.nsi @ONLY)
endif ()

# subdirectory

add_subdirectory(mU)
add_subdirectory(mUKernel)
add_subdirectory(mUSystem)
add_subdirectory(mUScript)
add_subdirectory(mULink)
add_subdirectory(mUTest)

add_subdirectory(nV)
add_subdirectory(nVKernel)
add_subdirectory(nVNumber)
add_subdirectory(nVParser)
add_subdirectory(nVPolynomial)
add_subdirectory(nVMatrix)
add_subdirectory(nVSystem)
add_subdirectory(nVCalculus)
add_subdirectory(nVGraphics)
add_subdirectory(nVString)
add_subdirectory(nVTest)
add_subdirectory(nVLink)

# from orignal CMake files

#set(USE_GOOGLE_PERFTOOLS 1)
#if(USE_GOOGLE_PERFTOOLS)
#  set(CMAKE_EXE_LINKER_FLAGS_PROF "-lprofiler -ltcmalloc")
#  set(CMAKE_SHARED_LINKER_FLAGS_PROF="-lprofiler -ltcmalloc")
#  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-ltcmalloc")
#  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE="-ltcmalloc")
#endif()

# generating packages with CPack
include (Packaging)
