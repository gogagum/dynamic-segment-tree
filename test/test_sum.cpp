////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <dst/partial/dynamic_sum_segment_tree.hpp>
#include <limits>
#include <random>
#include <ranges>

#include "reference/sum_seg_tree_reference.hpp"

using dst::DynamicSumSegmentTree;
namespace rng = std::ranges;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(DynamicSumSegmentTree, Construct) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
}

TEST(DynamicSumSegmentTree, QueryMiddle) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_EQ(tree.get(21), 54);
}

TEST(DynamicSumSegmentTree, QueryEnd) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(42), std::out_of_range);
}

TEST(DynamicSumSegmentTree, QueryAfterEnd) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(73), std::out_of_range);
}

TEST(DynamicSumSegmentTree, QueryBegin) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_EQ(tree.get(0), 54);
}

TEST(DynamicSumSegmentTree, QueryBeforeBegin) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(-5), std::out_of_range);
}

TEST(DynamicSumSegmentTree, OneUpdate) {
  auto tree = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
  tree.update(15, 37, 18);
  EXPECT_EQ(tree.get(5), 54);
  EXPECT_EQ(tree.get(14), 54);
  EXPECT_EQ(tree.get(15), 54 + 18);
  EXPECT_EQ(tree.get(27), 54 + 18);
  EXPECT_EQ(tree.get(36), 54 + 18);
  EXPECT_EQ(tree.get(37), 54);
}

TEST(DynamicSumSegmentTree, TwoSameUpdates) {
  auto tree = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
  tree.update(15, 37, 18);
  tree.update(15, 37, 14);
  EXPECT_EQ(tree.get(5), 54);
  EXPECT_EQ(tree.get(14), 54);
  EXPECT_EQ(tree.get(15), 54 + 18 + 14);
  EXPECT_EQ(tree.get(27), 54 + 18 + 14);
  EXPECT_EQ(tree.get(36), 54 + 18 + 14);
  EXPECT_EQ(tree.get(37), 54);
}

TEST(DynamicSumSegmentTree, TwoIntersectingUpdates) {
  auto tree = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
  tree.update(15, 37, 18);
  tree.update(20, 41, 14);
  EXPECT_EQ(tree.get(5), 54);
  EXPECT_EQ(tree.get(14), 54);
  EXPECT_EQ(tree.get(15), 54 + 18);
  EXPECT_EQ(tree.get(17), 54 + 18);
  EXPECT_EQ(tree.get(19), 54 + 18);
  EXPECT_EQ(tree.get(20), 54 + 18 + 14);
  EXPECT_EQ(tree.get(25), 54 + 18 + 14);
  EXPECT_EQ(tree.get(35), 54 + 18 + 14);
  EXPECT_EQ(tree.get(36), 54 + 18 + 14);
  EXPECT_EQ(tree.get(37), 54 + 14);
  EXPECT_EQ(tree.get(38), 54 + 14);
  EXPECT_EQ(tree.get(39), 54 + 14);
  EXPECT_EQ(tree.get(40), 54 + 14);
  EXPECT_EQ(tree.get(41), 54);
  EXPECT_THROW(tree.get(42), std::out_of_range);
}

TEST(DynamicSumSegmentTree, TwoNonIntersectingUpdates) {
  auto tree = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
  tree.update(5, 8, 18);
  tree.update(17, 38, 14);
  EXPECT_EQ(tree.get(3), 54);
  EXPECT_EQ(tree.get(5), 54 + 18);
  EXPECT_EQ(tree.get(6), 54 + 18);
  EXPECT_EQ(tree.get(8), 54);
  EXPECT_EQ(tree.get(12), 54);
  EXPECT_EQ(tree.get(16), 54);
  EXPECT_EQ(tree.get(17), 54 + 14);
  EXPECT_EQ(tree.get(23), 54 + 14);
  EXPECT_EQ(tree.get(38), 54);
  EXPECT_EQ(tree.get(40), 54);
  EXPECT_THROW(tree.get(42), std::out_of_range);
}

