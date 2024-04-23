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
#include "tools/non_movable_and_non_copyable.hpp"
#include "tools/non_movable_and_non_copyable_avg_combiner.hpp"

using dst::DynamicAvgSegmentTree;
using std::size_t;
using std::views::iota;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-owning-memory, cert-msc51-cpp, cert-msc32-c,
// cert-err58-cpp)

TEST(DynamicAvgSegmentTree, Construct) {
  constexpr auto kTreeEnd = size_t{57};
  [[maybe_unused]] auto tree = DynamicAvgSegmentTree<int, float>(2, kTreeEnd);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGet) {
  constexpr auto kTreeEnd = size_t{57};
  constexpr auto kFillValue = 42.f;

  const auto tree = DynamicAvgSegmentTree<int, float>(2, kTreeEnd, kFillValue);

  const auto avg = tree.rangeGet(3, 16);
  EXPECT_FLOAT_EQ(avg, kFillValue);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSet) {
  constexpr auto kFillValue = 42.f;
  auto tree = DynamicAvgSegmentTree<int, float>(0, 4, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(2, 4, kSetValue);

  const auto avg = tree.rangeGet(0, 4);
  EXPECT_FLOAT_EQ(avg, (kSetValue + kFillValue) / 2.f);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSetOneToTwo) {
  constexpr auto kTreeEnd = size_t{6};
  constexpr auto kFillValue = 42.f;
  auto tree = DynamicAvgSegmentTree<int, float>(0, kTreeEnd, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(4, kTreeEnd, kSetValue);

  const auto avg = tree.rangeGet(0, kTreeEnd);
  EXPECT_FLOAT_EQ(avg, (kSetValue + kFillValue * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, SimpleRangeGetAfterSetTwoToOne) {
  constexpr auto kTreeEnd = size_t{6};
  constexpr auto kFillValue = 42.f;
  auto tree = DynamicAvgSegmentTree<int, float>(0, kTreeEnd, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(0, 2, kSetValue);

  const auto avg = tree.rangeGet(0, kTreeEnd);
  EXPECT_FLOAT_EQ(avg, (kSetValue + kFillValue * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, Copy) {
  constexpr auto kTreeEnd = size_t{6};
  constexpr auto kFillValue = 42.f;
  auto tree = DynamicAvgSegmentTree<int, float>(0, kTreeEnd, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(0, 2, kSetValue);
  auto copy = std::as_const(tree);

  const auto avg = copy.rangeGet(0, kTreeEnd);
  EXPECT_FLOAT_EQ(avg, (kSetValue + kFillValue * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, Move) {
  constexpr auto kTreeEnd = size_t{6};
  constexpr auto kFillValue = 42.f;
  auto tree = DynamicAvgSegmentTree<int, float>(0, kTreeEnd, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(0, 2, kSetValue);
  const auto moved = std::move(tree);

  const auto avg = moved.rangeGet(0, kTreeEnd);
  EXPECT_FLOAT_EQ(avg, (kSetValue + kFillValue * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, NonCopyableGetValueT) {
  constexpr auto kTreeEnd = size_t{6};
  constexpr auto kFillValue = 42.f;
  auto tree =
      dst::DynamicSegmentTree<int, float, NonMovableAndNonCopyable<float>,
                              NonMovableAndNonCopyableAvgCombiner<float, int>,
                              NonMovableAndNonCopyable<float>::Construct>(
          0, kTreeEnd, kFillValue);

  constexpr auto kSetValue = 37.f;
  tree.set(0, 2, kSetValue);
  const auto moved = std::move(tree);

  const auto avg = moved.rangeGet(0, kTreeEnd);
  EXPECT_FLOAT_EQ(avg.get(), (kSetValue + kFillValue * 2) / 3.f);
}

TEST(DynamicAvgSegmentTree, SetRangeGetAvg) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = DynamicAvgSegmentTree<size_t, float>(0, kTreeEnd, 0.f);
  auto reference = AvgSegTreeReference<size_t, float>(0, kTreeEnd, 0.f);

  constexpr auto kGenSeed = 42U;
  std::mt19937 gen(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    const auto valToSet = std::uniform_real_distribution(0.f, 1000.f)(gen);
    tree.set(rngBegin, rngEnd, valToSet);
    reference.set(rngBegin, rngEnd, valToSet);
  }

  for ([[maybe_unused]] size_t iterNum : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    const auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    const auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_LE(std::abs((treeRes - refRes) / refRes), 1e-4);
  }
}

/*
TEST(DynamicAvgSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = DynamicAvgSegmentTree<size_t, float, float, std::plus<float>>(
      0, kTreeEnd, 0);
  auto reference = AvgSegTreeReference<size_t, float>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 54U;
  std::mt19937 gen(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    if (std::bernoulli_distribution()(gen)) {
      const auto setVal = std::uniform_real_distribution(0.f, 1000.f)(gen);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto updateVal = std::uniform_real_distribution(0.f, 1000.f)(gen);
      tree.update(rngBegin, rngEnd, updateVal);
      reference.update(rngBegin, rngEnd, std::plus<>{}, updateVal);
    }
  }

  for ([[maybe_unused]] size_t iterNum : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(gen);
    const auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    const auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_LE(std::abs((treeRes - refRes) / refRes), 1e-3);
  }
}
*/

// NOLINTEND(cppcoreguidelines-owning-memory, cert-msc51-cpp, cert-msc32-c,
// cert-err58-cpp)
