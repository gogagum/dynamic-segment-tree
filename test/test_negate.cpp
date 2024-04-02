////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_negate_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/seg_tree_reference_base.hpp"
#include "tools/generate_index_range.hpp"

using std::size_t;
using std::views::iota;
using GenerateIndRng = GenerateIndexRange<size_t>;

constexpr auto knegateOp = [](int num) {
  return -num;
};

template <std::integral KeyT, class ValueT,
          class Allocator = std::allocator<ValueT>>
using NegateSumDynamicSegmentTree =
    dst::DynamicNegateSegmentTree<KeyT, ValueT, ValueT, dst::NoRangeGetOp,
                                  dst::NoRangeGetOp, Allocator>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

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

TEST(DynamicNegateSegmentTree, UpdateAndSet) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 43, 5);
  tree.update(4, 7);
  tree.set(5, 11, 3);
  EXPECT_EQ(tree.get(7), 3);
  EXPECT_EQ(tree.get(4), -5);
  EXPECT_EQ(tree.get(41), 5);
  EXPECT_EQ(tree.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndCopy) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 43, 5);
  tree.update(4, 7);
  tree.set(5, 11, 3);

  const auto copy = tree;

  EXPECT_EQ(tree.get(7), 3);
  EXPECT_EQ(tree.get(4), -5);
  EXPECT_EQ(tree.get(41), 5);
  EXPECT_EQ(tree.get(3), 5);

  EXPECT_EQ(copy.get(7), 3);
  EXPECT_EQ(copy.get(4), -5);
  EXPECT_EQ(copy.get(41), 5);
  EXPECT_EQ(copy.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndCopyAssign) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 43, 5);
  auto copy = NegateSumDynamicSegmentTree<int, int>(0, 32, 5);;

  tree.update(4, 7);
  tree.set(5, 11, 3);

  copy = tree;

  EXPECT_EQ(tree.get(7), 3);
  EXPECT_EQ(tree.get(4), -5);
  EXPECT_EQ(tree.get(41), 5);
  EXPECT_EQ(tree.get(3), 5);

  EXPECT_EQ(copy.get(7), 3);
  EXPECT_EQ(copy.get(4), -5);
  EXPECT_EQ(copy.get(41), 5);
  EXPECT_EQ(copy.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndMove) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 43, 5);
  tree.update(4, 7);
  tree.set(5, 11, 3);

  const auto dest = std::move(tree);

  EXPECT_EQ(dest.get(7), 3);
  EXPECT_EQ(dest.get(4), -5);
  EXPECT_EQ(dest.get(41), 5);
  EXPECT_EQ(dest.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndMoveAssign) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 43, 5);
  auto dest = NegateSumDynamicSegmentTree<int, int>(3, 4, 11);

  tree.update(4, 7);
  tree.set(5, 11, 3);

  dest = std::move(tree);

  EXPECT_EQ(dest.get(7), 3);
  EXPECT_EQ(dest.get(4), -5);
  EXPECT_EQ(dest.get(41), 5);
  EXPECT_EQ(dest.get(3), 5);
}

TEST(DynamicNegateSegmentTree, TwoIntersectingUpdate) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
  tree.update(2, 10);
  tree.update(7, 21);
  EXPECT_EQ(tree.get(8), 13);
  EXPECT_EQ(tree.get(3), -13);
  EXPECT_EQ(tree.get(13), -13);
}

TEST(DynamicNegateSegmentTree, SimpleRangeGet) {
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, 42, 13);
  tree.update(2, 10);
  tree.update(7, 21);
  EXPECT_EQ(tree.get(8), 13);
  EXPECT_EQ(tree.get(3), -13);
  EXPECT_EQ(tree.get(13), -13);
}

TEST(DynamicNegateSegmentTree, UpdateFuzzTest) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, treeEnd, 42);
  auto reference = SegTreeReferenceBase<int, int>(0, treeEnd, 42);

  auto gen = std::mt19937(37);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    tree.update(rngBegin, rngEnd);
    reference.update(rngBegin, rngEnd, knegateOp);

    for (size_t idx : iota(size_t{0}, treeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

TEST(DynamicNegateSegmentTree, FuzzTestMixedSetUpdateGet) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = NegateSumDynamicSegmentTree<size_t, int>(0, treeEnd, 42);
  auto reference = SegTreeReferenceBase<size_t, int>(0, treeEnd, 42);

  std::mt19937 generator(54);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);

    if (std::bernoulli_distribution()(generator)) {
      const auto valueToSet = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, valueToSet);
      reference.set(rngBegin, rngEnd, valueToSet);
    } else {
      tree.update(rngBegin, rngEnd);
      reference.update(rngBegin, rngEnd, knegateOp);
    }

    for (size_t idx : iota(size_t{0}, treeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

TEST(DynamicNegateSegmentTree, UpdatePlusSetFuzzTest) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, treeEnd, 42);
  auto reference = SegTreeReferenceBase<int, int>(0, treeEnd, 42);

  auto gen = std::mt19937(37);

  for (size_t i : iota(0, 200)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    tree.update(rngBegin, rngEnd);
    reference.update(rngBegin, rngEnd, knegateOp);
  }

  for (size_t i : iota(0, 400)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    const auto valueToSet = std::uniform_int_distribution(0, 1000)(gen);
    tree.set(rngBegin, rngEnd, valueToSet);
    reference.set(rngBegin, rngEnd, valueToSet);

    for (size_t idx : iota(size_t{0}, treeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
