cmake_minimum_required(VERSION 3.15)

project(dynamic-segment-tree LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# without it public headers won't get installed
set(public_headers
    include/dynamic_segment_tree.hpp
    include/impl/dynamic_segment_tree_variation_base.hpp
    include/curried/dynamic_avg_segment_tree.hpp
    include/curried/dynamic_max_segment_tree.hpp
    include/curried/dynamic_min_segment_tree.hpp
    include/curried/dynamic_negate_segment_tree.hpp
    include/curried/dynamic_simple_get_set_segment_tree.hpp
    include/curried/dynamic_sum_segment_tree.hpp
)

# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (BUILD_TEST)
    add_subdirectory(test)
endif (BUILD_TEST)
