#include <gtest/gtest.h>

#include <dynamic_negate_segment_tree.hpp>

template <std::integral KeyT, class ValueT, class Allocator = std::allocator<ValueT>>
using NegateSumDynamicSegmentTree =
    dst::DynamicNegateSegmentTree<KeyT, ValueT, ValueT, std::plus<ValueT>, std::multiplies<void>, Allocator>;

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

