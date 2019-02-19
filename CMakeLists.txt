# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------- CMAKE PROJECT
# ---------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0054 NEW)
project(aff3ct)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    message(FATAL_ERROR "The '.git' folder can't be found, AFF3CT can't be compiled if it is not cloned "
                        "from a Git repository. Please do not download archives from GitHub and make a Git "
                        "clone instead (git clone https://github.com/aff3ct/aff3ct.git).")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------- CMAKE OPTIONS
# ---------------------------------------------------------------------------------------------------------------------

option(AFF3CT_COMPILE_EXE        "Compile the executable"                                                    ON )
option(AFF3CT_COMPILE_STATIC_LIB "Compile the static library"                                                OFF)
option(AFF3CT_COMPILE_SHARED_LIB "Compile the shared library"                                                OFF)
option(AFF3CT_LINK_GSL           "Link with the GSL library (used in the channels)"                          OFF)
option(AFF3CT_LINK_MKL           "Link with the MKL library (used in the channels)"                          OFF)
option(AFF3CT_SYSTEMC_SIMU       "Enable the SystemC simulation (incompatible with the library compilation)" OFF)
option(AFF3CT_SYSTEMC_MODULE     "Enable the SystemC support (only for the modules)"                         OFF)
option(AFF3CT_MPI                "Enable the MPI support"                                                    OFF)
option(AFF3CT_POLAR_BIT_PACKING  "Enable the bit packing technique for Polar code SC decoding"               ON )
option(AFF3CT_POLAR_BOUNDS       "Enable the use of the external Tal & Vardy Polar best channels generator"  OFF)
option(AFF3CT_COLORS             "Enable the colors in the terminal"                                         ON )

if(NOT (WIN32 OR APPLE))
    option(AFF3CT_BACKTRACE "Enable the backtrace display when an exception is raised" ON)
else()
    set(AFF3CT_BACKTRACE OFF)
endif()

if(MSVC)
    set(AFF3CT_EXT_STRINGS ON)
else()
    option(AFF3CT_EXT_STRINGS "Enable external strings for the help documentation" ON)
endif()

set(AFF3CT_PREC "MULTI" CACHE STRING "Select the precision in bits (can be '8', '16', '32', '64' or 'MULTI')")

if(AFF3CT_SYSTEMC_SIMU AND (AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB))
    message(FATAL_ERROR "It is impossible to compile the AFF3CT library if AFF3CT_SYSTEMC_SIMU='ON'.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------- CMAKE CONFIGURATION
# ---------------------------------------------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MACOSX_RPATH 1)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify bin and lib paths
set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH lib/)

