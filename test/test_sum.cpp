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
#include "tools/generate_index_range.hpp"

using dst::DynamicSumSegmentTree;
using std::int64_t;
using std::size_t;
using std::views::iota;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(DynamicSumSegmentTree, Construct) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
}

TEST(DynamicSumSegmentTree, QueryMiddle) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_EQ(tree.get(21), 54);
}

TEST(DynamicSumSegmentTree, QueryEnd) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(42), std::out_of_range);
}

TEST(DynamicSumSegmentTree, QueryAfterEnd) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(73), std::out_of_range);
}

TEST(DynamicSumSegmentTree, QueryBegin) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_EQ(tree.get(0), 54);
}

TEST(DynamicSumSegmentTree, QueryBeforeBegin) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  EXPECT_THROW(tree.get(-5), std::out_of_range);
}

TEST(DynamicSumSegmentTree, OneUpdate) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
    ret.update(15, 37, 18);
    return ret;
  }();

  EXPECT_EQ(tree.get(5), 54);
  EXPECT_EQ(tree.get(14), 54);
  EXPECT_EQ(tree.get(15), 54 + 18);
  EXPECT_EQ(tree.get(27), 54 + 18);
  EXPECT_EQ(tree.get(36), 54 + 18);
  EXPECT_EQ(tree.get(37), 54);
}

TEST(DynamicSumSegmentTree, TwoSameUpdates) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
    ret.update(15, 37, 18);
    ret.update(15, 37, 14);
    return ret;
  }();

  EXPECT_EQ(tree.get(5), 54);
  EXPECT_EQ(tree.get(14), 54);
  EXPECT_EQ(tree.get(15), 54 + 18 + 14);
  EXPECT_EQ(tree.get(27), 54 + 18 + 14);
  EXPECT_EQ(tree.get(36), 54 + 18 + 14);
  EXPECT_EQ(tree.get(37), 54);
}

TEST(DynamicSumSegmentTree, TwoIntersectingUpdates) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
    ret.update(15, 37, 18);
    ret.update(20, 41, 14);
    return ret;
  }();

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
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
    ret.update(5, 8, 18);
    ret.update(17, 38, 14);
    return ret;
  }();

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
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  const auto rangeGetResult = tree.rangeGet(0, 42);
  EXPECT_EQ(rangeGetResult, 54 * 42);
}

TEST(DynamicSumSegmentTree, SimpleRangeGetPart) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  const auto rangeGetResult = tree.rangeGet(12, 23);
  EXPECT_EQ(rangeGetResult, 54 * (23 - 12));
}

TEST(DynamicSumSegmentTree, SimpleRangeGetMoreThanAll) {
  const auto tree = DynamicSumSegmentTree<int, int>(0, 42, 54);
  const auto rangeGetResult = tree.rangeGet(-5, 50);
  EXPECT_EQ(rangeGetResult, 54 * 42);
}

TEST(DynamicSumSegmentTree, RangeGetAfterUpdate) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int, std::plus<int>>(0, 42, 54);
    ret.update(15, 37, 18);
    return ret;
  }();

  const auto getInsideUpdateRes = tree.rangeGet(17, 30);
  EXPECT_EQ(getInsideUpdateRes, (54 + 18) * (30 - 17));

  const auto getOfUpdateSizeRes = tree.rangeGet(15, 37);
  EXPECT_EQ(getOfUpdateSizeRes, (54 + 18) * (37 - 15));

  const auto getGreaterThanUpdateSizeRes = tree.rangeGet(5, 40);
  EXPECT_EQ(getGreaterThanUpdateSizeRes, 54 * (40 - 5) + 18 * (37 - 15));

  const auto intersectingGet1Res = tree.rangeGet(5, 30);
  EXPECT_EQ(intersectingGet1Res, 54 * (30 - 5) + 18 * (30 - 15));

  const auto intersectingGet2Res = tree.rangeGet(30, 40);
  EXPECT_EQ(intersectingGet2Res, 54 * (40 - 30) + 18 * (37 - 30));
}

TEST(DynamicSumSegmentTree, LongLongToInt) {
  const auto tree = DynamicSumSegmentTree<long long, int>(-100500, 100500, 42);

  const auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 42);
}

TEST(DynamicSumSegmentTree, IntToLong) {
  const auto tree = DynamicSumSegmentTree<int, long long, long long>(
      -100500, 100500, 1234567890098765432);

  const auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 1234567890098765432);
}

TEST(DynamicSumSegmentTree, LongLongToLongLong) {
  const auto tree = DynamicSumSegmentTree<long long, long long>(
      -100500, 100500, 1234567890098765432);

  const auto testVal = tree.get(17);
  EXPECT_EQ(testVal, 1234567890098765432);
}

