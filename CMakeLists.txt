# CMake entry point
cmake_minimum_required (VERSION 3.0.2)
cmake_policy(SET CMP0054 NEW)

project (aff3ct)

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

# Auto generate a header containing the last GIT SHA1
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_describe(GIT_VERSION "--tags")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp" @ONLY)
list(APPEND source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/version.cpp")

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

# Command line options
option (ENABLE_EXEC           "Enable to compile AFF3CT executable"           ON )
option (ENABLE_STATIC_LIB     "Enable to compile AFF3CT static library"       OFF)
option (ENABLE_SHARED_LIB     "Enable to compile AFF3CT shared library"       OFF)
option (ENABLE_GSL            "Enable GSL support"                            OFF)
option (ENABLE_MKL            "Enable MKL support"                            OFF)
option (ENABLE_SYSTEMC        "Enable SystemC support"                        OFF)
option (ENABLE_SYSTEMC_MODULE "Enable SystemC support (only for the modules)" OFF)
option (ENABLE_MPI            "Enable MPI support"                            OFF)

# Add includes
include_directories (src)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    include_directories (src/Tools/MSVC)
endif()

# MIPP wrapper includes
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/mipp.h")
    add_definitions (-DMIPP_ENABLE_BACKTRACE)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/src/")
    message(STATUS "MIPP found")
else ()
    message(FATAL_ERROR "MIPP can't be found, try to init the submodules with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/MIPP/")
endif ()

# rang header
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/rang.hpp")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/rang/include/")
    message(STATUS "rang found")
else ()
    message(FATAL_ERROR "rang can't be found, try to init the submodules with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/rang/")
endif ()

# CLI11 header
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/CLI11/include/CLI/CLI.hpp")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lib/CLI11/include/CLI/")
    message(STATUS "CLI11 found")
    #add_subdirectory(lib/CLI11)
    #aff3ct_link_libraries(CLI11::CLI11)
else ()
    message(FATAL_ERROR "CLI11 can't be found, try to init the submodules with the following cmd:\n"
                        "$ git submodule update --init -- ../lib/CLI11/")
endif ()


# Object
add_library (aff3ct-obj OBJECT ${source_files})

# Binary
if (ENABLE_EXEC)
    add_executable        (aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-bin PROPERTIES OUTPUT_NAME aff3ct)
endif (ENABLE_EXEC)

# Library
if (ENABLE_SHARED_LIB)
    add_library           (aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-shared-lib PROPERTIES OUTPUT_NAME aff3ct)
endif (ENABLE_SHARED_LIB)
if (ENABLE_STATIC_LIB)
    add_library           (aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
    set_target_properties (aff3ct-static-lib PROPERTIES OUTPUT_NAME aff3ct)
endif (ENABLE_STATIC_LIB)

macro (aff3ct_link_libraries lib)
    if (ENABLE_EXEC)
        target_link_libraries (aff3ct-bin ${lib})
    endif (ENABLE_EXEC)
    if (ENABLE_SHARED_LIB)
       target_link_libraries (aff3ct-shared-lib ${lib})
    endif (ENABLE_SHARED_LIB)
    if (ENABLE_STATIC_LIB)
        target_link_libraries (aff3ct-static-lib ${lib})
    endif (ENABLE_STATIC_LIB)
endmacro()

# GSL
if (ENABLE_GSL)
    add_definitions("-DCHANNEL_GSL")

    find_package (GSL REQUIRED)
    if (GSL_FOUND)
        include_directories (${GSL_INCLUDE_DIRS})
        aff3ct_link_libraries ("${GSL_LIBRARIES}")
    endif (GSL_FOUND)
endif (ENABLE_GSL)

# MKL
if (ENABLE_MKL)
    add_definitions("-DCHANNEL_MKL")

    find_package (MKL REQUIRED)
    if (MKL_FOUND)
        include_directories (${MKL_INCLUDE_DIRS})
        aff3ct_link_libraries ("${MKL_MINIMAL_LIBRARIES}")
    endif (MKL_FOUND)
endif (ENABLE_MKL)

# SystemC
if (ENABLE_SYSTEMC)
    if (ENABLE_SYSTEMC_MODULE)
        add_definitions ("-DSYSTEMC_MODULE")
    else ()
        add_definitions ("-DSYSTEMC")
        add_definitions ("-DSYSTEMC_MODULE")
	endif (ENABLE_SYSTEMC_MODULE)

    find_package (SystemC REQUIRED)
    if (SystemC_FOUND)
        include_directories (${SystemC_INCLUDE_DIRS})
        aff3ct_link_libraries ("${SystemC_LIBRARIES}")
    endif (SystemC_FOUND)

    find_package (TLM REQUIRED)
    if (TLM_FOUND)
        include_directories (${TLM_INCLUDE_DIRS})
    endif (TLM_FOUND)
endif (ENABLE_SYSTEMC)

# MPI
if (ENABLE_MPI)
    add_definitions ("-DENABLE_MPI")

    find_package (MPI REQUIRED)
    if (MPI_CXX_FOUND)

        include_directories (${MPI_CXX_INCLUDE_PATH})
        aff3ct_link_libraries ("${MPI_CXX_LIBRARIES}")

        if (MPI_CXX_COMPILE_FLAGS)
            if (ENABLE_EXEC)
                set_target_properties(aff3ct-bin        PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (ENABLE_EXEC)
            if (ENABLE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (ENABLE_SHARED_LIB)
            if (ENABLE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
            endif (ENABLE_STATIC_LIB)
        endif (MPI_CXX_COMPILE_FLAGS)

        if (MPI_CXX_LINK_FLAGS)
            if (ENABLE_EXEC)
                set_target_properties(aff3ct-bin        PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (ENABLE_EXEC)
            if (ENABLE_SHARED_LIB)
                set_target_properties(aff3ct-shared-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (ENABLE_SHARED_LIB)
            if (ENABLE_STATIC_LIB)
                set_target_properties(aff3ct-static-lib PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
            endif (ENABLE_STATIC_LIB)
        endif (MPI_CXX_LINK_FLAGS)

    endif (MPI_CXX_FOUND)
endif (ENABLE_MPI)

# Enable pthread
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    add_definitions (-Wno-overloaded-virtual)
    aff3ct_link_libraries (-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions (-pthread)
    aff3ct_link_libraries (-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    add_definitions (-pthread)
    aff3ct_link_libraries (-pthread)
endif()

# mandatory for rang on Windows
if (WIN32)
    add_definitions (-D_WIN32_WINNT=_WIN32_WINNT_VISTA)
    add_definitions (-DNOMINMAX)
    message(STATUS "System: Windows")
elseif (UNIX)
    add_definitions (-fPIC)
    message(STATUS "System: Unix")
elseif (APPLE)
    message(STATUS "System: MacOS")
endif()

# Specific options
add_definitions (-DENABLE_BIT_PACKING)
