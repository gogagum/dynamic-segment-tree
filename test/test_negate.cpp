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

// NOLINTBEGIN(cppcoreguidelines-owning-memory, cert-err58-cpp, cert-msc51-cpp,
// cert-msc32-c)

TEST(DynamicNegateSegmentTree, Construct) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 13;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);
}

TEST(DynamicNegateSegmentTree, Update) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 13;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);
  constexpr auto kUpdateOpEnd = 5;
  tree.update(2, kUpdateOpEnd);
  EXPECT_EQ(tree.get(4), -13);
}

TEST(DynamicNegateSegmentTree, DoubleUpdate) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 13;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);
  constexpr auto kUpdateOpEnd = 5;
  tree.update(2, kUpdateOpEnd);
  tree.update(2, kUpdateOpEnd);
  EXPECT_EQ(tree.get(4), 13);
}

TEST(DynamicNegateSegmentTree, UpdateAndSet) {
  constexpr auto kTreeEnd = 43;
  constexpr auto kFillValue = 5;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpEnd = 7;
  tree.update(4, kUpdateOpEnd);

  constexpr auto kSetOpBegin = 5;
  constexpr auto kSetOpEnd = 11;
  tree.set(kSetOpBegin, kSetOpEnd, 3);

  EXPECT_EQ(tree.get(7), 3);
  EXPECT_EQ(tree.get(4), -5);
  EXPECT_EQ(tree.get(41), 5);
  EXPECT_EQ(tree.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndCopy) {
  constexpr auto kTreeEnd = 43;
  constexpr auto kFillValue = 5;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpEnd = 7;
  tree.update(4, kUpdateOpEnd);

  constexpr auto kSetOpBegin = 5;
  constexpr auto kSetOpEnd = 11;
  tree.set(kSetOpBegin, kSetOpEnd, 3);

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
  using DST = NegateSumDynamicSegmentTree<int, int>;

  constexpr auto kTreeBegin = -14;
  constexpr auto kTreeEnd = 43;
  constexpr auto kFillValue = 5;
  auto tree = DST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kDestBegin = -11;
  constexpr auto kDestEnd = 32;
  constexpr auto kDestFillValue = -23;
  auto copy = DST(kDestBegin, kDestEnd, kDestFillValue);

  constexpr auto kUpdateOpEnd = 7;
  tree.update(4, kUpdateOpEnd);

  constexpr auto kSetOpBegin = 5;
  constexpr auto kSetOpEnd = 11;
  tree.set(kSetOpBegin, kSetOpEnd, 3);

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
  constexpr auto kTreeEnd = 43;
  constexpr auto kFillValue = 5;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpEnd = 7;
  tree.update(4, kUpdateOpEnd);

  constexpr auto kSetOpBegin = 5;
  constexpr auto kSetOpEnd = 11;
  tree.set(kSetOpBegin, kSetOpEnd, 3);

  const auto dest = std::move(tree);

  EXPECT_EQ(dest.get(7), 3);
  EXPECT_EQ(dest.get(4), -5);
  EXPECT_EQ(dest.get(41), 5);
  EXPECT_EQ(dest.get(3), 5);
}

TEST(DynamicNegateSegmentTree, UpdateSetAndMoveAssign) {
  constexpr auto kTreeEnd = 43;
  constexpr auto kFillValue = 5;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);

  constexpr auto kDestFillValue = 11;
  auto dest = NegateSumDynamicSegmentTree<int, int>(3, 4, kDestFillValue);

  constexpr auto kUpdateOpEnd = 7;
  tree.update(4, kUpdateOpEnd);

  constexpr auto kSetBegin = 5;
  constexpr auto kSetEnd = 11;
  tree.set(kSetBegin, kSetEnd, 3);

  dest = std::move(tree);

  EXPECT_EQ(dest.get(7), 3);
  EXPECT_EQ(dest.get(4), -5);
  EXPECT_EQ(dest.get(41), 5);
  EXPECT_EQ(dest.get(3), 5);
}

