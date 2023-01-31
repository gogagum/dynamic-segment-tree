#include <gtest/gtest.h>
#include <dynamic_max_segment_tree.hpp>

using dst::DynamicMaxSegmentTree;

TEST(DynamicMaxSegmentTree, Construct) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
}

TEST(DynamicMaxSegmentTree, SimpleRangeGet) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
    EXPECT_EQ(tree.rangeGet(5, 17), 34);
}

TEST(DynamicMaxSegmentTree, RangeGetAfterUpdate) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
    tree.update(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(5, 17), 34 + 4);
    EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMaxSegmentTree, RangeGetAfterSet) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
    tree.set(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(13, 20), 4);
    EXPECT_EQ(tree.rangeGet(2, 35), 34);
}

TEST(DynamicMaxSegmentTree, LadderUpRight) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
    tree.set(41, 42, 10000000);
    tree.set(40, 41, 1000000);
    tree.set(39, 40, 100000);
    tree.set(38, 39, 10000);
    tree.set(37, 38, 1000);
    tree.set(36, 37, 100);
    tree.set(35, 36, 10);
    tree.set(34, 35, 1);

    EXPECT_EQ(tree.rangeGet(0, 42), 10000000);
    EXPECT_EQ(tree.rangeGet(36, 42), 10000000);
    EXPECT_EQ(tree.rangeGet(0, 36), 10);
    EXPECT_EQ(tree.rangeGet(39, 42), 10000000);
    EXPECT_EQ(tree.rangeGet(0, 39), 10000);
}

TEST(DynamicMaxSegmentTree, LadderUpLeft) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
    tree.set(0, 1, 10000000);
    tree.set(1, 2, 1000000);
    tree.set(2, 3, 100000);
    tree.set(3, 4, 10000);
    tree.set(4, 5, 1000);
    tree.set(5, 6, 100);
    tree.set(6, 7, 10);
    tree.set(7, 7, 1);

    EXPECT_EQ(tree.rangeGet(0, 42), 10000000);
    EXPECT_EQ(tree.rangeGet(0, 6), 10000000);
    EXPECT_EQ(tree.rangeGet(6, 42), 10);
    EXPECT_EQ(tree.rangeGet(0, 3), 10000000);
    EXPECT_EQ(tree.rangeGet(3, 42), 10000);
}

TEST(DynamicMaxSegmentTree, LadderDownRight) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
    tree.set(41, 42, -10000000);
    tree.set(40, 41, -1000000);
    tree.set(39, 40, -100000);
    tree.set(38, 39, -10000);
    tree.set(37, 38, -1000);
    tree.set(36, 37, -100);
    tree.set(35, 36, -10);
    tree.set(34, 35, -1);

    EXPECT_EQ(tree.rangeGet(0, 42), 0);
    EXPECT_EQ(tree.rangeGet(36, 42), -100);
    EXPECT_EQ(tree.rangeGet(0, 36), 0);
    EXPECT_EQ(tree.rangeGet(39, 42), -100000);
    EXPECT_EQ(tree.rangeGet(0, 39),  0);
}

TEST(DynamicMaxSegmentTree, LadderDownLeft) {
    auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
    tree.set(0, 1, -10000000);
    tree.set(1, 2, -1000000);
    tree.set(2, 3, -100000);
    tree.set(3, 4, -10000);
    tree.set(4, 5, -1000);
    tree.set(5, 6, -100);
    tree.set(6, 7, -10);
    tree.set(7, 7, -1);

    EXPECT_EQ(tree.rangeGet(0, 42), 0);
    EXPECT_EQ(tree.rangeGet(0, 6), -100);
    EXPECT_EQ(tree.rangeGet(6, 42), 0);
    EXPECT_EQ(tree.rangeGet(0, 3), -100000);
    EXPECT_EQ(tree.rangeGet(3, 42), 0);
}
