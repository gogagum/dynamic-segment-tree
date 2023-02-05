#include <gtest/gtest.h>
#include <random>

#include "reference/avg_seg_tree_reference.hpp"

#include <curried/dynamic_avg_segment_tree.hpp>

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
    auto tree = DynamicAvgSegmentTree<int, float>(0, 1000, 0.f);
    auto reference = AvgSegTreeReference<int, float>(0, 1000, 0.f);

    std::mt19937 generator(42);

    for (std::size_t i = 0; i < 100; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)
        int setVal = generator() % 1000; // [0..1000)
        tree.set(rngStart, rngStart + rngLen, setVal);
        reference.set(rngStart, rngStart + rngLen, setVal);
    }

    for (std::size_t i = 0; i < 50; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)
        auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
        auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
        EXPECT_FLOAT_EQ(treeRes, refRes);
    }
}

TEST(DynamicAvgSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
    auto tree = DynamicAvgSegmentTree<int, float, float, std::plus<float>>(0, 1000, 0);
    auto reference = AvgSegTreeReference<int, float>(0, 1000, 0);

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
            int updateValue = generator() % 1000;  // [0..1000)
            tree.update(rngStart, rngStart + rngLen, updateValue);
            reference.update(rngStart, rngStart + rngLen, std::plus<int>(), updateValue);
        }
    }

    for (std::size_t i = 0; i < 50; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)
        auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
        auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
        EXPECT_FLOAT_EQ(treeRes, refRes);
    }
}
