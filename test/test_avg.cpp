////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_avg_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/avg_seg_tree_reference.hpp"
#include "tools/generate_index_range.hpp"

using dst::DynamicAvgSegmentTree;
using std::size_t;
using std::views::iota;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(DynamicAvgSegmentTree, Construct) {
  [[maybe_unused]] auto tree = DynamicAvgSegmentTree<int, float>(2, 57);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGet) {
  auto tree = DynamicAvgSegmentTree<int, float>(2, 57, 42.f);

  const auto avg = tree.rangeGet(3, 16);
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

TEST(DynamicAvgSegmentTree, Copy) {
  auto tree = DynamicAvgSegmentTree<int, float>(0, 6, 42.f);
  tree.set(0, 2, 37.f);
  auto copy = std::as_const(tree);

  auto avg = copy.rangeGet(0, 6);
  EXPECT_FLOAT_EQ(avg, (37.f + 42.f * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, Move) {
  auto tree = DynamicAvgSegmentTree<int, float>(0, 6, 42.f);
  tree.set(0, 2, 37.f);
  auto moved = std::move(tree);

  auto avg = moved.rangeGet(0, 6);
  EXPECT_FLOAT_EQ(avg, (37.f + 42.f * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, SetRangeGetAvg) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = DynamicAvgSegmentTree<size_t, float>(0, treeEnd, 0.f);
  auto reference = AvgSegTreeReference<size_t, float>(0, treeEnd, 0.f);

  std::mt19937 gen(42);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    const auto valToSet = std::uniform_real_distribution(0.f, 1000.f)(gen);
    tree.set(rngBegin, rngEnd, valToSet);
    reference.set(rngBegin, rngEnd, valToSet);
  }

  for (size_t i : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_FLOAT_EQ(treeRes, refRes);
  }
}

TEST(DynamicAvgSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = DynamicAvgSegmentTree<size_t, float, float, std::plus<float>>(
      0, treeEnd, 0);
  auto reference = AvgSegTreeReference<size_t, float>(0, treeEnd, 0);

  std::mt19937 gen(54);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    if (std::bernoulli_distribution()(gen)) {
      const auto setVal = std::uniform_real_distribution(0.f, 1000.f)(gen);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto updateVal = std::uniform_real_distribution(0.f, 1000.f)(gen);
      tree.update(rngBegin, rngEnd, updateVal);
      reference.update(rngBegin, rngEnd, std::plus<int>(), updateVal);
    }
  }

  for (size_t i : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(gen);
    auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_FLOAT_EQ(treeRes, refRes);
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