TEST(DynamicNegateSegmentTree, TwoIntersectingUpdate) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 13;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);
  {
    constexpr auto kUpdateOpEnd = 10;
    tree.update(2, kUpdateOpEnd);
  }
  {
    constexpr auto kUpdateOpBegin = 7;
    constexpr auto kUpdateOpEnd = 21;
    tree.update(kUpdateOpBegin, kUpdateOpEnd);
  }
  EXPECT_EQ(tree.get(8), 13);
  EXPECT_EQ(tree.get(3), -13);
  EXPECT_EQ(tree.get(13), -13);
}

TEST(DynamicNegateSegmentTree, SimpleRangeGet) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 13;
  auto tree = NegateSumDynamicSegmentTree<int, int>(0, kTreeEnd, kFillValue);
  {
    constexpr auto kUpdateOpEnd = 10;
    tree.update(2, kUpdateOpEnd);
  }
  {
    constexpr auto kUpdateOpBegin = 7;
    constexpr auto kUpdateOpEnd = 21;
    tree.update(kUpdateOpBegin, kUpdateOpEnd);
  }
  EXPECT_EQ(tree.get(8), 13);
  EXPECT_EQ(tree.get(3), -13);
  EXPECT_EQ(tree.get(13), -13);
}

TEST(DynamicNegateSegmentTree, UpdateFuzzTest) {
  constexpr auto kTreeEnd = size_t{1000};
  constexpr auto kFillVal = 42;
  auto tree = NegateSumDynamicSegmentTree<size_t, int>(0, kTreeEnd, kFillVal);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, kFillVal);

  constexpr auto kGenSeed = 37U;
  auto gen = std::mt19937(kGenSeed);

  for (size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    tree.update(rngBegin, rngEnd);
    reference.update(rngBegin, rngEnd, knegateOp);

    for (size_t idx : iota(size_t{0}, kTreeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

TEST(DynamicNegateSegmentTree, FuzzTestMixedSetUpdateGet) {
  constexpr auto kTreeEnd = size_t{1000};
  constexpr auto kFillValue = 42;
  auto tree = NegateSumDynamicSegmentTree<size_t, int>(0, kTreeEnd, kFillValue);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, kFillValue);

  constexpr auto kGenSeed = 54U;
  std::mt19937 generator(kGenSeed);

  for (size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);

    if (std::bernoulli_distribution()(generator)) {
      const auto valueToSet = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, valueToSet);
      reference.set(rngBegin, rngEnd, valueToSet);
    } else {
      tree.update(rngBegin, rngEnd);
      reference.update(rngBegin, rngEnd, knegateOp);
    }

    for (size_t idx : iota(size_t{0}, kTreeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

/*

TEST(DynamicNegateSegmentTree, UpdatePlusSetFuzzTest) {
  constexpr auto kTreeEnd = size_t{1000};
  constexpr auto kFillValue = 42;
  auto tree = NegateSumDynamicSegmentTree<size_t, int>(0, kTreeEnd, kFillValue);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, kFillValue);

  constexpr auto kGenSeed = 37U;
  auto gen = std::mt19937(kGenSeed);

  for (size_t iterNum : iota(0, 200)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    tree.update(rngBegin, rngEnd);
    reference.update(rngBegin, rngEnd, knegateOp);
  }

  for (size_t iterNum : iota(0, 400)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    const auto valueToSet = std::uniform_int_distribution(0, 1000)(gen);
    tree.set(rngBegin, rngEnd, valueToSet);
    reference.set(rngBegin, rngEnd, valueToSet);

    for (size_t idx : iota(size_t{0}, kTreeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

*/

// NOLINTEND(cppcoreguidelines-owning-memory, cert-err58-cpp, cert-msc51-cpp,
// cert-msc32-c)
