# CMake entry point
cmake_minimum_required (VERSION 2.6)

project (aff3ct)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Specify bin path
set (EXECUTABLE_OUTPUT_PATH bin/)

# Génération de la liste des fichiers sources
file (GLOB_RECURSE source_files src/*)

# Déclaration de l'exécutable
add_executable (aff3ct ${source_files})

# Command line options
option(ENABLE_SYSTEMC "Build the SystemC version" OFF)

find_package(GSL)
if (GSL_FOUND)
    include_directories(${GSL_INCLUDE_DIRS})
    target_link_libraries(aff3ct ${GSL_LIBRARIES})
endif (GSL_FOUND)

if (ENABLE_SYSTEMC)
    add_definitions("-DSYSTEMC")
    find_package(SystemC REQUIRED)
    if (SystemC_FOUND)
        include_directories(${SystemC_INCLUDE_DIRS})
        target_link_libraries(aff3ct ${SystemC_LIBRARIES})
    endif (SystemC_FOUND)

    find_package(TLM REQUIRED)
    if (TLM_FOUND)
        include_directories(${TLM_INCLUDE_DIRS})
    endif (TLM_FOUND)
endif (ENABLE_SYSTEMC)

# Enable C++11 and pthread
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-std=c++1y)
  target_link_libraries(aff3ct -lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(-std=c++11 -pthread)
  target_link_libraries(aff3ct -lpthread)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  add_definitions(-std=c++11 -pthread)
  target_link_libraries(aff3ct -lpthread)
endif()