# Generate the source files list
file(GLOB_RECURSE source_files src/*)

# Lists used to generate the CMake config files
set(AFF3CT_DEFINITIONS     "")
set(AFF3CT_INCLUDE_DIRS    "")
set(AFF3CT_LIBRARIES       "")
set(AFF3CT_COMPILE_OPTIONS "")

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------ GET VERSION FROM GIT
# ---------------------------------------------------------------------------------------------------------------------

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_describe(GIT_VERSION "--tags" "--abbrev=7")

string(REGEX REPLACE "^v"                 ""    GIT_VERSION_RMV   ${GIT_VERSION})
string(REGEX REPLACE "-.*"                ""    GIT_VERSION_MMP   ${GIT_VERSION_RMV})
string(REGEX REPLACE "^.*-"               ""    GIT_HASH          ${GIT_VERSION_RMV})
string(      REPLACE "-"                  ""    GIT_BUILD         ${GIT_VERSION_RMV})
string(      REPLACE "${GIT_VERSION_MMP}" ""    GIT_BUILD         ${GIT_BUILD})
string(REGEX REPLACE "(.*)\\..*\\..*"     "\\1" GIT_VERSION_MAJOR ${GIT_VERSION_MMP})
string(REGEX REPLACE ".*\\.(.*)\\..*"     "\\1" GIT_VERSION_MINOR ${GIT_VERSION_MMP})
string(REGEX REPLACE ".*\\..*\\.(.*)"     "\\1" GIT_VERSION_PATCH ${GIT_VERSION_MMP})

if(NOT "${GIT_BUILD}" STREQUAL "")
    string(REPLACE "${GIT_HASH}" "" GIT_BUILD ${GIT_BUILD})
else()
    set(GIT_HASH "")
endif()

message(STATUS "AFF3CT - Git Version: ${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}")
if(NOT "${GIT_BUILD}" STREQUAL "")
    message(STATUS "AFF3CT - Git Build: ${GIT_BUILD}")
endif()
if(NOT "${GIT_HASH}" STREQUAL "")
    message(STATUS "AFF3CT - Git Hash: ${GIT_HASH}")
endif()

if(("${GIT_BUILD}" STREQUAL "") OR ("${GIT_HASH}" STREQUAL ""))
    set(GIT_TAIL "")
else()
    set(GIT_TAIL "-${GIT_BUILD}-${GIT_HASH}")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------- GENERATE VERSION FILE
# ---------------------------------------------------------------------------------------------------------------------

# Auto generate a header containing the last GIT SHA1
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp" @ONLY)

list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp")

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------- GENERATE STRINGS FILE
# ---------------------------------------------------------------------------------------------------------------------

if(NOT AFF3CT_EXT_STRINGS)
    file(READ doc/sphinx/strings.rst AFF3CT_STRINGS)

    string(REPLACE "\\" "\\\\" AFF3CT_STRINGS "${AFF3CT_STRINGS}")
    string(REPLACE "\"" "\\\"" AFF3CT_STRINGS "${AFF3CT_STRINGS}")
    string(REPLACE "\n" "\\n"  AFF3CT_STRINGS "${AFF3CT_STRINGS}")

    # Auto generate a file containing the doc strings
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/Documentation/strings.cpp.in"
                   "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/Documentation/strings.cpp" @ONLY)

    list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/Documentation/strings.cpp")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------- GENERATE AFF3CT SHORTCUT FILE
# ---------------------------------------------------------------------------------------------------------------------

if(AFF3CT_COMPILE_EXE AND UNIX)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/aff3ct" "bin/aff3ct" @ONLY)
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------ CREATE FILTERS
# ---------------------------------------------------------------------------------------------------------------------

# Filters creation for IDEs (tested on Visual Studio and based on the "source_group" function)
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if(IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${source_files})

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------ COMPILER OPTIONS AND DEFINITIONS
# ---------------------------------------------------------------------------------------------------------------------

macro(aff3ct_add_definitions def)
    add_definitions(${def})
    list(APPEND AFF3CT_DEFINITIONS ${def})
endmacro()

macro(aff3ct_add_compile_options opt)
    add_compile_options(${opt})
    list(APPEND AFF3CT_COMPILE_OPTIONS ${opt})
endmacro()

# set -fpic ON
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# by compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    aff3ct_add_compile_options(-Wno-overloaded-virtual)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    aff3ct_add_compile_options(-pthread)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    aff3ct_add_compile_options(-pthread)
endif()

# by system
if(WIN32) # for Windows operating system in general
    set(WINDOWS_VISTA 0x0600)
    aff3ct_add_definitions(-D_WIN32_WINNT=${WINDOWS_VISTA})
    aff3ct_add_definitions(-DNOMINMAX)
    message(STATUS "AFF3CT - System: Windows")
elseif(APPLE) # for macOS
    message(STATUS "AFF3CT - System: macOS")
elseif(UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix
    message(STATUS "AFF3CT - System: Unix/Linux")
endif()

# common
if(AFF3CT_POLAR_BIT_PACKING)
    aff3ct_add_definitions(-DAFF3CT_POLAR_BIT_PACKING)
    message(STATUS "AFF3CT - Polar bit packing: on")
else()
    message(STATUS "AFF3CT - Polar bit packing: off")
endif()
if(AFF3CT_POLAR_BOUNDS)
    aff3ct_add_definitions(-DAFF3CT_POLAR_BOUNDS)
    message(STATUS "AFF3CT - Polar bounds: on")
else()
    message(STATUS "AFF3CT - Polar bounds: off")
endif()
if(AFF3CT_COLORS)
    aff3ct_add_definitions(-DAFF3CT_COLORS)
    message(STATUS "AFF3CT - Terminal colors: on")
else()
    message(STATUS "AFF3CT - Terminal colors: off")
endif()
if(AFF3CT_BACKTRACE)
    aff3ct_add_definitions(-DAFF3CT_BACKTRACE)
    message(STATUS "AFF3CT - Backtrace: on")
else()
    message(STATUS "AFF3CT - Backtrace: off")
endif()
if(AFF3CT_EXT_STRINGS)
    aff3ct_add_definitions(-DAFF3CT_EXT_STRINGS)
    message(STATUS "AFF3CT - External strings: on")
else()
    message(STATUS "AFF3CT - External strings: off")
endif()

if(AFF3CT_PREC STREQUAL "8")
    aff3ct_add_definitions(-DAFF3CT_8BIT_PREC)
    message(STATUS "AFF3CT - Precision: 8-bit")
elseif(AFF3CT_PREC STREQUAL "16")
    aff3ct_add_definitions(-DAFF3CT_16BIT_PREC)
    message(STATUS "AFF3CT - Precision: 16-bit")
elseif(AFF3CT_PREC STREQUAL "32")
    aff3ct_add_definitions(-DAFF3CT_32BIT_PREC)
    message(STATUS "AFF3CT - Precision: 32-bit")
elseif(AFF3CT_PREC STREQUAL "64")
    aff3ct_add_definitions(-DAFF3CT_64BIT_PREC)
    message(STATUS "AFF3CT - Precision: 64-bit")
elseif(AFF3CT_PREC STREQUAL "MULTI")
    aff3ct_add_definitions(-DAFF3CT_MULTI_PREC)
    message(STATUS "AFF3CT - Precision: 8/16/32/64-bit")
else()
    message(FATAL_ERROR "AFF3CT_PREC='${AFF3CT_PREC}' and should be '8', '16', '32', '64' or 'MULTI'.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------- HEADER ONLY LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

# AFF3CT headers
include_directories(src)

# MSVC dirent.h
if(MSVC)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC/include/")
endif()

# MIPP
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/mipp.h")
    aff3ct_add_definitions(-DMIPP_ENABLE_BACKTRACE)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/")
    message(STATUS "AFF3CT - Header found: MIPP")
else()
    message(FATAL_ERROR "AFF3CT - MIPP can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/MIPP/")
endif()

# rang
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/rang.hpp")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/")
    message(STATUS "AFF3CT - Header found: rang")
else()
    message(FATAL_ERROR "AFF3CT - rang can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/rang/")
endif()

# date
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/date.h")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/")
    message(STATUS "AFF3CT - Header found: date")
else()
    message(FATAL_ERROR "AFF3CT - date can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/date/")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------- OBJECTS/LIBS/EXE
# ---------------------------------------------------------------------------------------------------------------------

# Object
add_library(aff3ct-obj OBJECT ${source_files})

# Binary
if(AFF3CT_COMPILE_EXE)
    add_executable(aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-bin PROPERTIES OUTPUT_NAME aff3ct-${GIT_VERSION_RMV})
    message(STATUS "AFF3CT - Compile: executable")
endif(AFF3CT_COMPILE_EXE)

# Library
if(AFF3CT_COMPILE_SHARED_LIB)
    add_library(aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-shared-lib PROPERTIES OUTPUT_NAME aff3ct-${GIT_VERSION_RMV})
    message(STATUS "AFF3CT - Compile: shared library")
endif(AFF3CT_COMPILE_SHARED_LIB)
if(AFF3CT_COMPILE_STATIC_LIB)
    add_library(aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-static-lib PROPERTIES OUTPUT_NAME aff3ct-${GIT_VERSION_RMV})
    message(STATUS "AFF3CT - Compile: static library")
endif(AFF3CT_COMPILE_STATIC_LIB)

macro(aff3ct_include_directories dir)
    include_directories(${dir})
    list(APPEND AFF3CT_INCLUDE_DIRS ${dir})
endmacro()

macro(aff3ct_link_libraries lib)
    if(AFF3CT_COMPILE_EXE)
        target_link_libraries(aff3ct-bin ${lib})
    endif(AFF3CT_COMPILE_EXE)
    if(AFF3CT_COMPILE_SHARED_LIB)
       target_link_libraries(aff3ct-shared-lib ${lib})
    endif(AFF3CT_COMPILE_SHARED_LIB)
    if(AFF3CT_COMPILE_STATIC_LIB)
        target_link_libraries(aff3ct-static-lib ${lib})
    endif(AFF3CT_COMPILE_STATIC_LIB)
    list(APPEND AFF3CT_LIBRARIES ${lib})
endmacro()

# ---------------------------------------------------------------------------------------------------------------------
# -------------------------------------------------------------------------------------------------- COMPILED LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

# SystemC (it is very important to have SystemC link before the other libraries !!!)
if(AFF3CT_SYSTEMC_SIMU OR AFF3CT_SYSTEMC_MODULE)
    if(AFF3CT_SYSTEMC_SIMU)
        aff3ct_add_definitions("-DAFF3CT_SYSTEMC_SIMU")
        aff3ct_add_definitions("-DAFF3CT_SYSTEMC_MODULE")
    else()
        aff3ct_add_definitions("-DAFF3CT_SYSTEMC_MODULE")
    endif()

    find_package(SystemC REQUIRED QUIET)
    if(SystemC_FOUND)
        message(STATUS "AFF3CT - Library found: SystemC")
        aff3ct_include_directories("${SystemC_INCLUDE_DIRS}")
        aff3ct_link_libraries("${SystemC_LIBRARIES}")
    endif(SystemC_FOUND)

    find_package(TLM REQUIRED QUIET)
    if(TLM_FOUND)
        message(STATUS "AFF3CT - Library found: TLM")
        aff3ct_include_directories("${TLM_INCLUDE_DIRS}")
    endif(TLM_FOUND)
endif()

# GSL
if(AFF3CT_LINK_GSL)
    aff3ct_add_definitions("-DAFF3CT_CHANNEL_GSL")

    find_package(GSL REQUIRED QUIET)
    if(GSL_FOUND)
        message(STATUS "AFF3CT - Library found: GSL")
        aff3ct_include_directories("${GSL_INCLUDE_DIRS}")
        aff3ct_link_libraries("${GSL_LIBRARIES}")
    endif(GSL_FOUND)
endif(AFF3CT_LINK_GSL)

# MKL
if(AFF3CT_LINK_MKL)
    aff3ct_add_definitions("-DAFF3CT_CHANNEL_MKL")

    # set (MKL_STATIC ON)
    find_package(MKL REQUIRED QUIET)
    if(MKL_FOUND)
        message(STATUS "AFF3CT - Library found: MKL")
        aff3ct_include_directories("${MKL_INCLUDE_DIRS}")
        aff3ct_link_libraries("${MKL_MINIMAL_LIBRARIES}")
    endif(MKL_FOUND)
endif(AFF3CT_LINK_MKL)

# MPI
if(AFF3CT_MPI)
    aff3ct_add_definitions("-DAFF3CT_MPI")

    find_package(MPI REQUIRED QUIET)
    if(MPI_CXX_FOUND)

        message(STATUS "AFF3CT - Library found: MPI")
        aff3ct_include_directories("${MPI_CXX_INCLUDE_PATH}")
        aff3ct_link_libraries("${MPI_CXX_LIBRARIES}")

        if(MPI_CXX_COMPILE_FLAGS)
            if(AFF3CT_COMPILE_EXE)
                set_target_properties(aff3ct-bin PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif(AFF3CT_COMPILE_EXE)
            if(AFF3CT_COMPILE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif(AFF3CT_COMPILE_SHARED_LIB)
            if(AFF3CT_COMPILE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif(AFF3CT_COMPILE_STATIC_LIB)
        endif(MPI_CXX_COMPILE_FLAGS)

        if(MPI_CXX_LINK_FLAGS)
            if(AFF3CT_COMPILE_EXE)
                set_target_properties(aff3ct-bin PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif(AFF3CT_COMPILE_EXE)
            if(AFF3CT_COMPILE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif(AFF3CT_COMPILE_SHARED_LIB)
            if(AFF3CT_COMPILE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif(AFF3CT_COMPILE_STATIC_LIB)
        endif(MPI_CXX_LINK_FLAGS)

    endif(MPI_CXX_FOUND)
endif(AFF3CT_MPI)

# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
aff3ct_link_libraries(Threads::Threads)

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------- GENERATE CMAKE CONFIG FILES
# ---------------------------------------------------------------------------------------------------------------------

# Auto generate cmake config files to link with AFF3CT library (only if an AFF3CT library has been compiled)
if(AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/aff3ct-config.cmake.in"
                   "lib/cmake/aff3ct-${GIT_VERSION_RMV}/aff3ct-config.cmake" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/aff3ct-config-version.cmake.in"
                   "lib/cmake/aff3ct-${GIT_VERSION_RMV}/aff3ct-config-version.cmake" @ONLY)
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------- INSTALL
# ---------------------------------------------------------------------------------------------------------------------

# set CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR and CMAKE_INSTALL_DATAROOTDIR variables
include(GNUInstallDirs)

if(AFF3CT_COMPILE_EXE)
    install(TARGETS aff3ct-bin RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/)
    if(UNIX)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/aff3ct" DESTINATION ${CMAKE_INSTALL_BINDIR}/
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    endif()
endif()
if(AFF3CT_COMPILE_SHARED_LIB)
    install(TARGETS aff3ct-shared-lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
endif()
if(AFF3CT_COMPILE_STATIC_LIB)
    install(TARGETS aff3ct-static-lib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
endif()

if(AFF3CT_COMPILE_SHARED_LIB OR AFF3CT_COMPILE_STATIC_LIB)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    install(DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/aff3ct-${GIT_VERSION_RMV}"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/latex/AFF3CT.pdf")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/latex/AFF3CT.pdf"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${GIT_VERSION_RMV}/doc/pdf)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/html/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/html/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${GIT_VERSION_RMV}/doc/html)
endif()
if(AFF3CT_EXT_STRINGS)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/strings.rst"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${GIT_VERSION_RMV}/doc/strings)
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.hxx")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/date
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/MIPP
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/MIPP
        FILES_MATCHING PATTERN "*.hxx")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/dirent
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${GIT_VERSION_RMV}/rang
        FILES_MATCHING PATTERN "*.hpp")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/conf/cde/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/conf/" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${GIT_VERSION_RMV}/conf
            FILES_MATCHING PATTERN "*")
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/refs/BCH/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/refs/" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${GIT_VERSION_RMV}/refs
            FILES_MATCHING PATTERN "*.txt"
            PATTERN "readers"   EXCLUDE
            PATTERN "untracked" EXCLUDE)
endif()
