# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------- CMAKE PROJECT
# ---------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0054 NEW)

if(${CMAKE_VERSION} VERSION_GREATER "3.13")
    cmake_policy(SET CMP0083 NEW)
endif()

project(aff3ct)


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
option(AFF3CT_OVERRIDE_VERSION   "Compile without .git directory, provided a version and hash"               OFF)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" AND NOT AFF3CT_OVERRIDE_VERSION)
    message(FATAL_ERROR "The '.git' folder can't be found, AFF3CT can't be compiled if it is not cloned "
                        "from a Git repository. Please do not download archives from GitHub and make a Git "
                        "clone instead (git clone https://github.com/aff3ct/aff3ct.git).")
endif()

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

if(AFF3CT_MPI AND (AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB))
    message(FATAL_ERROR "Building AFF3CT with the MPI support is incompatible with the library mode.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------- CMAKE CONFIGURATION
# ---------------------------------------------------------------------------------------------------------------------

# set CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR and CMAKE_INSTALL_DATAROOTDIR variables
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MACOSX_RPATH 1)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify bin and lib paths
set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH lib/)

# Generate the source files list
file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------ GET VERSION FROM GIT
# ---------------------------------------------------------------------------------------------------------------------

if(NOT AFF3CT_OVERRIDE_VERSION)
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    git_describe(GIT_VERSION "--tags" "--abbrev=7")
else()
    set(GIT_VERSION ${AFF3CT_OVERRIDE_VERSION})
endif()



string(REGEX REPLACE "^v"                ""    AFF3CT_VERSION_FULL  ${GIT_VERSION})
string(REGEX REPLACE "-.*"               ""    AFF3CT_VERSION       ${AFF3CT_VERSION_FULL})
string(REGEX REPLACE "^.*-"              ""    AFF3CT_HASH          ${AFF3CT_VERSION_FULL})
string(      REPLACE "-"                 ""    AFF3CT_BUILD         ${AFF3CT_VERSION_FULL})
string(      REPLACE "${AFF3CT_VERSION}" ""    AFF3CT_BUILD         ${AFF3CT_BUILD})
string(REGEX REPLACE "(.*)\\..*\\..*"    "\\1" AFF3CT_VERSION_MAJOR ${AFF3CT_VERSION})
string(REGEX REPLACE ".*\\.(.*)\\..*"    "\\1" AFF3CT_VERSION_MINOR ${AFF3CT_VERSION})
string(REGEX REPLACE ".*\\..*\\.(.*)"    "\\1" AFF3CT_VERSION_PATCH ${AFF3CT_VERSION})

if(NOT "${AFF3CT_BUILD}" STREQUAL "")
    string(REPLACE "${AFF3CT_HASH}" "" AFF3CT_BUILD ${AFF3CT_BUILD})
else()
    set(AFF3CT_HASH "")
endif()

message(STATUS "AFF3CT - Version: ${AFF3CT_VERSION}")
if(NOT "${AFF3CT_BUILD}" STREQUAL "")
    message(STATUS "AFF3CT - Build: ${AFF3CT_BUILD}")
endif()
if(NOT "${AFF3CT_HASH}" STREQUAL "")
    message(STATUS "AFF3CT - Hash: ${AFF3CT_HASH}")
endif()

if(("${AFF3CT_BUILD}" STREQUAL "") OR ("${AFF3CT_HASH}" STREQUAL ""))
    set(AFF3CT_VERSION_EXTRA "")
else()
    set(AFF3CT_VERSION_EXTRA "-${AFF3CT_BUILD}-${AFF3CT_HASH}")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------ GENERATE FILES
# ---------------------------------------------------------------------------------------------------------------------

# ------------------------------------------------------------------------------------------------------------- VERSION
# Auto generate a header containing the last Git SHA1
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp" @ONLY)
list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp")

# Auto generate cmake config version file to link with AFF3CT library (only if an AFF3CT library has been compiled)
if (AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/aff3ct-config-version.cmake.in"
                   "lib/cmake/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct-config-version.cmake" @ONLY)
endif()

# ------------------------------------------------------------------------------------------------------------- STRINGS
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

