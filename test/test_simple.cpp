#include <gtest/gtest.h>
#include <random>
#include "reference/seg_tree_reference_base.hpp"

#include <curried/dynamic_simple_get_set_segment_tree.hpp>

using dst::DynamicSimpleGetSetSegmentTree;

TEST(SimpleGetSetDynamicSegmentTree, Construct) {
    auto tree = DynamicSimpleGetSetSegmentTree<int, long long>(-5, 100, 77);
}

TEST(SimpleGetSetDynamicSegmentTree, RangeSet) {
    auto tree = DynamicSimpleGetSetSegmentTree<int, long long>(0, 42, 77);

    tree.set(13, 17, 56);

    EXPECT_EQ(tree.get(13), 56);
    EXPECT_EQ(tree.get(15), 56);
    EXPECT_EQ(tree.get(16), 56);
    EXPECT_EQ(tree.get(17), 77);
    EXPECT_EQ(tree.get(8), 77);
    EXPECT_EQ(tree.get(37), 77);
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGet) {
    auto tree = DynamicSimpleGetSetSegmentTree<int, int>(0, 1000, 0);
    auto reference = SegTreeReferenceBase<int, int>(0, 1000, 0);

    std::mt19937 generator(42);

    for (std::size_t i = 0; i < 100; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)
        int setVal = generator() % 1000; // [0..100)
        tree.set(rngStart, rngStart + rngLen, setVal);
        reference.set(rngStart, rngStart + rngLen, setVal);
    }

    for (std::size_t i = 0; i < 50; ++i) {
        int idx = generator() % 1000; // [0..1000)
        auto treeRes = tree.get(idx);
        auto refRes = reference.get(idx);
        EXPECT_EQ(treeRes, refRes);
    }
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGetMixed) {
    auto tree = DynamicSimpleGetSetSegmentTree<int, int>(0, 1000, 0);
    auto reference = SegTreeReferenceBase<int, int>(0, 1000, 0);

    std::mt19937 generator(42);

    for (std::size_t i = 0; i < 100; ++i) {
        std::size_t rngStart = generator() % 500; // [0..500)
        std::size_t rngLen = generator() % 500;   // [0..500)

        if (int opType = generator() % 2; opType == 0) {
            int setVal = generator() % 1000; // [0..1000)
            tree.set(rngStart, rngStart + rngLen, setVal);
            reference.set(rngStart, rngStart + rngLen, setVal);
        } else {
            int idx = generator() % 1000; // [0..1000)
            auto treeRes = tree.get(idx);
            auto refRes = reference.get(idx);
            EXPECT_EQ(treeRes, refRes);
        }
    }
}
