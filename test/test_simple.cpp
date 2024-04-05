////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <array>
#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/seg_tree_reference_base.hpp"
#include "tools/generate_index_range.hpp"

using std::uint64_t;
using I_LL_SimpleDST = dst::DynamicSimpleGetSetSegmentTree<int, int64_t>;
constexpr auto kiLLValGetter = &I_LL_SimpleDST::get;
using std::bind_front;
using std::size_t;
using std::ranges::equal;
using std::views::iota;
using std::views::transform;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, cert-err58-cpp)

TEST(SimpleGetSetDynamicSegmentTree, Construct) {
  constexpr auto kTreeBegin = -5;
  constexpr auto kTreeEnd = 100;
  constexpr auto kFillValue = 77;
  auto tree = I_LL_SimpleDST(kTreeBegin, kTreeEnd, kFillValue);
}

TEST(SimpleGetSetDynamicSegmentTree, RangeSet) {
  constexpr auto kTreeBegin = -17;
  constexpr auto kTreeEnd = 93;
  constexpr auto kFillValue = 77;
  auto tree = I_LL_SimpleDST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto values = kIndicies | transform(bind_front(kiLLValGetter, &tree));

  EXPECT_TRUE(equal(kExpectedVals, values));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndCopy) {
  constexpr auto kTreeBegin = 0;
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = 77;
  auto tree = I_LL_SimpleDST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  auto copy = std::as_const(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = kIndicies | transform(bind_front(kiLLValGetter, &tree));
  const auto treeVals = kIndicies | transform(bind_front(kiLLValGetter, &copy));

  EXPECT_TRUE(equal(kExpectedVals, copyVals));
  EXPECT_TRUE(equal(kExpectedVals, treeVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndCopyAssign) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = I_LL_SimpleDST(0, kTreeEnd, kFillValue);

  constexpr auto kCopyBegin = 5;
  constexpr auto kCopyEnd = 13;
  constexpr auto kCopyFillValue = uint64_t{67};
  auto copy = I_LL_SimpleDST(kCopyBegin, kCopyEnd, kCopyFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  copy = std::as_const(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = kIndicies | transform(bind_front(kiLLValGetter, &tree));
  const auto treeVals = kIndicies | transform(bind_front(kiLLValGetter, &copy));

  EXPECT_TRUE(equal(kExpectedVals, copyVals));
  EXPECT_TRUE(equal(kExpectedVals, treeVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndMove) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = I_LL_SimpleDST(0, kTreeEnd, kFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  auto moved = std::move(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals =
      kIndicies | transform(bind_front(kiLLValGetter, &moved));

  EXPECT_TRUE(equal(kExpectedVals, movedVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndMoveAssign) {
  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = I_LL_SimpleDST(0, kTreeEnd, kFillValue);

  constexpr auto kDestTreeBegin = 5;
  constexpr auto kDestTreeEnd = 13;
  constexpr auto kDestTreeFillValue = 67;
  auto moved = I_LL_SimpleDST(kDestTreeBegin, kDestTreeEnd, kDestTreeFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  moved = std::move(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals =
      kIndicies | transform(bind_front(kiLLValGetter, &moved));

  EXPECT_TRUE(equal(kExpectedVals, movedVals));
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGet) {
  using DST = dst::DynamicSimpleGetSetSegmentTree<size_t, int>;
  using Ref = SegTreeReferenceBase<size_t, int>;
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = DST(0, kTreeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
    reference.set(rngBegin, rngEnd, setVal);

    constexpr auto kids = iota(size_t{0}, kTreeEnd);
    const auto treeVals = kids | transform(bind_front(&DST::get, &tree));
    const auto refVals = kids | transform(bind_front(&Ref::get, &reference));
    EXPECT_TRUE(equal(treeVals, refVals));
  }
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGetMixed) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = dst::DynamicSimpleGetSetSegmentTree<size_t, int>(0, kTreeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);

    if (std::bernoulli_distribution()(generator)) {
      const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto idx = std::uniform_int_distribution<size_t>(0, 999)(generator);
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, cert-err58-cpp)