# ------------------------------------------------------------------------------------------------------------ SHORTCUT
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
# ---------------------------------------------------------------------------------------------------- OBJECTS/LIBS/EXE
# ---------------------------------------------------------------------------------------------------------------------

if(${CMAKE_VERSION} VERSION_GREATER "3.13")
    include(CheckPIESupported)
    check_pie_supported()
endif()

# Object
add_library(aff3ct-obj OBJECT ${source_files})
set_target_properties(aff3ct-obj PROPERTIES
                                 POSITION_INDEPENDENT_CODE ON) # set -fpic

# Binary
if(AFF3CT_COMPILE_EXE)
    add_executable(aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-bin PROPERTIES
                                     OUTPUT_NAME aff3ct-${AFF3CT_VERSION_FULL}
                                     POSITION_INDEPENDENT_CODE ON) # set -fpie
    message(STATUS "AFF3CT - Compile: executable")
endif(AFF3CT_COMPILE_EXE)

# Library
if(AFF3CT_COMPILE_SHARED_LIB)
    add_library(aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-shared-lib PROPERTIES
                                            OUTPUT_NAME aff3ct-${AFF3CT_VERSION_FULL}
                                            POSITION_INDEPENDENT_CODE ON) # set -fpic
    message(STATUS "AFF3CT - Compile: shared library")
endif(AFF3CT_COMPILE_SHARED_LIB)
if(AFF3CT_COMPILE_STATIC_LIB)
    add_library(aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties(aff3ct-static-lib PROPERTIES
                                            OUTPUT_NAME aff3ct-${AFF3CT_VERSION_FULL}
                                            POSITION_INDEPENDENT_CODE ON) # set -fpic
    message(STATUS "AFF3CT - Compile: static library")
endif(AFF3CT_COMPILE_STATIC_LIB)

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------ COMPILER DEFINITIONS
# ---------------------------------------------------------------------------------------------------------------------

macro(aff3ct_target_compile_definitions def)
    target_compile_definitions(aff3ct-obj PUBLIC $<BUILD_INTERFACE:${def}> $<INSTALL_INTERFACE:${def}>)
    if(AFF3CT_COMPILE_EXE)
        target_compile_definitions(aff3ct-bin PUBLIC $<BUILD_INTERFACE:${def}> $<INSTALL_INTERFACE:${def}>)
    endif()
    if(AFF3CT_COMPILE_SHARED_LIB)
        target_compile_definitions(aff3ct-shared-lib PUBLIC $<BUILD_INTERFACE:${def}> $<INSTALL_INTERFACE:${def}>)
    endif()
    if(AFF3CT_COMPILE_STATIC_LIB)
        target_compile_definitions(aff3ct-static-lib PUBLIC $<BUILD_INTERFACE:${def}> $<INSTALL_INTERFACE:${def}>)
    endif()
endmacro()

# by system
if(WIN32) # for Windows operating system in general
    set(WINDOWS_VISTA 0x0600)
    aff3ct_target_compile_definitions(_WIN32_WINNT=${WINDOWS_VISTA})
    aff3ct_target_compile_definitions(NOMINMAX)
    message(STATUS "AFF3CT - System: Windows")
elseif(APPLE) # for macOS
    message(STATUS "AFF3CT - System: macOS")
elseif(UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix
    message(STATUS "AFF3CT - System: Unix/Linux")
endif()

# common
if(AFF3CT_POLAR_BIT_PACKING)
    aff3ct_target_compile_definitions(AFF3CT_POLAR_BIT_PACKING)
    message(STATUS "AFF3CT - Polar bit packing: on")
else()
    message(STATUS "AFF3CT - Polar bit packing: off")
endif()
if(AFF3CT_POLAR_BOUNDS)
    aff3ct_target_compile_definitions(AFF3CT_POLAR_BOUNDS)
    message(STATUS "AFF3CT - Polar bounds: on")
else()
    message(STATUS "AFF3CT - Polar bounds: off")
endif()
if(AFF3CT_COLORS)
    aff3ct_target_compile_definitions(AFF3CT_COLORS)
    message(STATUS "AFF3CT - Terminal colors: on")
else()
    message(STATUS "AFF3CT - Terminal colors: off")
endif()
if(AFF3CT_BACKTRACE)
    aff3ct_target_compile_definitions(AFF3CT_BACKTRACE)
    message(STATUS "AFF3CT - Backtrace: on")
else()
    message(STATUS "AFF3CT - Backtrace: off")
endif()
if(AFF3CT_EXT_STRINGS)
    aff3ct_target_compile_definitions(AFF3CT_EXT_STRINGS)
    message(STATUS "AFF3CT - External strings: on")
else()
    message(STATUS "AFF3CT - External strings: off")
endif()

if(AFF3CT_PREC STREQUAL "8")
    aff3ct_target_compile_definitions(AFF3CT_8BIT_PREC)
    message(STATUS "AFF3CT - Precision: 8-bit")
elseif(AFF3CT_PREC STREQUAL "16")
    aff3ct_target_compile_definitions(AFF3CT_16BIT_PREC)
    message(STATUS "AFF3CT - Precision: 16-bit")
elseif(AFF3CT_PREC STREQUAL "32")
    aff3ct_target_compile_definitions(AFF3CT_32BIT_PREC)
    message(STATUS "AFF3CT - Precision: 32-bit")
elseif(AFF3CT_PREC STREQUAL "64")
    aff3ct_target_compile_definitions(AFF3CT_64BIT_PREC)
    message(STATUS "AFF3CT - Precision: 64-bit")
elseif(AFF3CT_PREC STREQUAL "MULTI")
    aff3ct_target_compile_definitions(AFF3CT_MULTI_PREC)
    message(STATUS "AFF3CT - Precision: 8/16/32/64-bit")
else()
    message(FATAL_ERROR "AFF3CT_PREC='${AFF3CT_PREC}' and should be '8', '16', '32', '64' or 'MULTI'.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------- HEADER ONLY LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

macro(aff3ct_target_include_directories dir_build dir_install)
    target_include_directories(aff3ct-obj PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir_build}/>
                               $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/${dir_install}>)
    if(AFF3CT_COMPILE_EXE)
        target_include_directories(aff3ct-bin PUBLIC
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir_build}/>
                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/${dir_install}>)
    endif()
    if(AFF3CT_COMPILE_SHARED_LIB)
        target_include_directories(aff3ct-shared-lib PUBLIC
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir_build}/>
                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/${dir_install}>)
    endif()
    if(AFF3CT_COMPILE_STATIC_LIB)
        target_include_directories(aff3ct-static-lib PUBLIC
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir_build}/>
                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/${dir_install}>)
    endif()
