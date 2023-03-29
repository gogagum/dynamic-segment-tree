################################################################################
# Copyright Georgy Guminov 2023.
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

add_subdirectory(boost)

add_executable(dst-test
    boost_allocator.cpp
    boost_adaptive_pool.cpp
    test_negate.cpp
    test_sum.cpp
    test_min.cpp
    test_max.cpp
    test_simple.cpp
    test_avg.cpp
)

enable_testing()

if (CMAKE_CROSSCOMPILING)
    message("Cross compiling. Use static libraries for tests.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
elseif()
    message("Not cross compiling. Use dynamic libraries.")
endif()

include(GoogleTest)

target_link_libraries(dst-test PUBLIC gtest_main dynamic-segment-tree Boost::container)