TEST(DynamicSumSegmentTree, SimpleRangeGetAll) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  auto rangeGetResult = tree.rangeGet(0, 42);
  EXPECT_EQ(rangeGetResult, 54 * 42);
}

TEST(DynamicSumSegmentTree, SimpleRangeGetPart) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  auto rangeGetResult = tree.rangeGet(12, 23);
  EXPECT_EQ(rangeGetResult, 54 * (23 - 12));
}

TEST(DynamicSumSegmentTree, SimpleRangeGetMoreThanAll) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  auto rangeGetResult = tree.rangeGet(-5, 50);
  EXPECT_EQ(rangeGetResult, 54 * 42);
}

TEST(DynamicSumSegmentTree, RangeGetAfterUpdate) {
  auto tree = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
  tree.update(15, 37, 18);

  auto getInsideUpdateRes = tree.rangeGet(17, 30);
  EXPECT_EQ(getInsideUpdateRes, (54 + 18) * (30 - 17));

  auto getOfUpdateSizeRes = tree.rangeGet(15, 37);
  EXPECT_EQ(getOfUpdateSizeRes, (54 + 18) * (37 - 15));

  auto getGreaterThanUpdateSizeRes = tree.rangeGet(5, 40);
  EXPECT_EQ(getGreaterThanUpdateSizeRes, 54 * (40 - 5) + 18 * (37 - 15));

  auto intersectingGet1Res = tree.rangeGet(5, 30);
  EXPECT_EQ(intersectingGet1Res, 54 * (30 - 5) + 18 * (30 - 15));

  auto intersectingGet2Res = tree.rangeGet(30, 40);
  EXPECT_EQ(intersectingGet2Res, 54 * (40 - 30) + 18 * (37 - 30));
}

TEST(DynamicSumSegmentTree, LongLongToInt) {
  auto tree = DynamicSumSegmentTree<long long, int>(-100500, 100500, 42);

  auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 42);
}

TEST(DynamicSumSegmentTree, IntToLong) {
  auto tree = DynamicSumSegmentTree<int, long long, long long>(
      -100500, 100500, 1234567890098765432);

  auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 1234567890098765432);
}

TEST(DynamicSumSegmentTree, LongLongToLongLong) {
  auto tree = DynamicSumSegmentTree<long long, long long>(-100500, 100500,
                                                          1234567890098765432);

  auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 1234567890098765432);
}

TEST(DynamicSumSegmentTree, LongLongToLongLongLadder) {
  const auto topBorder = 1000000ll;
  using SegTree = DynamicSumSegmentTree<long long, long long, long long,
                                        std::plus<long long>>;
  auto tree = SegTree(0ll, topBorder);

  tree.update(topBorder - topBorder / 2, topBorder, 1ll);
  tree.update(topBorder - topBorder / 4, topBorder, 10ll);
  tree.update(topBorder - topBorder / 8, topBorder, 100ll);
  tree.update(topBorder - topBorder / 16, topBorder, 1000ll);
  tree.update(topBorder - topBorder / 32, topBorder, 10000ll);
  tree.update(topBorder - topBorder / 64, topBorder, 100000ll);
  tree.update(topBorder - topBorder / 128, topBorder, 1000000ll);
  tree.update(topBorder - topBorder / 256, topBorder, 10000000ll);

  auto testVal1 = tree.get(topBorder - 1);
  EXPECT_EQ(testVal1, 11111111);
  auto testVal2 = tree.get(topBorder - topBorder / 32 - 1);
  EXPECT_EQ(testVal2, 1111);
}

TEST(DynamicSumSegmentTree, Set) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 21);
  tree.set(0, 37, 73);
  EXPECT_EQ(tree.get(0), 73);
  EXPECT_EQ(tree.get(15), 73);
  EXPECT_EQ(tree.get(37), 21);
  EXPECT_EQ(tree.get(40), 21);
}