endmacro()

# AFF3CT headers
aff3ct_target_include_directories("src" "aff3ct")

# MSVC dirent.h
if(MSVC)
    aff3ct_target_include_directories("lib/MSVC/include" "dirent")
endif()

# MIPP
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/mipp.h")
    aff3ct_target_compile_definitions(MIPP_ENABLE_BACKTRACE)
    aff3ct_target_include_directories("lib/MIPP/src" "MIPP")
    message(STATUS "AFF3CT - Header found: MIPP")
else()
    message(FATAL_ERROR "AFF3CT - MIPP can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/MIPP/")
endif()

# rang
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/rang.hpp")
    aff3ct_target_include_directories("lib/rang/include" "rang")
    message(STATUS "AFF3CT - Header found: rang")
else()
    message(FATAL_ERROR "AFF3CT - rang can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/rang/")
endif()

# date
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/date.h")
    aff3ct_target_include_directories("lib/date/include/date" "date")
    message(STATUS "AFF3CT - Header found: date")
else()
    message(FATAL_ERROR "AFF3CT - date can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/date/")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# -------------------------------------------------------------------------------------------------- COMPILED LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

macro(aff3ct_target_link_libraries lib)
    if(AFF3CT_COMPILE_EXE)
        target_link_libraries(aff3ct-bin PUBLIC ${lib})
    endif(AFF3CT_COMPILE_EXE)
    if(AFF3CT_COMPILE_SHARED_LIB)
       target_link_libraries(aff3ct-shared-lib INTERFACE ${lib})
    endif(AFF3CT_COMPILE_SHARED_LIB)
    if(AFF3CT_COMPILE_STATIC_LIB)
        target_link_libraries(aff3ct-static-lib INTERFACE ${lib})
    endif(AFF3CT_COMPILE_STATIC_LIB)
