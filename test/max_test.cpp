#include <gtest/gtest.h>
#include <lazy_max_segment_tree.hpp>

using lst::LazyMaxSegmentTree;

TEST(LazyMaxSegmentTree, Construct) {
    auto tree = LazyMaxSegmentTree<int, int>(0, 42, 34);
}

TEST(LazyMaxSegmentTree, SimpleRangeGet) {
    auto tree = LazyMaxSegmentTree<int, int>(0, 42, 34);
    EXPECT_EQ(tree.rangeGet(5, 17), 34);
}

TEST(LazyMaxSegmentTree, RangeGetAfterUpdate) {
    auto tree = LazyMaxSegmentTree<int, int>(0, 42, 34);
    tree.update(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(5, 17), 34 + 4);
    EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(LazyMaxSegmentTree, RangeGetAfterSet) {
    auto tree = LazyMaxSegmentTree<int, int>(0, 42, 34);
    tree.set(12, 22, 4);
    EXPECT_EQ(tree.rangeGet(13, 20), 4);
    EXPECT_EQ(tree.rangeGet(2, 35), 34);
}
