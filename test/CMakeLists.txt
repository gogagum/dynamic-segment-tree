set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(dst-test
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
target_link_libraries(dst-test PRIVATE gtest_main dynamic-segment-tree)

include(GoogleTest)