endmacro()

macro(aff3ct_target_include_directories dir)
    target_include_directories(aff3ct-obj PUBLIC $<BUILD_INTERFACE:${dir}> $<INSTALL_INTERFACE:${dir}>)
    if(AFF3CT_COMPILE_EXE)
        target_include_directories(aff3ct-bin PUBLIC $<BUILD_INTERFACE:${dir}> $<INSTALL_INTERFACE:${dir}>)
    endif()
    if(AFF3CT_COMPILE_SHARED_LIB)
        target_include_directories(aff3ct-shared-lib PUBLIC $<BUILD_INTERFACE:${dir}> $<INSTALL_INTERFACE:${dir}>)
    endif()
    if(AFF3CT_COMPILE_STATIC_LIB)
        target_include_directories(aff3ct-static-lib PUBLIC $<BUILD_INTERFACE:${dir}> $<INSTALL_INTERFACE:${dir}>)
    endif()
endmacro()

# SystemC (it is very important to have SystemC linked before the other libraries !!!)
if(AFF3CT_SYSTEMC_SIMU OR AFF3CT_SYSTEMC_MODULE)
    if(AFF3CT_SYSTEMC_SIMU)
        aff3ct_target_compile_definitions("AFF3CT_SYSTEMC_SIMU")
        aff3ct_target_compile_definitions("AFF3CT_SYSTEMC_MODULE")
    else()
        aff3ct_target_compile_definitions("AFF3CT_SYSTEMC_MODULE")
    endif()

    find_package(SystemC REQUIRED QUIET)
    if(SystemC_FOUND)
        message(STATUS "AFF3CT - Library found: SystemC")
        aff3ct_target_include_directories("${SystemC_INCLUDE_DIRS}")
        aff3ct_target_link_libraries("${SystemC_LIBRARIES}")
    endif(SystemC_FOUND)

    find_package(TLM REQUIRED QUIET)
    if(TLM_FOUND)
        message(STATUS "AFF3CT - Library found: TLM")
        aff3ct_target_include_directories("${TLM_INCLUDE_DIRS}")
    endif(TLM_FOUND)
endif()

# GSL
if(AFF3CT_LINK_GSL)
    aff3ct_target_compile_definitions("AFF3CT_CHANNEL_GSL")

    find_package(GSL REQUIRED QUIET)
    if(GSL_FOUND)
        message(STATUS "AFF3CT - Library found: GSL")
        aff3ct_target_link_libraries(GSL::gsl)
    endif(GSL_FOUND)
endif(AFF3CT_LINK_GSL)

# MKL
if(AFF3CT_LINK_MKL)
    aff3ct_target_compile_definitions("AFF3CT_CHANNEL_MKL")

    # set (MKL_STATIC ON)
    find_package(MKL REQUIRED QUIET)
    if(MKL_FOUND)
        message(STATUS "AFF3CT - Library found: MKL")
        aff3ct_target_include_directories("${MKL_INCLUDE_DIRS}")
        aff3ct_target_link_libraries("${MKL_MINIMAL_LIBRARIES}")
    endif(MKL_FOUND)
endif(AFF3CT_LINK_MKL)

