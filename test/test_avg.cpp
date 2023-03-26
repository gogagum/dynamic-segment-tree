////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <random>

#include "reference/avg_seg_tree_reference.hpp"

#include <dst/curried/dynamic_avg_segment_tree.hpp>

using dst::DynamicAvgSegmentTree;

TEST(DynamicAvgSegmentTree, Construct) {
    [[maybe_unused]] auto tree = DynamicAvgSegmentTree<int, float>(2, 57);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGet) {
    auto tree = DynamicAvgSegmentTree<int, float>(2, 57, 42.f);

    auto avg = tree.rangeGet(3, 16);
    EXPECT_FLOAT_EQ(avg, 42.f);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSet) {
    auto tree = DynamicAvgSegmentTree<int, float>(0, 4, 42.f);

    tree.set(2, 4, 37.f);

    auto avg = tree.rangeGet(0, 4);
    EXPECT_FLOAT_EQ(avg, (37.f + 42.f) / 2.f);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSetOneToTwo) {
    auto tree = DynamicAvgSegmentTree<int, float>(0, 6, 42.f);

    tree.set(4, 6, 37.f);

    auto avg = tree.rangeGet(0, 6);
    EXPECT_FLOAT_EQ(avg, (37.f + 42.f * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSetTwoToOne) {
    auto tree = DynamicAvgSegmentTree<int, float>(0, 6, 42.f);

    tree.set(0, 2, 37.f);

    auto avg = tree.rangeGet(0, 6);
    EXPECT_FLOAT_EQ(avg, (37.f + 42.f * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, SetRangeGetAvg) {
    auto tree = DynamicAvgSegmentTree<std::size_t, float>(0, 1000, 0.f);
    auto reference = AvgSegTreeReference<std::size_t, float>(0, 1000, 0.f);

    std::mt19937 generator(42);

    for (std::size_t i = 0; i < 100; ++i) {
        const std::size_t rngStart = generator() % 500; // [0..500)
        const std::size_t rngLen = generator() % 500;   // [0..500)
        const float setVal = static_cast<float>(generator() % 1000); // [0..1000)
        tree.set(rngStart, rngStart + rngLen, setVal);
        reference.set(rngStart, rngStart + rngLen, setVal);
    }

    for (std::size_t i = 0; i < 50; ++i) {
        const std::size_t rngStart = generator() % 500; // [0..500)
        const std::size_t rngLen = generator() % 500;   // [0..500)
        auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
        auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
        EXPECT_FLOAT_EQ(treeRes, refRes);
    }
}

TEST(DynamicAvgSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
    auto tree =
        DynamicAvgSegmentTree<std::size_t, float, float, std::plus<float>>(
            0, 1000, 0);
    auto reference = AvgSegTreeReference<std::size_t, float>(0, 1000, 0);

    std::mt19937 generator(54);

    for (std::size_t i = 0; i < 100; ++i) {
        const std::size_t rngStart = generator() % 500; // [0..500)
        const std::size_t rngLen = generator() % 500;   // [0..500)

        const int operationChoise = static_cast<int>(generator()) % 2;

        if (operationChoise) {
            const float setVal =
                static_cast<float>(generator() % 1000); // [0..1000)
            tree.set(rngStart, rngStart + rngLen, setVal);
            reference.set(rngStart, rngStart + rngLen, setVal);
        } else {
            const float updateValue =
                static_cast<float>(generator() % 1000);  // [0..1000)
            tree.update(rngStart, rngStart + rngLen, updateValue);
            reference.update(rngStart, rngStart + rngLen,
                             std::plus<int>(), updateValue);
        }
    }

    for (std::size_t i = 0; i < 50; ++i) {
        const std::size_t rngStart = generator() % 500; // [0..500)
        const std::size_t rngLen = generator() % 500;   // [0..500)
        auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
        auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
        EXPECT_FLOAT_EQ(treeRes, refRes);
    }
}
