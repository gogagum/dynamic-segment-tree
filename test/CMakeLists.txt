project(dynamic-segment-tree-tests LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(segment_tree_test
    negate_test.cpp
    sum_test.cpp
    min_test.cpp
    max_test.cpp)

enable_testing()

target_link_libraries(segment_tree_test PRIVATE gtest_main dynamic-segment-tree)

include(GoogleTest)
gtest_discover_tests(segment_tree_test)