# MPI
if(AFF3CT_MPI)
    aff3ct_target_compile_definitions("AFF3CT_MPI")

    find_package(MPI REQUIRED QUIET)
    if(MPI_CXX_FOUND)

        message(STATUS "AFF3CT - Library found: MPI")
        if(${CMAKE_VERSION} VERSION_LESS "3.9.6")
            aff3ct_target_include_directories("${MPI_CXX_INCLUDE_PATH}")
            aff3ct_target_link_libraries("${MPI_CXX_LIBRARIES}")

            if(MPI_CXX_COMPILE_FLAGS)
                if(AFF3CT_COMPILE_EXE)
                    set_target_properties(aff3ct-bin PROPERTIES COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
                endif()
            endif(MPI_CXX_COMPILE_FLAGS)

            if(MPI_CXX_LINK_FLAGS)
                if(AFF3CT_COMPILE_EXE)
                    set_target_properties(aff3ct-bin PROPERTIES LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
                endif()
            endif(MPI_CXX_LINK_FLAGS)
        else()
            aff3ct_target_link_libraries(MPI::MPI_CXX)
        endif()

    endif(MPI_CXX_FOUND)
endif(AFF3CT_MPI)

# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
aff3ct_target_link_libraries(Threads::Threads)

# ---------------------------------------------------------------------------------------------------------------------
# -------------------------------------------------------------------------------------------------------------- EXPORT
# ---------------------------------------------------------------------------------------------------------------------

if (AFF3CT_COMPILE_SHARED_LIB AND NOT AFF3CT_COMPILE_STATIC_LIB)
    export(TARGETS
           aff3ct-shared-lib
           NAMESPACE aff3ct::
           FILE "lib/cmake/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct-config.cmake")
endif()

if (AFF3CT_COMPILE_STATIC_LIB AND NOT AFF3CT_COMPILE_SHARED_LIB)
    export(TARGETS
           aff3ct-static-lib
           NAMESPACE aff3ct::
           FILE "lib/cmake/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct-config.cmake")
endif()

if(AFF3CT_COMPILE_SHARED_LIB AND AFF3CT_COMPILE_STATIC_LIB)
    export(TARGETS
           aff3ct-shared-lib
           aff3ct-static-lib
           NAMESPACE aff3ct::
           FILE "lib/cmake/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct-config.cmake")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------- INSTALL
# ---------------------------------------------------------------------------------------------------------------------

if(AFF3CT_COMPILE_EXE)
    install(TARGETS aff3ct-bin
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/
            COMPONENT simulator)
    if(UNIX)
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/aff3ct" DESTINATION ${CMAKE_INSTALL_BINDIR}/
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
                COMPONENT simulator)
    endif()
endif()
if(AFF3CT_COMPILE_SHARED_LIB)
    if(WIN32)
        install(TARGETS aff3ct-shared-lib
                EXPORT aff3ct-config
                RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/
                COMPONENT library)
    else()
        install(TARGETS aff3ct-shared-lib
                EXPORT aff3ct-config
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/
                COMPONENT library)
    endif()
endif()
if(AFF3CT_COMPILE_STATIC_LIB)
    install(TARGETS aff3ct-static-lib
            EXPORT aff3ct-config
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/
            COMPONENT library)
endif()

if (AFF3CT_COMPILE_SHARED_LIB OR AFF3CT_COMPILE_STATIC_LIB)
    install(EXPORT
            aff3ct-config
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/aff3ct-${AFF3CT_VERSION_FULL}/"
            NAMESPACE aff3ct::
            COMPONENT library)

    install(FILES "${CMAKE_BINARY_DIR}/lib/cmake/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct-config-version.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/aff3ct-${AFF3CT_VERSION_FULL}/"
            COMPONENT library)

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct
            COMPONENT headers
            FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct
            COMPONENT headers
            FILES_MATCHING PATTERN "*.hpp")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/aff3ct
            COMPONENT headers
            FILES_MATCHING PATTERN "*.hxx")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/date
            COMPONENT headers
            FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/MIPP
            COMPONENT headers
            FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/MIPP
            COMPONENT headers
            FILES_MATCHING PATTERN "*.hxx")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC/include/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/dirent
            COMPONENT headers
            FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/aff3ct-${AFF3CT_VERSION_FULL}/rang
            COMPONENT headers
            FILES_MATCHING PATTERN "*.hpp")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/latex/AFF3CT.pdf")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/latex/AFF3CT.pdf"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${AFF3CT_VERSION_FULL}/doc/pdf
            COMPONENT documentation)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/html/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/build/html/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${AFF3CT_VERSION_FULL}/doc/html
            COMPONENT documentation)
endif()
if(AFF3CT_EXT_STRINGS)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/strings.rst"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${AFF3CT_VERSION_FULL}/doc/strings
            COMPONENT documentation)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/conf/cde/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/conf/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${AFF3CT_VERSION_FULL}/conf
            COMPONENT configuration
            FILES_MATCHING PATTERN "*"
            PATTERN ".git*" EXCLUDE)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/refs/BCH/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/refs/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/aff3ct-${AFF3CT_VERSION_FULL}/refs
            COMPONENT references
            FILES_MATCHING PATTERN "*.txt"
            PATTERN "readers" EXCLUDE
            PATTERN "untracked" EXCLUDE
            PATTERN ".git*" EXCLUDE)