TEST(DynamicSumSegmentTree, LongLongToLongLongLadder) {
  const auto topBorder = 1000000ll;
  using SegTree = DynamicSumSegmentTree<long long, long long, long long,
                                        std::plus<long long>>;
  const auto tree = [] {
    auto ret = SegTree(0ll, topBorder);

    ret.update(topBorder - topBorder / 2, topBorder, 1ll);
    ret.update(topBorder - topBorder / 4, topBorder, 10ll);
    ret.update(topBorder - topBorder / 8, topBorder, 100ll);
    ret.update(topBorder - topBorder / 16, topBorder, 1000ll);
    ret.update(topBorder - topBorder / 32, topBorder, 10000ll);
    ret.update(topBorder - topBorder / 64, topBorder, 100000ll);
    ret.update(topBorder - topBorder / 128, topBorder, 1000000ll);
    ret.update(topBorder - topBorder / 256, topBorder, 10000000ll);

    return ret;
  }();

  const auto testVal1 = tree.get(topBorder - 1);
  EXPECT_EQ(testVal1, 11111111);
  const auto testVal2 = tree.get(topBorder - topBorder / 32 - 1);
  EXPECT_EQ(testVal2, 1111);
}

TEST(DynamicSumSegmentTree, Set) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int>(0, 42, 21);
    ret.set(0, 37, 73);
    return ret;
  }();

  EXPECT_EQ(tree.get(0), 73);
  EXPECT_EQ(tree.get(15), 73);
  EXPECT_EQ(tree.get(37), 21);
  EXPECT_EQ(tree.get(40), 21);
}

TEST(DynamicSumSegmentTree, SetOnTheSameRange) {
  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int>(0, 42, 21);
    ret.set(0, 37, 73);
    ret.set(0, 37, 37);
    return ret;
  }();

  EXPECT_EQ(tree.get(0), 37);
  EXPECT_EQ(tree.get(15), 37);
  EXPECT_EQ(tree.get(37), 21);
  EXPECT_EQ(tree.get(40), 21);
}

TEST(DynamicSumSegmentTree, IntRangesGiveInt64Sum) {
  constexpr auto maxInt = std::numeric_limits<int>::max();

  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int64_t>(0, 3, 0);
    ret.set(0, 1, maxInt - 13);
    ret.set(1, 3, maxInt - 7);
    return ret;
  }();

  const auto part1LL = static_cast<int64_t>(maxInt - 13);
  const auto part2LL = static_cast<int64_t>(maxInt - 7);

  EXPECT_EQ(tree.rangeGet(0, 3), part1LL + part2LL * 2);
}

TEST(DynamicSumSegmentTree, IntRangesGiveInt64SumLongRange) {
  constexpr auto maxInt = std::numeric_limits<int>::max();

  const auto tree = [] {
    auto ret = DynamicSumSegmentTree<int, int, int64_t>(0, 100, 0);
    ret.set(0, 42, maxInt / 3);
    ret.set(42, 100, maxInt / 7);
    return ret;
  }();

  const auto part1LL = static_cast<int64_t>(maxInt / 3);
  const auto part2LL = static_cast<int64_t>(maxInt / 7);

  EXPECT_EQ(tree.rangeGet(0, 100), part1LL * 42 + part2LL * 58);
}

TEST(DynamicSumSegmentTree, FuzzTestSetRangeGet) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = DynamicSumSegmentTree<size_t, int>(0, treeEnd, 0);
  auto reference = SumSegTreeReference<size_t, int>(0, treeEnd, 0);

  std::mt19937 generator(42);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);
    const auto valToSet = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, valToSet);
    reference.set(rngBegin, rngEnd, valToSet);
  }

  for (size_t i : iota(0, 50)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);
    const auto treeRes = tree.rangeGet(rngBegin, rngEnd);
    const auto refRes = reference.rangeGet(rngBegin, rngEnd);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(DynamicSumSegmentTree, IntRangeGivesInt64SumFuzzTest) {
  std::mt19937 gen(37);

  for (size_t i : iota(0, 20)) {
    auto tree = DynamicSumSegmentTree<int, int, int64_t>(0, 1 << 29, 0);

    const int rng1Start = std::uniform_int_distribution(0, (1 << 28) - 1)(gen);
    const int rng1Length = std::uniform_int_distribution(0, (1 << 27) - 1)(gen);
    const int rng2Length = std::uniform_int_distribution(0, (1 << 27) - 1)(gen);

    const auto setVal1 = std::uniform_int_distribution(0, (1 << 30) - 1)(gen);
    const auto setVal2 = std::uniform_int_distribution(0, (1 << 30) - 1)(gen);

    tree.set(rng1Start, rng1Start + rng1Length, setVal1);
    tree.set(rng1Start + rng1Length, rng1Start + rng1Length + rng2Length,
             setVal2);

    const auto expectedSum = static_cast<int64_t>(setVal1) * rng1Length +
                             static_cast<int64_t>(setVal2) * rng2Length;

    const auto rangeGetRes =
        tree.rangeGet(rng1Start, rng1Start + rng1Length + rng2Length);

    EXPECT_EQ(rangeGetRes, expectedSum);
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
