#include <gtest/gtest.h>

#include <random>

#include <dynamic_negate_segment_tree.hpp>
#include "reference/seg_tree_reference_base.hpp"

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
        int rangeBeg = gen() % 500;  // [0..500)
        int rangeLen = gen() % 500;  // [0..500)

        tree.update(rangeBeg, rangeBeg + rangeLen);
        reference.update(rangeBeg, rangeBeg + rangeLen, updateOp);
    }


    for (std::size_t i = 0; i < 100; ++i) {
        int idx = gen() % 1000;

        const auto treeRes = tree.get(idx);
        const auto referenceRes = reference.get(idx);

        EXPECT_EQ(treeRes, referenceRes);
    }
}

TEST(DynamicNegateSegmentTree, FuzzTestMixedSetUpdateGet) {
    auto tree = NegateSumDynamicSegmentTree<int, int>(0, 1000, 42);
    auto reference = SegTreeReferenceBase<int, int>(0, 1000, 42);

    const auto updateOp = [](int num) { return -num; };
    std::mt19937 generator(54);

    for (std::size_t i = 0; i < 100; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)

        int operationChoise = generator() % 2;

        if (operationChoise) {
            int setVal = generator() % 1000; // [0..1000)
            tree.set(rngStart, rngStart + rngLen, setVal);
            reference.set(rngStart, rngStart + rngLen, setVal);
        } else {
            tree.update(rngStart, rngStart + rngLen);
            reference.update(rngStart, rngStart + rngLen, updateOp);
        }
    }

    for (std::size_t i = 0; i < 50; ++i) {
        int idx = generator() % 100; // [0..1000)
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
        int rangeBeg = gen() % 500;  // [0..500)
        int rangeLen = gen() % 500;  // [0..500)

        tree.update(rangeBeg, rangeBeg + rangeLen);
        reference.update(rangeBeg, rangeBeg + rangeLen, updateOp);
    }

    for (std::size_t i = 0; i < 100; ++i) {
        int rangeBeg = gen() % 500;  // [0..500)
        int rangeLen = gen() % 500;  // [0..500)

        int valueToSet = gen() % 1000;  // [0..1000)

        tree.set(rangeBeg, rangeBeg + rangeLen, valueToSet);
        reference.set(rangeBeg, rangeBeg + rangeLen, valueToSet);
    }

    for (std::size_t i = 0; i < 100; ++i) {
        int idx = gen() % 1000;

        const auto treeRes = tree.get(idx);
        const auto referenceRes = reference.get(idx);

        EXPECT_EQ(treeRes, referenceRes);
    }
}