endif()

# ---------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------- CPACK
# ---------------------------------------------------------------------------------------------------------------------

# build a CPack driven installer package
include(InstallRequiredSystemLibraries)

# general configuration and package description
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_DEB_COMPONENT_INSTALL ON)
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Adrien Cassagne <adrien.cassagne@inria.fr>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mathieu Léonardon <mathieu.leonardon@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://aff3ct.github.io/")
set(CPACK_DEBIAN_PACKAGE_VERSION "${AFF3CT_VERSION}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_VENDOR "Inria/IMS Bordeaux")
set(CPACK_PACKAGE_CONTACT "Inria/IMS Bordeaux")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_GROUPING "ALL_COMPONENTS_IN_ONE")
set(CPACK_PACKAGE_NAME "aff3ct")
set(CPACK_SOURCE_PACKAGE_NAME "aff3ct")
set(CPACK_PACKAGE_DESCRIPTION "AFF3CT - A Fast Forward Error Correction Toolbox!")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AFF3CT - A Fast Forward Error Correction Toolbox!
                                       AFF3CT is an open source software dedicated to Forward Error Correction
                                       (FEC or channel coding) simulations. It is written in C++ and it supports
                                       a broad range of codes: from the well-spread turbo codes and Low-Density
                                       Parity-Check (LDPC) codes to the more recent polar codes. A particular
                                       emphasis is given to simulation throughput performance (multiple Mb/s on
                                       modern CPUs). AFF3CT can also been used as a FEC library and all the blocks
                                       can be reused separately in various contexts.")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_FILE_NAME "aff3ct_${AFF3CT_VERSION}.orig")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "aff3ct_${AFF3CT_VERSION}.orig")
set(CPACK_PACKAGE_VERSION "${AFF3CT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${AFF3CT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${AFF3CT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${AFF3CT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "AFF3CT Component")

# set the component to be included in the package
set(CPACK_COMPONENTS_ALL simulator library headers documentation configuration references)

# rename the components to be displayed in the installer
set(CPACK_COMPONENT_SIMULATOR_DISPLAY_NAME "Simulator")
set(CPACK_COMPONENT_LIBRARY_DISPLAY_NAME "Library")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")
set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
set(CPACK_COMPONENT_CONFIGURATION_DISPLAY_NAME "Configuration Files")
set(CPACK_COMPONENT_REFERENCES_DISPLAY_NAME "BER/FER References")

# add a description of the components
set(CPACK_COMPONENT_SIMULATOR_DESCRIPTION "The AFF3CT standalone simulator application.")
set(CPACK_COMPONENT_LIBRARY_DESCRIPTION "The library containing all the AFF3CT toolbox features.")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "The C++ header files to use with the AFF3CT library.")
set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "The detailed documentation of the simulator and the library (html and PDF).")
set(CPACK_COMPONENT_CONFIGURATION_DESCRIPTION "Input files that can be used to configure the simulator or some library blocks.")
set(CPACK_COMPONENT_REFERENCES_DESCRIPTION "Many pre-simulated BER/FER reference curves.")

# add dependencies between the components
set(CPACK_COMPONENT_HEADERS_DEPENDS library)
set(CPACK_COMPONENT_SIMULATOR_DEPENDS configuration)

# group the components
set(CPACK_COMPONENT_SIMULATOR_GROUP "Runtime")
set(CPACK_COMPONENT_CONFIGURATION_GROUP "Runtime")
set(CPACK_COMPONENT_REFERENCES_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARY_GROUP "Development")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")

# add a description of the groups
set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION "The AFF3CT simulator and related components.")
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools you need to develop with the AFF3CT toolbox.")

# ignored files
set(CPACK_IGNORE_FILES "/build/;/\\\\.idea")
set(CPACK_SOURCE_IGNORE_FILES "${CPACK_IGNORE_FILES}")
set(CPACK_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};/")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

if (AFF3CT_UPLOAD_PPA)
    include(UploadPPA)
endif()

include(CPack)
