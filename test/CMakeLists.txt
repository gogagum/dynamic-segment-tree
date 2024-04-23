################################################################################
# Copyright Georgy Guminov 2023-2024.
# Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    range-v3
    URL https://github.com/ericniebler/range-v3/archive/refs/tags/0.12.0.zip
)

FetchContent_MakeAvailable(range-v3)

add_subdirectory(boost)

add_executable(dst-test
    allocator_common.cpp
    node.cpp
    test_negate.cpp
    test_sum.cpp
    test_min.cpp
    test_max.cpp
    test_avg.cpp
    test_copy_only_copy_counter.cpp
    test_copy_and_move_counter.cpp
    counters/copy_only_copy_counter.cpp
    counters/copy_and_move_counter.cpp
    test_copy_count.cpp
    test_copy_and_move_counts.cpp
    tools/info_stream.cpp
    tools/copy_n_times_then_throw.cpp
)

enable_testing()

if (CMAKE_CROSSCOMPILING)
    message("Cross compiling. Use static libraries for tests.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
elseif()
    message("Not cross compiling. Use dynamic libraries.")
endif()

include(GoogleTest)

target_link_libraries(dst-test PUBLIC gtest_main dynamic-segment-tree Boost::container Boost::core range-v3)
