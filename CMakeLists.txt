# CMake entry point
cmake_minimum_required (VERSION 2.8.8)

project (aff3ct)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MACOSX_RPATH 1)

# Specify bin and lib paths
set (EXECUTABLE_OUTPUT_PATH bin/)
set (LIBRARY_OUTPUT_PATH lib/)

# Génération de la liste des fichiers sources
file (GLOB_RECURSE source_files src/*)

# Object
add_library (aff3ct-obj OBJECT ${source_files})
# Binary
add_executable (aff3ct-bin $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties(aff3ct-bin PROPERTIES OUTPUT_NAME aff3ct)
# Library
add_library (aff3ct-shared-lib SHARED $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties(aff3ct-shared-lib PROPERTIES OUTPUT_NAME aff3ct)
add_library (aff3ct-static-lib STATIC $<TARGET_OBJECTS:aff3ct-obj>)
set_target_properties(aff3ct-static-lib PROPERTIES OUTPUT_NAME aff3ct)

# Command line options
option(ENABLE_SYSTEMC "Enable SystemC support" OFF)
option(ENABLE_GSL     "Enable GSL support"     OFF)

macro(aff3ct_link_libraries lib)
    target_link_libraries(aff3ct-bin ${lib})
    target_link_libraries(aff3ct-shared-lib ${lib})
    target_link_libraries(aff3ct-static-lib ${lib})
endmacro()

# GSL
if (ENABLE_GSL)
    add_definitions("-DCHANNEL_GSL")

    find_package(GSL REQUIRED)
    if (GSL_FOUND)
        include_directories(${GSL_INCLUDE_DIRS})
        aff3ct_link_libraries("${GSL_LIBRARIES}")
    endif (GSL_FOUND)
endif (ENABLE_GSL)

# SystemC
if (ENABLE_SYSTEMC)
    add_definitions("-DSYSTEMC")

    find_package(SystemC REQUIRED)
    if (SystemC_FOUND)
        include_directories(${SystemC_INCLUDE_DIRS})
        aff3ct_link_libraries("${SystemC_LIBRARIES}")
    endif (SystemC_FOUND)

    find_package(TLM REQUIRED)
    if (TLM_FOUND)
        include_directories(${TLM_INCLUDE_DIRS})
    endif (TLM_FOUND)
endif (ENABLE_SYSTEMC)

# Enable C++11, pthread and fPIC
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-std=c++1y -fPIC)
    aff3ct_link_libraries(-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-std=c++11 -pthread -fPIC)
    aff3ct_link_libraries(-lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    add_definitions(-std=c++11 -pthread)
    aff3ct_link_libraries(-pthread)
endif()