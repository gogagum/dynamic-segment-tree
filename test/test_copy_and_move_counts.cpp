////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>

#include "counters/copy_and_move_counter.hpp"
#include "tools/info_stream.hpp"

using std::size_t;

// NOLINTBEGIN(cppcoreguidelines-owning-memory, cert-err58-*)

TEST(DSTCopyAndMoveCount, ConstructCopying) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{42};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, copyAndMoveCounter);

  EXPECT_EQ(stats->getMoveCount(), 0);
  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyAndMoveCount, ConstructAndMoving) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{42};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(copyAndMoveCounter));

  EXPECT_EQ(stats->getMoveCount(), 1);
  EXPECT_EQ(stats->getCopyCount(), 0);
}

TEST(DSTCopyAndMoveCount, CopyTree) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  const auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 42, copyAndMoveCounter);

  const auto copy = segTree;  // NOLINT

  EXPECT_EQ(stats->getMoveCount(), 0);
  EXPECT_EQ(stats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, ConstructAndSet) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{42};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(copyAndMoveCounter));

  auto [setStats, setCopyAndMoveCounter] = CopyAndMoveCounter::init();

  {
    constexpr auto kSetOpBegin = size_t{21};
    segTree.set(kSetOpBegin, kTreeEnd, std::move(setCopyAndMoveCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }
}

TEST(DSTCopyAndMoveCount, EmptyRangeSetOperation) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{32};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(initCounter));

  initStats->reset();
  {
    auto [emptySetStats, emptySetCounter] = CopyAndMoveCounter::init();

    segTree.set(4, 4, emptySetCounter);
    EXPECT_EQ(emptySetStats->getCopyCount(), 0);
    EXPECT_EQ(emptySetStats->getMoveCount(), 0);
  }

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 0);
}

TEST(DSTCopyAndMoveCount, SetToAlignedQuarterRangeInTheEnd) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{32};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpBegin = 24;
    segTree.set(kSetOpBegin, kTreeEnd, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }

  EXPECT_EQ(initStats->getMoveCount(), 2);
  EXPECT_EQ(initStats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, SetToAlignedQuarterRangeInTheBeginning) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{32};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpEnd = size_t{8};
    segTree.set(0, kSetOpEnd, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }

  EXPECT_EQ(initStats->getMoveCount(), 2);
  EXPECT_EQ(initStats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, SetToNonAlignedQuarterRange) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{32};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpBegin = size_t{19};
    constexpr auto kSetOpEnd = size_t{27};
    segTree.set(kSetOpBegin, kSetOpEnd, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 3);
  }

  EXPECT_EQ(initStats->getMoveCount(), 8);
  EXPECT_EQ(initStats->getCopyCount(), 8);
}

TEST(DSTCopyAndMoveCount, SetOneValueInSmallTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{16};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [singleSetStats, singleSetCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpEnd = size_t{5};
    segTree.set(4, kSetOpEnd, std::move(singleSetCounter));
    EXPECT_LE(singleSetStats->getCopyCount(), 0);
    EXPECT_LE(singleSetStats->getMoveCount(), 1);

    if (singleSetStats->getMoveCount() < 1) {
      infoStreamPrint("Move count is smaller than expected: {}\n",
                      singleSetStats->getMoveCount());
    }
  }

  EXPECT_LE(initStats->getCopyCount(), 4);
  EXPECT_LE(initStats->getMoveCount(), 4);

  if (initStats->getCopyCount() < 4) {
    infoStreamPrint("Init value copy count is smaller than expected: {}\n",
                    initStats->getCopyCount());
  }

  if (initStats->getMoveCount() < 4) {
    infoStreamPrint("Init value move count is smaller than expected: {}\n",
                    initStats->getMoveCount());
  }
}

