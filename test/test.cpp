#include <gtest/gtest.h>
#include <lazy_segment_tree.hpp>

TEST(LazySegmentTree, Construct) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
}

TEST(LazySegmentTree, QueryMiddle) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    EXPECT_EQ(tree.get(21), 54);
}

TEST(LazySegmentTree, QueryEnd) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    EXPECT_EQ(tree.get(42), 0);
}

TEST(LazySegmentTree, QueryAfterEnd) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    EXPECT_EQ(tree.get(73), 0);
}

TEST(LazySegmentTree, QueryBegin) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    EXPECT_EQ(tree.get(0), 54);
}

TEST(LazySegmentTree, QueryBeforeBegin) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    EXPECT_EQ(tree.get(-5), 0);
}

TEST(LazySegmentTree, OneUpdate) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    tree.update(15, 37, 18);
    EXPECT_EQ(tree.get(5), 54);
    EXPECT_EQ(tree.get(14), 54);
    EXPECT_EQ(tree.get(15), 54 + 18);
    EXPECT_EQ(tree.get(27), 54 + 18);
    EXPECT_EQ(tree.get(36), 54 + 18);
    EXPECT_EQ(tree.get(37), 54);
}

TEST(LazySegmentTree, TwoSameUpdates) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    tree.update(15, 37, 18);
    tree.update(15, 37, 14);
    EXPECT_EQ(tree.get(5), 54);
    EXPECT_EQ(tree.get(14), 54);
    EXPECT_EQ(tree.get(15), 54 + 18 + 14);
    EXPECT_EQ(tree.get(27), 54 + 18 + 14);
    EXPECT_EQ(tree.get(36), 54 + 18 + 14);
    EXPECT_EQ(tree.get(37), 54);
}

TEST(LazySegmentTree, TwoIntersectingUpdates) {
    auto tree = LazySegmentTree<int, int>(0, 42, 54);
    tree.update(15, 37, 18);
    tree.update(20, 41, 14);
    EXPECT_EQ(tree.get(5), 54);
    EXPECT_EQ(tree.get(14), 54);
    EXPECT_EQ(tree.get(15), 54 + 18);
    EXPECT_EQ(tree.get(17), 54 + 18);
    EXPECT_EQ(tree.get(19), 54 + 18);
    EXPECT_EQ(tree.get(20), 54 + 18 + 14);
    EXPECT_EQ(tree.get(25), 54 + 18 + 14);
    EXPECT_EQ(tree.get(36), 54 + 18 + 14);
    EXPECT_EQ(tree.get(37), 54 + 14);
    EXPECT_EQ(tree.get(39), 54 + 14);
    EXPECT_EQ(tree.get(41), 54);
    EXPECT_EQ(tree.get(42), 0);
}
