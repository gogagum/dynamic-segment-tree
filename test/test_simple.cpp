#include <gtest/gtest.h>

#include <simple_get_set_dynamic_segment_tree.hpp>

using dst::SimpleGetSetDynamicSegmentTree;

TEST(SimpleGetSetDynamicSegmentTree, Construct) {
    auto tree = SimpleGetSetDynamicSegmentTree<int, long long>(-5, 100, 77);
}

TEST(SimpleGetSetDynamicSegmentTree, RangeSet) {
    auto tree = SimpleGetSetDynamicSegmentTree<int, long long>(0, 42, 77);

    tree.set(13, 17, 56);

    EXPECT_EQ(tree.get(13), 56);
    EXPECT_EQ(tree.get(15), 56);
    EXPECT_EQ(tree.get(16), 56);
    EXPECT_EQ(tree.get(17), 77);
    EXPECT_EQ(tree.get(8), 77);
    EXPECT_EQ(tree.get(37), 77);
}
