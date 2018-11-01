# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------- CMAKE PROJECT
# ---------------------------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.0.2)
cmake_policy(SET CMP0054 NEW)
project (aff3ct)

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------- CMAKE OPTIONS
# ---------------------------------------------------------------------------------------------------------------------

option (AFF3CT_COMPILE_EXE        "Compile the executable"                                                    ON )
option (AFF3CT_COMPILE_STATIC_LIB "Compile the static library"                                                OFF)
option (AFF3CT_COMPILE_SHARED_LIB "Compile the shared library"                                                OFF)
option (AFF3CT_LINK_GSL           "Link with the GSL library (used in the channels)"                          OFF)
option (AFF3CT_LINK_MKL           "Link with the MKL library (used in the channels)"                          OFF)
option (AFF3CT_SYSTEMC_SIMU       "Enable the SystemC simulation (incompatible with the library compilation)" OFF)
option (AFF3CT_SYSTEMC_MODULE     "Enable the SystemC support (only for the modules)"                         OFF)
option (AFF3CT_MPI                "Enable the MPI support"                                                    OFF)
option (AFF3CT_POLAR_BIT_PACKING  "Enable the bit packing technique for Polar code SC decoding"               ON )
option (AFF3CT_COLORS             "Enable the colors in the terminal"                                         ON )

if (NOT (WIN32 OR APPLE))
    option (AFF3CT_BACKTRACE "Enable the backtrace display when and exception is raised" ON)
endif()

set(AFF3CT_PREC "MULTI" CACHE STRING "Select the precision in bits (can be '8', '16', '32', '64' or 'MULTI')")

