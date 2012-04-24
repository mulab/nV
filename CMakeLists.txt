cmake_minimum_required(VERSION 2.8)

project(nV CXX)

# Configurable options

set(PROJECT_BUILD_TYPE Debug)

set(CMAKE_PREFIX_PATH ""
    CACHE STRING "We will try to find all external libraries here")
set(EXTRA_INCLUDE ""
    CACHE STRING "extra directories to find *.h files")

set(USE_JAVA      "NO" CACHE STRING "nVLink - Java")
set(USE_NTL_FLINT "NO" CACHE STRING "nVLink - ntl & flint")
set(USE_YAFU      "NO"  CACHE STRING "nVLink - yafu")
set(USE_PYTHON    "NO"  CACHE STRING "nVLink - Python")
set(USE_LUA       "NO" CACHE STRING "nVLink - Lua")
set(USE_ECL       "NO"  CACHE STRING "nVLink - Ecl")
set(USE_MU0       "NO"  CACHE STRING "nVLink - mU0")

set(PROJECT_STAGE_DIR ${PROJECT_BINARY_DIR}/${PROJECT_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_STAGE_DIR}/bin)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIBRARY_OUTPUT_PATH ${PROJECT_STAGE_DIR}/bin)
else ()
    set(LIBRARY_OUTPUT_PATH ${PROJECT_STAGE_DIR}/lib)
endif ()
set(MUSCRIPT_RELATIVE_DIR "lib/mU/")
set(NVSCRIPT_RELATIVE_DIR "lib/nV/")
set(MUSCRIPT_OUTPUT_PATH ${PROJECT_STAGE_DIR}/${MUSCRIPT_RELATIVE_DIR})
set(NVSCRIPT_OUTPUT_PATH ${PROJECT_STAGE_DIR}/${NVSCRIPT_RELATIVE_DIR})

# Import utility macros

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

include (Utils)

# Overall -I s

include_directories (BEFORE ${PROJECT_SOURCE_DIR}/Include/external)
include_directories (${PROJECT_SOURCE_DIR}/Include)
include_directories (${EXTRA_INCLUDE})

# Project Headers
file(GLOB_RECURSE PROJECT_HEADERS "Include/*.h")

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

# subdirectory

add_subdirectory(mU)
add_subdirectory(mUKernel)
add_subdirectory(mUSystem)
add_subdirectory(mUScript)
add_subdirectory(mULink)

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
