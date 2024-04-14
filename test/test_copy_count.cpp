////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>

#include "counters/copy_only_copy_counter.hpp"
#include "tools/info_stream.hpp"

using std::size_t;

// NOLINTBEGIN(cppcoreguidelines-owning-memory, cert-err58-*)

TEST(DSTCopyCount, Construct) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{42};

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetOneValueInSmallTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{16};

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  constexpr auto kSetOpBegin = size_t{4};
  constexpr auto kSetOpEnd = size_t{5};

  segTree.set(kSetOpBegin, kSetOpEnd, copyCounter);

  constexpr auto kExpectedCopiesCnt = size_t{10};

  EXPECT_LE(stats->getCopyCount(), kExpectedCopiesCnt);

  if (stats->getCopyCount() < kExpectedCopiesCnt) {
    infoStreamPrint("Copy count is smaller than expected: {}",
                    stats->getCopyCount());
  }
}

TEST(DSTCopyCount, SetOneValueInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    constexpr auto kSetOpBegin = 1004;
    constexpr auto kSetOpEnd = 1005;
    segTree.set(kSetOpBegin, kSetOpEnd, setCounter);
    EXPECT_LE(setStats->getCopyCount(), 1);

    if (setStats->getCopyCount() < 1) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }
  }

  constexpr auto kExpectedCopiesCnt = size_t{40};
  EXPECT_LE(initStats->getCopyCount(), kExpectedCopiesCnt);
  if (initStats->getCopyCount() < kExpectedCopiesCnt) {
    infoStreamPrint(
        "Copy count of initial value is smaller than expected: {}\n",
        initStats->getCopyCount());
  }
}

TEST(DSTCopyCount, EmptyRangeSetOperation) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{16};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();
    segTree.set(4, 4, setCounter);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }

  EXPECT_EQ(initStats->getCopyCount(), 0);
}

TEST(DSTCopyCount, SetSomeValuesInSmallTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{16};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    constexpr auto kSetOpBegin = size_t{4};
    constexpr auto kSetOpEnd = size_t{10};
    segTree.set(kSetOpBegin, kSetOpEnd, setCounter);

    constexpr auto kExpectedCopiesCnt = size_t{2};
    EXPECT_LE(setStats->getCopyCount(), kExpectedCopiesCnt);

    if (setStats->getCopyCount() < kExpectedCopiesCnt) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }
  }

  constexpr auto kExpectedCopiesCnt = size_t{8};
  EXPECT_LE(initStats->getCopyCount(), kExpectedCopiesCnt);
  if (initStats->getCopyCount() < kExpectedCopiesCnt) {
    infoStreamPrint("Copy count is smaller than expected: {}\n",
                    initStats->getCopyCount());
  }
}

TEST(DSTCopyCount, EmptyRangeSetOperationOnBigTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, copyCounter);

  constexpr auto kSetOpBegin = 785;
  constexpr auto kSetOpEnd = 785;

  segTree.set(kSetOpBegin, kSetOpEnd, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetSomeValuesInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    constexpr auto kSetOpBegin = 1004;
    constexpr auto kSetOpEnd = 2005;
    segTree.set(kSetOpBegin, kSetOpEnd, setCounter);

    constexpr auto kExpectedCopiesCnt = size_t{9};
    EXPECT_LE(setStats->getCopyCount(), kExpectedCopiesCnt);
    if (setStats->getCopyCount() < kExpectedCopiesCnt) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }
  }

  constexpr auto kExpectedCopiesCnt = size_t{56};
  EXPECT_LE(initStats->getCopyCount(), kExpectedCopiesCnt);
  if (initStats->getCopyCount() < kExpectedCopiesCnt) {
    infoStreamPrint(
        "Copy count of initial value is smaller than expected: {}\n",
        initStats->getCopyCount());
  }
}

TEST(DSTCopyCount, SetManyNumbersInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    constexpr auto kSetOpBegin = size_t{1004};
    constexpr auto kSetOpEnd = size_t{1024ul * 512};
    segTree.set(kSetOpBegin, kSetOpEnd, setCounter);

    constexpr auto kExpectedCopiesCnt = size_t{11};
    EXPECT_LE(setStats->getCopyCount(), kExpectedCopiesCnt);

    if (setStats->getCopyCount() < kExpectedCopiesCnt) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }
  }

  constexpr auto kExpectedCopiesCnt = size_t{36};
  EXPECT_LE(initStats->getCopyCount(), kExpectedCopiesCnt);
  if (initStats->getCopyCount() < kExpectedCopiesCnt) {
    infoStreamPrint("Copy count of initial value is smaller than expected: {}",
                    initStats->getCopyCount());
  }
}

// NOLINTEND(cppcoreguidelines-owning-memory, cert-err58-*)
