project(dynamic-segment-tree-tests LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(segment_tree_test
    test_negate.cpp
    test_sum.cpp
    test_min.cpp
    test_max.cpp
    test_simple.cpp
    test_avg.cpp
)

enable_testing()

target_link_libraries(segment_tree_test PRIVATE gtest_main dynamic-segment-tree)

include(GoogleTest)
gtest_discover_tests(segment_tree_test)

