################################################################################
# Copyright Georgy Guminov 2023-2024.
# Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.15)

include(FetchContent)

function(add_boost_package PackageName package_repo_addr tag)
    message(STATUS "FetchContent: ${PackageName}, ${package_repo_addr}, ${tag}")

    FetchContent_Declare(
        ${PackageName}
        GIT_REPOSITORY https://github.com/boostorg/${package_repo_addr}.git
        GIT_TAG ${tag}
    )

    FetchContent_MakeAvailable(${PackageName})
endfunction()

add_boost_package(Boost.Container container boost-1.84.0)
add_boost_package(Boost.Assert assert boost-1.84.0)
add_boost_package(Boost.Config config boost-1.84.0)
add_boost_package(Boost.Core core boost-1.84.0)
add_boost_package(Boost.Intrusive intrusive boost-1.84.0)
add_boost_package(Boost.StaticAssert static_assert boost-1.84.0)
add_boost_package(Boost.Move move boost-1.84.0)
add_boost_package(Boost.ContainerHash container_hash boost-1.84.0)
add_boost_package(Boost.ThrowException throw_exception boost-1.84.0)
add_boost_package(Boost.Describe describe boost-1.84.0)
add_boost_package(Boost.TypeTraits type_traits boost-1.84.0)
add_boost_package(Boost.Mp11 mp11 boost-1.84.0)