if (AFF3CT_SYSTEMC_SIMU AND (AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB))
    message(FATAL_ERROR "It is impossible to compile the AFF3CT library if AFF3CT_SYSTEMC_SIMU='ON'.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------- CMAKE CONFIGURATION
# ---------------------------------------------------------------------------------------------------------------------

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set (CMAKE_MACOSX_RPATH 1)

# Enable C++11
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set (CMAKE_CXX_STANDARD 14)
else ()
    set (CMAKE_CXX_STANDARD 11)
endif()

# Specify bin and lib paths
set (EXECUTABLE_OUTPUT_PATH bin/)
set (LIBRARY_OUTPUT_PATH lib/)

# Generate the source files list
file (GLOB_RECURSE source_files src/*)

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
git_describe(GIT_VERSION "--tags")

string(REGEX REPLACE "^v"                    "" GIT_VERSION_RMV   ${GIT_VERSION})
string(REGEX REPLACE "-.*"                   "" GIT_VERSION_MMP   ${GIT_VERSION_RMV})
string(REGEX REPLACE "^.*-"                  "" GIT_HASH          ${GIT_VERSION_RMV})
string(      REPLACE "-"                     "" GIT_BUILD         ${GIT_VERSION_RMV})
string(      REPLACE "${GIT_VERSION_MMP}"    "" GIT_BUILD         ${GIT_BUILD})
string(      REPLACE "${GIT_HASH}"           "" GIT_BUILD         ${GIT_BUILD})
string(REGEX REPLACE "[.].*"                 "" GIT_VERSION_MAJOR ${GIT_VERSION_MMP})
string(      REPLACE "${GIT_VERSION_MAJOR}." "" GIT_VERSION_MINOR ${GIT_VERSION_MMP})
string(REGEX REPLACE "[.].*"                 "" GIT_VERSION_MINOR ${GIT_VERSION_MINOR})
string(      REPLACE "${GIT_VERSION_MAJOR}." "" GIT_VERSION_PATCH ${GIT_VERSION_MMP})
string(      REPLACE "${GIT_VERSION_MINOR}." "" GIT_VERSION_PATCH ${GIT_VERSION_PATCH})
string(REGEX REPLACE "[.].*"                 "" GIT_VERSION_PATCH ${GIT_VERSION_PATCH})

message(STATUS "AFF3CT - Git Version: ${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}")
message(STATUS "AFF3CT - Git Build: ${GIT_BUILD}")
message(STATUS "AFF3CT - Git Hash: ${GIT_HASH}")

if(("GIT_BUILD" STREQUAL "") OR ("GIT_HASH" STREQUAL ""))
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
# ------------------------------------------------------------------------------------------------------ CREATE FILTERS
# ---------------------------------------------------------------------------------------------------------------------

# Filters creation for IDEs (tested on Visual Studio and based on the "source_group" function)
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
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

macro (aff3ct_add_definitions def)
    add_definitions (${def})
    list(APPEND AFF3CT_DEFINITIONS ${def})
endmacro()

macro (aff3ct_add_compile_options opt)
    add_compile_options (${opt})
    list(APPEND AFF3CT_COMPILE_OPTIONS ${opt})
endmacro()

# by compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    aff3ct_add_compile_options(-Wno-overloaded-virtual)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    aff3ct_add_compile_options(-pthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    aff3ct_add_compile_options(-pthread)
endif()

# by system
if (WIN32) # for Windows operating system in general
    set (WINDOWS_VISTA 0x0600)
    aff3ct_add_definitions (-D_WIN32_WINNT=${WINDOWS_VISTA})
    aff3ct_add_definitions (-DNOMINMAX)
    message(STATUS "AFF3CT - System: Windows")
elseif (APPLE) # for macOS
    message(STATUS "AFF3CT - System: macOS")
elseif (UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix
    aff3ct_add_compile_options(-fPIC)
    message(STATUS "AFF3CT - System: Unix/Linux")
endif()

# common
if (AFF3CT_POLAR_BIT_PACKING)
    aff3ct_add_definitions (-DAFF3CT_POLAR_BIT_PACKING)
    message(STATUS "AFF3CT - Polar bit packing: on")
else()
    message(STATUS "AFF3CT - Polar bit packing: off")
endif()
if (AFF3CT_COLORS)
    aff3ct_add_definitions (-DAFF3CT_COLORS)
    message(STATUS "AFF3CT - Terminal colors: on")
else()
    message(STATUS "AFF3CT - Terminal colors: off")
endif()
if (NOT (WIN32 AND APPLE))
    if (AFF3CT_BACKTRACE)
        aff3ct_add_definitions (-DAFF3CT_BACKTRACE)
        message(STATUS "AFF3CT - Backtrace: on")
    else()
        message(STATUS "AFF3CT - Backtrace: off")
    endif()
endif()
if (AFF3CT_PREC STREQUAL "8")
    aff3ct_add_definitions (-DAFF3CT_8BIT_PREC)
    message(STATUS "AFF3CT - Precision: 8-bit")
elseif (AFF3CT_PREC STREQUAL "16")
    aff3ct_add_definitions (-DAFF3CT_16BIT_PREC)
    message(STATUS "AFF3CT - Precision: 16-bit")
elseif (AFF3CT_PREC STREQUAL "32")
    aff3ct_add_definitions (-DAFF3CT_32BIT_PREC)
    message(STATUS "AFF3CT - Precision: 32-bit")
elseif (AFF3CT_PREC STREQUAL "64")
    aff3ct_add_definitions (-DAFF3CT_64BIT_PREC)
    message(STATUS "AFF3CT - Precision: 64-bit")
elseif (AFF3CT_PREC STREQUAL "MULTI")
    aff3ct_add_definitions (-DAFF3CT_MULTI_PREC)
    message(STATUS "AFF3CT - Precision: 8/16/32/64-bit")
else()
    message(FATAL_ERROR "AFF3CT_PREC='${AFF3CT_PREC}' and should be '8', '16', '32', '64' or 'MULTI'.")
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------- HEADER ONLY LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

# AFF3CT headers
include_directories (src)

# MSVC dirent.h
if (MSVC)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC/include/")
endif()

# MIPP
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/mipp.h")
    aff3ct_add_definitions (-DMIPP_ENABLE_BACKTRACE)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/")
    message(STATUS "AFF3CT - Header found: MIPP")
else ()
    message(FATAL_ERROR "AFF3CT - MIPP can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/MIPP/")
endif ()

# rang
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/rang.hpp")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/")
    message(STATUS "AFF3CT - Header found: rang")
else ()
    message(FATAL_ERROR "AFF3CT - rang can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/rang/")
endif ()

# CLI11
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/CLI11/include/CLI/CLI.hpp")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/CLI11/include/CLI/")
    message(STATUS "AFF3CT - Header found: CLI11")
    #add_subdirectory(lib/CLI11)
    #aff3ct_link_libraries(CLI11::CLI11)
else ()
    message(FATAL_ERROR "AFF3CT - CLI11 can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/CLI11/")
endif ()

# date
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/date.h")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/")
    message(STATUS "AFF3CT - Header found: date")
else ()
    message(FATAL_ERROR "AFF3CT - date can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/date/")
endif ()

# ---------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------- OBJECTS/LIBS/EXE
# ---------------------------------------------------------------------------------------------------------------------

# Object
add_library (aff3ct-obj OBJECT ${source_files})

# Binary
if (AFF3CT_COMPILE_EXE)
    add_executable        (aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-bin PROPERTIES OUTPUT_NAME aff3ct)
    message(STATUS "AFF3CT - Compile: executable")
endif (AFF3CT_COMPILE_EXE)

# Library
if (AFF3CT_COMPILE_SHARED_LIB)
    add_library           (aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-shared-lib PROPERTIES OUTPUT_NAME aff3ct)
    message(STATUS "AFF3CT - Compile: shared library")
endif (AFF3CT_COMPILE_SHARED_LIB)
if (AFF3CT_COMPILE_STATIC_LIB)
    add_library           (aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-static-lib PROPERTIES OUTPUT_NAME aff3ct)
    message(STATUS "AFF3CT - Compile: static library")
endif (AFF3CT_COMPILE_STATIC_LIB)

macro (aff3ct_include_directories dir)
    include_directories (${dir})
    list(APPEND AFF3CT_INCLUDE_DIRS ${dir})
endmacro()

macro (aff3ct_link_libraries lib)
    if (AFF3CT_COMPILE_EXE)
        target_link_libraries (aff3ct-bin ${lib})
    endif (AFF3CT_COMPILE_EXE)
    if (AFF3CT_COMPILE_SHARED_LIB)
       target_link_libraries (aff3ct-shared-lib ${lib})
    endif (AFF3CT_COMPILE_SHARED_LIB)
    if (AFF3CT_COMPILE_STATIC_LIB)
        target_link_libraries (aff3ct-static-lib ${lib})
    endif (AFF3CT_COMPILE_STATIC_LIB)
    list(APPEND AFF3CT_LIBRARIES ${lib})
endmacro()

# ---------------------------------------------------------------------------------------------------------------------
# -------------------------------------------------------------------------------------------------- COMPILED LIBRARIES
# ---------------------------------------------------------------------------------------------------------------------

# GSL
if (AFF3CT_LINK_GSL)
    aff3ct_add_definitions("-DAFF3CT_CHANNEL_GSL")

    find_package (GSL REQUIRED)
    if (GSL_FOUND)
        message(STATUS "AFF3CT - Library found: GSL")
        aff3ct_include_directories (${GSL_INCLUDE_DIRS})
        aff3ct_link_libraries ("${GSL_LIBRARIES}")
    endif (GSL_FOUND)
endif (AFF3CT_LINK_GSL)

# MKL
if (AFF3CT_LINK_MKL)
    aff3ct_add_definitions("-DAFF3CT_CHANNEL_MKL")

    find_package (MKL REQUIRED)
    if (MKL_FOUND)
        message(STATUS "AFF3CT - Library found: MKL")
        aff3ct_include_directories (${MKL_INCLUDE_DIRS})
        aff3ct_link_libraries ("${MKL_MINIMAL_LIBRARIES}")
    endif (MKL_FOUND)
endif (AFF3CT_LINK_MKL)

# SystemC
if (AFF3CT_SYSTEMC_SIMU OR AFF3CT_SYSTEMC_MODULE)
    if (AFF3CT_SYSTEMC_SIMU)
        aff3ct_add_definitions ("-DAFF3CT_SYSTEMC_SIMU")
        aff3ct_add_definitions ("-DAFF3CT_SYSTEMC_MODULE")
    else ()
        aff3ct_add_definitions ("-DAFF3CT_SYSTEMC_MODULE")
	endif ()

    find_package (SystemC REQUIRED)
    if (SystemC_FOUND)
        message(STATUS "AFF3CT - Library found: SystemC")
        aff3ct_include_directories (${SystemC_INCLUDE_DIRS})
        aff3ct_link_libraries ("${SystemC_LIBRARIES}")
    endif (SystemC_FOUND)

    find_package (TLM REQUIRED)
    if (TLM_FOUND)
        message(STATUS "AFF3CT - Library found: TLM")
        aff3ct_include_directories (${TLM_INCLUDE_DIRS})
    endif (TLM_FOUND)
endif ()

# MPI
if (AFF3CT_MPI)
    aff3ct_add_definitions ("-DAFF3CT_ENABLE_MPI")

    find_package (MPI REQUIRED)
    if (MPI_CXX_FOUND)

        message(STATUS "AFF3CT - Library found: MPI")
        aff3ct_include_directories (${MPI_CXX_INCLUDE_PATH})
        aff3ct_link_libraries ("${MPI_CXX_LIBRARIES}")

        if (MPI_CXX_COMPILE_FLAGS)
            if (AFF3CT_COMPILE_EXE)
                set_target_properties(aff3ct-bin        PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (AFF3CT_COMPILE_EXE)
            if (AFF3CT_COMPILE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (AFF3CT_COMPILE_SHARED_LIB)
            if (AFF3CT_COMPILE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (AFF3CT_COMPILE_STATIC_LIB)
        endif (MPI_CXX_COMPILE_FLAGS)

        if (MPI_CXX_LINK_FLAGS)
            if (AFF3CT_COMPILE_EXE)
                set_target_properties(aff3ct-bin        PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (AFF3CT_COMPILE_EXE)
            if (AFF3CT_COMPILE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (AFF3CT_COMPILE_SHARED_LIB)
            if (AFF3CT_COMPILE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (AFF3CT_COMPILE_STATIC_LIB)
        endif (MPI_CXX_LINK_FLAGS)

    endif (MPI_CXX_FOUND)
endif (AFF3CT_MPI)

# pthread
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"      OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"        OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    aff3ct_link_libraries (-lpthread)
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------- GENERATE CMAKE CONFIG FILES
# ---------------------------------------------------------------------------------------------------------------------

# Auto generate cmake config files to link with AFF3CT library (only if an AFF3CT library has been compiled)
if (AFF3CT_COMPILE_STATIC_LIB OR AFF3CT_COMPILE_SHARED_LIB)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/aff3ct-config.cmake.in"
                   "lib/cmake/aff3ct-config.cmake" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/aff3ct-config-version.cmake.in"
                   "lib/cmake/aff3ct-config-version.cmake" @ONLY)
endif()

# ---------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------- INSTALL
# ---------------------------------------------------------------------------------------------------------------------

if (AFF3CT_COMPILE_EXE)
    install(TARGETS aff3ct-bin RUNTIME DESTINATION bin/aff3ct-${GIT_VERSION_RMV})
endif()
if (AFF3CT_COMPILE_SHARED_LIB)
    install(TARGETS aff3ct-shared-lib LIBRARY DESTINATION lib/aff3ct-${GIT_VERSION_RMV})
endif()
if (AFF3CT_COMPILE_STATIC_LIB)
    install(TARGETS aff3ct-static-lib ARCHIVE DESTINATION lib/aff3ct-${GIT_VERSION_RMV})
endif()

if (AFF3CT_COMPILE_SHARED_LIB OR AFF3CT_COMPILE_STATIC_LIB)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/aff3ct-config.cmake"
                  "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/aff3ct-config-version.cmake"
            DESTINATION lib/cmake/aff3ct-${GIT_VERSION_RMV})
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/aff3ct
        FILES_MATCHING PATTERN "*.hxx")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/CLI11/include/CLI/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/CLI11
        FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/date/include/date/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/date
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/MIPP
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/MIPP
        FILES_MATCHING PATTERN "*.hxx")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/MSVC/include/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/dirent
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/" DESTINATION include/aff3ct-${GIT_VERSION_RMV}/rang
        FILES_MATCHING PATTERN "*.hpp")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/conf/cde/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/conf/" DESTINATION share/aff3ct-${GIT_VERSION_RMV}/conf
            FILES_MATCHING PATTERN "*")
endif()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/refs/BCH/")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/refs/" DESTINATION share/aff3ct-${GIT_VERSION_RMV}/refs
            FILES_MATCHING PATTERN "*.txt")
endif()