TEST(DynamicSumSegmentTree, SetOnTheSameRange) {
  auto tree = DynamicSumSegmentTree<int, int>(0, 42, 21);
  tree.set(0, 37, 73);
  tree.set(0, 37, 37);
  EXPECT_EQ(tree.get(0), 37);
  EXPECT_EQ(tree.get(15), 37);
  EXPECT_EQ(tree.get(37), 21);
  EXPECT_EQ(tree.get(40), 21);
}

TEST(DynamicSumSegmentTree, IntRangesGiveInt64Sum) {
  auto tree = DynamicSumSegmentTree<int, int, std::int64_t>(0, 3, 0);
  tree.set(0, 1, std::numeric_limits<int>::max() - 13);
  tree.set(1, 3, std::numeric_limits<int>::max() - 7);

  const long long part1LL =
      static_cast<std::int64_t>(std::numeric_limits<int>::max() - 13);

  const long long part2LL =
      static_cast<std::int64_t>(std::numeric_limits<int>::max() - 7);

  EXPECT_EQ(tree.rangeGet(0, 3), part1LL + part2LL * 2);
}

TEST(DynamicSumSegmentTree, IntRangesGiveInt64SumLongRange) {
  auto tree = DynamicSumSegmentTree<int, int, std::int64_t>(0, 100, 0);
  tree.set(0, 42, std::numeric_limits<int>::max() / 3);
  tree.set(42, 100, std::numeric_limits<int>::max() / 7);

  const auto part1LL =
      static_cast<std::int64_t>(std::numeric_limits<int>::max() / 3);

  const auto part2LL =
      static_cast<std::int64_t>(std::numeric_limits<int>::max() / 7);

  EXPECT_EQ(tree.rangeGet(0, 100), part1LL * 42 + part2LL * 58);
}

TEST(DynamicSumSegmentTree, FuzzTestSetRangeGet) {
  auto tree = DynamicSumSegmentTree<std::size_t, int>(0, 1000, 0);
  auto reference = SumSegTreeReference<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(42);

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;           // [0..500)
    const std::size_t rngLen = generator() % 500;             // [0..500)
    const int setVal = static_cast<int>(generator()) % 1000;  // [0..1000)
    tree.set(rngStart, rngStart + rngLen, setVal);
    reference.set(rngStart, rngStart + rngLen, setVal);
  }

  for (std::size_t i : rng::iota_view(0, 50)) {
    const std::size_t rngStart = generator() % 500;  // [0..500)
    const std::size_t rngLen = generator() % 500;    // [0..500)
    auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
    auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(DynamicSumSementTree, IntRangeGivesInt64SumFuzzTest) {
  std::mt19937 generator(37);

  for (std::size_t i : rng::iota_view(0, 20)) {
    auto tree = DynamicSumSegmentTree<int, int, std::int64_t>(0, 1 << 29, 0);

    const int rng1Start = generator() % (1 << 28);   // [0..2**28)
    const int rng1Length = generator() % (1 << 27);  // [0..2**27)
    const int rng2Length = generator() % (1 << 27);  // [0..2**27)

    const int setVal1 = generator() % (1 << 30);  // [0..2**30)
    const int setVal2 = generator() % (1 << 30);  // [0..2**30)

    tree.set(rng1Start, rng1Start + rng1Length, setVal1);
    tree.set(rng1Start + rng1Length, rng1Start + rng1Length + rng2Length,
             setVal2);

    const auto expectedSum = static_cast<std::int64_t>(setVal1) * rng1Length +
                             static_cast<std::int64_t>(setVal2) * rng2Length;

    const auto rangeGetRes =
        tree.rangeGet(rng1Start, rng1Start + rng1Length + rng2Length);

    EXPECT_EQ(rangeGetRes, expectedSum);
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
