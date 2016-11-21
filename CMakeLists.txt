# CMake entry point
cmake_minimum_required (VERSION 3.0.2)

project (aff3ct)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set (CMAKE_MACOSX_RPATH 1)

# Specify bin and lib paths
set (EXECUTABLE_OUTPUT_PATH bin/)
set (LIBRARY_OUTPUT_PATH lib/)

# Generate the source files list
file (GLOB_RECURSE source_files src/*)

# Command line options
option (ENABLE_GSL     "Enable GSL support"     OFF)
option (ENABLE_MKL     "Enable MKL support"     OFF)
option (ENABLE_SYSTEMC "Enable SystemC support" OFF)
option (ENABLE_STARPU  "Enable StarPU support"  OFF)
option (ENABLE_MPI     "Enable MPI support"     OFF)

# StarPU: set the link dir
if (ENABLE_STARPU)
    find_package (PkgConfig REQUIRED)
    if (PKG_CONFIG_FOUND)
        pkg_search_module(SPU REQUIRED starpu-1.2)
        link_directories(${SPU_LIBRARY_DIRS})
    endif (PKG_CONFIG_FOUND)
endif (ENABLE_STARPU)

# Add includes
include_directories (src)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
include_directories (src/Tools/MSVC)
endif()

# Object
add_library (aff3ct-obj OBJECT ${source_files})

# Binary
add_executable        (aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties (aff3ct-bin PROPERTIES OUTPUT_NAME aff3ct)

# Library
add_library           (aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties (aff3ct-shared-lib PROPERTIES OUTPUT_NAME aff3ct)
add_library           (aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties (aff3ct-static-lib PROPERTIES OUTPUT_NAME aff3ct)

macro (aff3ct_link_libraries lib)
    target_link_libraries (aff3ct-bin ${lib})
    target_link_libraries (aff3ct-shared-lib ${lib})
    target_link_libraries (aff3ct-static-lib ${lib})
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
    add_definitions ("-DSYSTEMC")

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

# StarPU
if (ENABLE_STARPU)
    add_definitions ("-DSTARPU")

    find_package (PkgConfig REQUIRED)
    if (PKG_CONFIG_FOUND)
        pkg_search_module(SPU REQUIRED starpu-1.2)

        include_directories (${SPU_INCLUDE_DIRS})
        aff3ct_link_libraries ("${SPU_LIBRARIES}")

    endif (PKG_CONFIG_FOUND)
endif (ENABLE_STARPU)

# MPI
if (ENABLE_MPI)
    add_definitions ("-DENABLE_MPI")

    find_package (MPI REQUIRED)
    if (MPI_CXX_FOUND)

        include_directories (${MPI_CXX_INCLUDE_PATH})
        aff3ct_link_libraries ("${MPI_CXX_LIBRARIES}")

        if (MPI_CXX_COMPILE_FLAGS)
            set_target_properties(aff3ct-bin PROPERTIES
                COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
        endif (MPI_CXX_COMPILE_FLAGS)

        if (MPI_CXX_LINK_FLAGS)
            set_target_properties(aff3ct-bin PROPERTIES
                LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
        endif (MPI_CXX_LINK_FLAGS)

    endif (MPI_CXX_FOUND)
endif (ENABLE_MPI)

# Enable C++11 and pthread
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    add_definitions (-std=c++1y -Wno-overloaded-virtual)
    aff3ct_link_libraries (-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions (-std=c++11 -pthread)
    aff3ct_link_libraries (-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    add_definitions (-std=c++11 -pthread)
    aff3ct_link_libraries (-pthread)
endif()

if (UNIX)
	add_definitions (-fPIC)
endif()

# Specific options
add_definitions (-DENABLE_BIT_PACKING -DMIPP_ALIGNED_LOADS)