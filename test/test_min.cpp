////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <cstddef>
#include <dst/partial/dynamic_min_segment_tree.hpp>
#include <random>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/zip.hpp>
#include <ranges>

#include "reference/min_seg_tree_reference.hpp"
#include "tools/generate_index_range.hpp"

using dst::DynamicMinSegmentTree;
using ranges::views::reverse;
using ranges::views::zip;
using std::size_t;
using std::views::iota;
using UniformSizeTDistr = std::uniform_int_distribution<size_t>;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-owning-memory, cert-err58-cpp, cert-msc51-cpp,
// cert-msc32-c)

TEST(DynamicMinSegmentTree, Construct) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, kFillValue);
}

TEST(DynamicMinSegmentTree, SimpleRangeGet) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, kFillValue);
  EXPECT_EQ(tree.rangeGet(5, 17), 34);
}

TEST(DynamicMinSegmentTree, RangeGetAfterUpdate) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree =
      DynamicMinSegmentTree<int, int, std::plus<int>>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, RangeGetAfterSet) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.set(kUpdateOpBegin, kUpdateOpEnd, 4);

  EXPECT_EQ(tree.rangeGet(13, 20), 4);
  EXPECT_EQ(tree.rangeGet(2, 35), 4);
}

TEST(DynamicMinSegmentTree, UpdateAndSet) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree =
      DynamicMinSegmentTree<int, int, std::plus<int>>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 38);
}

