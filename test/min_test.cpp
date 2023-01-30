#include <gtest/gtest.h>
#include <lazy_min_segment_tree.hpp>

using lst::LazyMinSegmentTree;

TEST(LazyMinSegmentTree, Construct) {
    auto tree = LazyMinSegmentTree<int, int>(0, 42, 34);
}

TEST(LazyMinSegmentTree, SimpleRangeGet) {
    auto tree = LazyMinSegmentTree<int, int>(0, 42, 34);
    EXPECT_EQ(tree.rangeGet(5, 17), 34);
}

TEST(LazyMinSegmentTree, RangeGetAfterUpdate) {
    auto tree = LazyMinSegmentTree<int, int>(0, 42, 34);
    tree.update(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(5, 17), 34);
    EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(LazyMinSegmentTree, RangeGetAfterSet) {
    auto tree = LazyMinSegmentTree<int, int>(0, 42, 34);
    tree.set(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(13, 20), 4);
    EXPECT_EQ(tree.rangeGet(2, 35), 4);
}


