////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <random>
#include "reference/seg_tree_reference_base.hpp"

#include <dst/curried/dynamic_negate_segment_tree.hpp>

template <std::integral KeyT, class ValueT, class Allocator = std::allocator<ValueT>>
using NegateSumDynamicSegmentTree =
    dst::DynamicNegateSegmentTree<KeyT,
                                  ValueT,
                                  ValueT,
                                  dst::NoRangeGetOp,
                                  dst::NoRangeGetOp,
                                  Allocator>;

TEST(DynamicNegateSegmentTree, Construct) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
}

TEST(DynamicNegateSegmentTree, Update) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
    tree.update(2, 5);
    EXPECT_EQ(tree.get(4), -13);
}

TEST(DynamicNegateSegmentTree, DoubleUpdate) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
    tree.update(2, 5);
    tree.update(2, 5);
    EXPECT_EQ(tree.get(4), 13);
}

TEST(DynamicNegateSegmentTree, TwoIntersectingUpdate) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
    tree.update(2, 10);
    tree.update(7, 21);
    EXPECT_EQ(tree.get(8), 13);
    EXPECT_EQ(tree.get(3), -13);
    EXPECT_EQ(tree.get(13), -13);
}

TEST(DynamicNegateSegmentTree, SimpleRangweGet) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
    tree.update(2, 10);
    tree.update(7, 21);
    EXPECT_EQ(tree.get(8), 13);
    EXPECT_EQ(tree.get(3), -13);
    EXPECT_EQ(tree.get(13), -13);
}


TEST(DynamicNegateSegmentTree, UpdateFuzzTest) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 1000, 42);
    auto reference = SegTreeReferenceBase<int, int>(0, 1000, 42);

    const auto updateOp = [](int num) { return -num; };
    auto gen = std::mt19937(37);

    for (std::size_t i = 0; i < 100; ++i) {
        const int rangeBeg = static_cast<int>(gen() % 500);  // [0..500)
        const int rangeLen = static_cast<int>(gen() % 500);  // [0..500)

        tree.update(rangeBeg, rangeBeg + rangeLen);
        reference.update(rangeBeg, rangeBeg + rangeLen, updateOp);
    }


    for (std::size_t i = 0; i < 100; ++i) {
        const int idx = static_cast<int>(gen() % 1000);

        const auto treeRes = tree.get(idx);
        const auto referenceRes = reference.get(idx);

        EXPECT_EQ(treeRes, referenceRes);
    }
}

TEST(DynamicNegateSegmentTree, FuzzTestMixedSetUpdateGet) {
    auto tree = NegateSumDynamicSegmentTree<std::size_t, int>(0, 1000, 42);
    auto reference = SegTreeReferenceBase<std::size_t, int>(0, 1000, 42);

    const auto updateOp = [](int num) { return -num; };
    std::mt19937 generator(54);

    for (std::size_t i = 0; i < 100; ++i) {
        const std::size_t rngStart = generator() % 500; // [0..500)
        const std::size_t rngLen = generator() % 500;   // [0..500)

        const int operationChoise = static_cast<int>(generator()) % 2;

        if (operationChoise) {
            const int setVal =
                static_cast<int>(generator()) % 1000; // [0..1000)
            tree.set(rngStart, rngStart + rngLen, setVal);
            reference.set(rngStart, rngStart + rngLen, setVal);
        } else {
            tree.update(rngStart, rngStart + rngLen);
            reference.update(rngStart, rngStart + rngLen, updateOp);
        }
    }

    for (std::size_t i = 0; i < 50; ++i) {
        const std::size_t idx = generator() % 100; // [0..1000)
        auto treeRes = tree.get(idx);
        auto refRes = reference.get(idx);
        EXPECT_EQ(treeRes, refRes);
    }
}

TEST(DynamicNegateSegmentTree, UpdatePlusSetFuzzTest) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 1000, 42);
    auto reference = SegTreeReferenceBase<int, int>(0, 1000, 42);

    const auto updateOp = [](int num) { return -num; };
    auto gen = std::mt19937(37);

    for (std::size_t i = 0; i < 100; ++i) {
        const int rangeBeg = static_cast<int>(gen() % 500);  // [0..500)
        const int rangeLen = static_cast<int>(gen() % 500);  // [0..500)

        tree.update(rangeBeg, rangeBeg + rangeLen);
        reference.update(rangeBeg, rangeBeg + rangeLen, updateOp);
    }

    for (std::size_t i = 0; i < 100; ++i) {
        const int rangeBeg = static_cast<int>(gen() % 500);  // [0..500)
        const int rangeLen = static_cast<int>(gen() % 500);  // [0..500)

        const int valueToSet = static_cast<int>(gen() % 1000);  // [0..1000)

        tree.set(rangeBeg, rangeBeg + rangeLen, valueToSet);
        reference.set(rangeBeg, rangeBeg + rangeLen, valueToSet);
    }

    for (std::size_t i = 0; i < 100; ++i) {
        const int idx = static_cast<int>(gen() % 1000);

        const auto treeRes = tree.get(idx);
        const auto referenceRes = reference.get(idx);

        EXPECT_EQ(treeRes, referenceRes);
    }
}