TEST(DSTCopyAndMoveCount, SetSomeValuesInSmallTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{16};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, initCounter);

  EXPECT_EQ(initStats->getCopyCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpEnd = size_t{10};
    segTree.set(4, kSetOpEnd, std::move(setCounter));
    EXPECT_LE(setStats->getCopyCount(), 1);
    EXPECT_LE(setStats->getMoveCount(), 1);

    if (setStats->getCopyCount() < 1) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }

    if (setStats->getMoveCount() < 1) {
      infoStreamPrint("Move count is smaller than expected: {}\n",
                      setStats->getMoveCount());
    }
  }

  EXPECT_LE(initStats->getCopyCount(), 4);
  EXPECT_LE(initStats->getMoveCount(), 4);

  if (initStats->getCopyCount() < 4) {
    infoStreamPrint("Copy count for init value is smaller than expected: {}\n",
                    initStats->getCopyCount());
  }

  if (initStats->getCopyCount() < 4) {
    infoStreamPrint("Copy count for init value is smaller than expected: {}\n",
                    initStats->getMoveCount());
  }
}

TEST(DSTCopyAndMoveCount, SetSomeValuesInBigTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpBegin = size_t{1004};
    constexpr auto kSetOpEnd = size_t{2005};

    segTree.set(kSetOpBegin, kSetOpEnd, std::move(setCounter));

    constexpr auto kExpectedCopyCount = size_t{8};
    EXPECT_LE(setStats->getCopyCount(), kExpectedCopyCount);
    EXPECT_LE(setStats->getMoveCount(), 1);

    if (setStats->getCopyCount() < kExpectedCopyCount) {
      infoStreamPrint("Copy count is smaller than expected: {}\n",
                      setStats->getCopyCount());
    }

    if (setStats->getMoveCount() < 1) {
      infoStreamPrint("Move count is smaller than expected: {}\n",
                      setStats->getMoveCount());
    }
  }

  constexpr auto kExpectedCopyCount = size_t{28};
  constexpr auto kExpectedMoveCount = size_t{28};

  EXPECT_LE(initStats->getCopyCount(), kExpectedCopyCount);
  EXPECT_LE(initStats->getMoveCount(), kExpectedMoveCount);

  if (initStats->getCopyCount() < kExpectedCopyCount) {
    infoStreamPrint(
        "Copy count of initial value is smaller than expected: {}\n",
        initStats->getCopyCount());
  }

  if (initStats->getMoveCount() < kExpectedMoveCount) {
    infoStreamPrint(
        "Move count of initial value is smaller than expected: {}\n",
        initStats->getMoveCount());
  }
}

TEST(DSTCopyAndMoveCount, SetManyNumbersInBigTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  constexpr auto kTreeEnd = size_t{1024ul * 1024};
  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, kTreeEnd, initCounter);

  EXPECT_EQ(initStats->getCopyCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    constexpr auto kSetOpBegin = size_t{1004};
    constexpr auto kSetOpEnd = size_t{1024ul * 512};

    segTree.set(kSetOpBegin, kSetOpEnd, std::move(setCounter));

    constexpr auto kExpectedCopyCount = size_t{10};

    EXPECT_LE(setStats->getCopyCount(), kExpectedCopyCount);
    EXPECT_LE(setStats->getMoveCount(), 1);

    if (setStats->getCopyCount() < kExpectedCopyCount) {
      infoStreamPrint(
          "Copy count of initial value is smaller than expected: {}\n",
          initStats->getCopyCount());
    }

    if (setStats->getMoveCount() < 1) {
      infoStreamPrint(
          "Move count of initial value is smaller than expected: {}\n",
          initStats->getMoveCount());
    }
  }

  constexpr auto kExpectedCopyCount = size_t{18};
  constexpr auto kExpectedMoveCount = size_t{18};

  EXPECT_LE(initStats->getCopyCount(), kExpectedCopyCount);
  EXPECT_LE(initStats->getMoveCount(), kExpectedMoveCount);

  if (initStats->getCopyCount() < kExpectedCopyCount) {
    infoStreamPrint(
        "Copy count of initial value is smaller than expected: {}\n",
        initStats->getCopyCount());
  }

  if (initStats->getMoveCount() < kExpectedMoveCount) {
    infoStreamPrint(
        "Move count of initial value is smaller than expected: {}\n",
        initStats->getMoveCount());
  }
}

// NOLINTEND(cppcoreguidelines-owning-memory, cert-err58-*)
