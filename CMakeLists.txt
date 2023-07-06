################################################################################
# Copyright Georgy Guminov 2023.
# Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.15)

project(dynamic-segment-tree LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: remove this flag when Clang 17 is out.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "set -fexperimental-library flag")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library")
endif()

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE 
    src/dynamic_segment_tree_update_variation_base.cpp
    src/dynamic_avg_segment_tree.cpp
    src/dynamic_max_segment_tree.cpp
    src/dynamic_min_segment_tree.cpp
    src/dynamic_negate_segment_tree.cpp
    src/dynamic_simple_get_set_segment_tree.cpp
    src/dynamic_sum_segment_tree.cpp
    src/mp.cpp
    src/node.cpp
    src/dynamic_segment_tree_range_get_variation_base.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# without it public headers won't get installed
set(public_headers
    include/dst/mp.hpp
    include/dst/dynamic_segment_tree.hpp
    include/dst/curried/dynamic_avg_segment_tree.hpp
    include/dst/curried/dynamic_max_segment_tree.hpp
    include/dst/curried/dynamic_min_segment_tree.hpp
    include/dst/curried/dynamic_negate_segment_tree.hpp
    include/dst/curried/dynamic_simple_get_set_segment_tree.hpp
    include/dst/curried/dynamic_sum_segment_tree.hpp
)

# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (DST_TESTS)
    add_subdirectory(test)
endif (DST_TESTS)