TEST(DynamicMinSegmentTree, UpdateSetAndCopy) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree =
      DynamicMinSegmentTree<int, int, std::plus<int>>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);
  const auto copy = std::as_const(tree);

  EXPECT_EQ(copy.rangeGet(5, 17), 34);
  EXPECT_EQ(copy.rangeGet(12, 18), 34 + 4);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, UpdateSetAndCopyWithSpecifiedAllocator) {
  using DST = DynamicMinSegmentTree<int, int, std::plus<int>>;
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DST(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  const DynamicMinSegmentTree<int, int, std::plus<int>> copy(
      std::as_const(tree), std::allocator<int>{});

  EXPECT_EQ(copy.rangeGet(5, 17), 34);
  EXPECT_EQ(copy.rangeGet(12, 18), 34 + 4);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, UpdateSetAndCopyAssign) {
  using DST = DynamicMinSegmentTree<int, int, std::plus<int>>;
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DST(0, kTreeEnd, kFillValue);
  constexpr auto kDestTreeEnd = 37;
  auto dest = DST(0, kDestTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  dest = std::as_const(tree);

  EXPECT_EQ(dest.rangeGet(5, 17), 34);
  EXPECT_EQ(dest.rangeGet(12, 18), 34 + 4);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, CopyAssignToNonEmpty) {
  using DST = DynamicMinSegmentTree<int, int, std::plus<int>>;
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DST(0, kTreeEnd, kFillValue);
  constexpr auto kDestTreeEnd = 37;
  auto dest = DST(0, kDestTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  dest.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  dest.set(kSetOpBegin, kSetOpEnd, kSetValue);

  dest = tree;

  EXPECT_EQ(dest.rangeGet(5, 17), 34);
  EXPECT_EQ(dest.rangeGet(12, 18), 34);

  EXPECT_EQ(tree.rangeGet(5, 17), 34);
  EXPECT_EQ(tree.rangeGet(12, 18), 34);
}

TEST(DynamicMinSegmentTree, UpdateSetAndMove) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree =
      DynamicMinSegmentTree<int, int, std::plus<int>>(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  const auto moved = std::move(tree);

  EXPECT_EQ(moved.rangeGet(5, 17), 34);
  EXPECT_EQ(moved.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, UpdateSetAndMoveWithSpecifiedAllocator) {
  using DST = DynamicMinSegmentTree<int, int, std::plus<int>>;

  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DST(0, kTreeEnd, kFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  const DST moved(std::move(tree), std::allocator<int>{});

  EXPECT_EQ(moved.rangeGet(5, 17), 34);
  EXPECT_EQ(moved.rangeGet(12, 18), 38);
}

TEST(DynamicMinSegmentTree, UpdateSetAndMoveAssign) {
  using DST = DynamicMinSegmentTree<int, int, std::plus<int>>;

  constexpr auto kTreeBegin = -17;
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 34;
  auto tree = DST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kDestTreeBegin = -14;
  constexpr auto kDestTreeEnd = 37;
  constexpr auto kDestFillValue = 3;
  auto dest = DST(kDestTreeBegin, kDestTreeEnd, kDestFillValue);

  constexpr auto kUpdateOpBegin = 12;
  constexpr auto kUpdateOpEnd = 22;
  tree.update(kUpdateOpBegin, kUpdateOpEnd, 4);

  constexpr auto kSetOpBegin = 17;
  constexpr auto kSetOpEnd = 27;
  constexpr auto kSetValue = 66;
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  dest = std::move(tree);

  EXPECT_EQ(dest.rangeGet(5, 17), 34);
  EXPECT_EQ(dest.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMinSegmentTree, LadderUpRight) {
  constexpr auto kTreeEnd = 42;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, 0);

  constexpr auto kSetBegins = iota(34, 42);
  auto kSetValues = std::array<int, kSetBegins.size()>{
      1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};  // NOLINT
  for (auto [setBegin, setVal] : zip(kSetBegins, kSetValues)) {
    tree.set(setBegin, setBegin + 1, setVal);
  }

  EXPECT_EQ(tree.rangeGet(0, 42), 0);
  EXPECT_EQ(tree.rangeGet(36, 42), 100);
  EXPECT_EQ(tree.rangeGet(0, 36), 0);
  EXPECT_EQ(tree.rangeGet(39, 42), 100000);
  EXPECT_EQ(tree.rangeGet(0, 39), 0);
}

TEST(DynamicMinSegmentTree, LadderUpLeft) {
  constexpr auto kTreeEnd = size_t{42};
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, 0);

  constexpr auto kSetBegins = iota(0, 8);
  auto kSetValues = std::array<int, kSetBegins.size()>{
      1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};  // NOLINT
  for (auto [setBegin, setVal] : zip(kSetBegins, kSetValues | reverse)) {
    tree.set(setBegin, setBegin + 1, setVal);
  }

  EXPECT_EQ(tree.rangeGet(0, 42), 0);
  EXPECT_EQ(tree.rangeGet(0, 6), 100);
  EXPECT_EQ(tree.rangeGet(6, 42), 0);
  EXPECT_EQ(tree.rangeGet(0, 3), 100000);
  EXPECT_EQ(tree.rangeGet(3, 42), 0);
}

TEST(DynamicMinSegmentTree, LadderDownRight) {
  constexpr auto kTreeEnd = 42;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, 0);

  constexpr auto kSetBegins = iota(34, 42);
  auto kSetValues = std::array<int, kSetBegins.size()>{
      -1, -10, -100, -1000, -10000, -100000, -1000000, -10000000};  // NOLINT
  for (auto [setBegin, setVal] : zip(kSetBegins, kSetValues)) {
    tree.set(setBegin, setBegin + 1, setVal);
  }

  EXPECT_EQ(tree.rangeGet(0, 42), -10000000);
  EXPECT_EQ(tree.rangeGet(36, 42), -10000000);
  EXPECT_EQ(tree.rangeGet(0, 36), -10);
  EXPECT_EQ(tree.rangeGet(39, 42), -10000000);
  EXPECT_EQ(tree.rangeGet(0, 39), -10000);
}

TEST(DynamicMinSegmentTree, LadderDownLeft) {
  constexpr auto kTreeEnd = 42;
  auto tree = DynamicMinSegmentTree<int, int>(0, kTreeEnd, 0);

  constexpr auto kSetBegins = iota(0, 8);
  auto kSetValues = std::array<int, kSetBegins.size()>{
      -1, -10, -100, -1000, -10000, -100000, -1000000, -10000000};  // NOLINT
  for (auto [setBegin, setVal] : zip(kSetBegins, kSetValues | reverse)) {
    tree.set(setBegin, setBegin + 1, setVal);
  }

  EXPECT_EQ(tree.rangeGet(0, 42), -10000000);
  EXPECT_EQ(tree.rangeGet(0, 6), -10000000);
  EXPECT_EQ(tree.rangeGet(6, 42), -10);
  EXPECT_EQ(tree.rangeGet(0, 3), -10000000);
  EXPECT_EQ(tree.rangeGet(3, 42), -10000);
}

TEST(DynamicMinSegmentTree, FuzzTestSetUpdateGet) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree =
      DynamicMinSegmentTree<size_t, int, std::plus<int>>(0, kTreeEnd, 0);
  auto reference = MinSegTreeReference<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const int setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
    reference.set(rngBegin, rngEnd, setVal);
  }

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto updVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.update(rngBegin, rngEnd, updVal);
    reference.update(rngBegin, rngEnd, std::plus<>{}, updVal);

    for (auto idx : iota(0, 1000)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

TEST(DynamicMinSegmentTree, FuzzTestMixedSetUpdateGet) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree =
      DynamicMinSegmentTree<size_t, int, std::plus<int>>(0, kTreeEnd, 0);
  auto reference = MinSegTreeReference<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 54U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);

    if (std::bernoulli_distribution()(generator)) {
      const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto updVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.update(rngBegin, rngEnd, updVal);
      reference.update(rngBegin, rngEnd, std::plus<>(), updVal);
    }

    for (size_t idx : iota(size_t{0}, kTreeEnd)) {
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

TEST(DynamicMinSegmentTree, FuzzTestSetRangeGet) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = DynamicMinSegmentTree<size_t, int>(0, kTreeEnd, 0);
  auto reference = MinSegTreeReference<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
    reference.set(rngBegin, rngEnd, setVal);
  }

  for ([[maybe_unused]] size_t iterNum : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    const auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_EQ(treeRes, refRes);
  }
}

/*
TEST(DynamicMinSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree =
      DynamicMinSegmentTree<size_t, int, std::plus<int>>(0, kTreeEnd, 0);
  auto reference = MinSegTreeReference<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 54U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    if (std::bernoulli_distribution()(generator)) {
      const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto updVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.update(rngBegin, rngEnd, updVal);
      reference.update(rngBegin, rngEnd, std::plus<>(), updVal);
    }
  }

  for ([[maybe_unused]] size_t iterNum : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    const auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_EQ(treeRes, refRes);
  }
}
*/

// NOLINTEND(cppcoreguidelines-owning-memory, cert-err58-cpp, cert-msc51-cpp,
// cert-msc32-c)
