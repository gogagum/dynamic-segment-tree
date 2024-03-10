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

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-*)

TEST(DSTCopyAndMoveCount, ConstructCopying) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 42, copyAndMoveCounter);

  EXPECT_EQ(stats->getMoveCount(), 0);
  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyAndMoveCount, ConstructAndMoving) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 42, std::move(copyAndMoveCounter));

  EXPECT_EQ(stats->getMoveCount(), 1);
  EXPECT_EQ(stats->getCopyCount(), 0);
}

TEST(DSTCopyAndMoveCount, CopyTree) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  const auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 42, copyAndMoveCounter);

  [[maybe_unused]] const auto copy = segTree;

  EXPECT_EQ(stats->getMoveCount(), 0);
  EXPECT_EQ(stats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, ConstructAndSet) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 42, std::move(copyAndMoveCounter));

  auto [setStats, setCopyAndMoveCounter] = CopyAndMoveCounter::init();

  {
    segTree.set(21, 42, std::move(setCopyAndMoveCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }
}

TEST(DSTCopyAndMoveCount, EmptyRangeSetOperation) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

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

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(24, 32, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }

  EXPECT_EQ(initStats->getMoveCount(), 2);
  EXPECT_EQ(initStats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, SetToAlignedQuarterRangeInTheBeginning) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(0, 8, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 0);
  }

  EXPECT_EQ(initStats->getMoveCount(), 2);
  EXPECT_EQ(initStats->getCopyCount(), 2);
}

TEST(DSTCopyAndMoveCount, SetToNonAlignedQuarterRange) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(19, 27, std::move(setCounter));

    EXPECT_EQ(setStats->getMoveCount(), 1);
    EXPECT_EQ(setStats->getCopyCount(), 3);
  }

  EXPECT_EQ(initStats->getMoveCount(), 8);
  EXPECT_EQ(initStats->getCopyCount(), 8);
}

TEST(DSTCopyAndMoveCount, SetOneValueInSmallTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyAndMoveCounter>(
          0, 16, initCounter);

  initStats->reset();
  {
    auto [singleSetStats, singleSetCounter] = CopyAndMoveCounter::init();

    segTree.set(4, 5, std::move(singleSetCounter));
    EXPECT_TRUE(singleSetStats->getCopyCount() <= 0);
    EXPECT_TRUE(singleSetStats->getMoveCount() <= 1);

    if (singleSetStats->getMoveCount() < 1) {
      infoStream() << "Move count is smaller than expected: "
                   << singleSetStats->getMoveCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 4);
  EXPECT_TRUE(initStats->getMoveCount() <= 4);

  if (initStats->getCopyCount() < 4) {
    infoStream() << "Init value copy count is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }

  if (initStats->getMoveCount() < 4) {
    infoStream() << "Init value move count is smaller than expected: "
                 << initStats->getMoveCount() << std::endl;
  }
}

TEST(DSTCopyAndMoveCount, SetSomeValuesInSmallTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 16, initCounter);

  EXPECT_EQ(initStats->getCopyCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(4, 10, std::move(setCounter));
    EXPECT_TRUE(setStats->getCopyCount() <= 1);
    EXPECT_TRUE(setStats->getMoveCount() <= 1);

    if (setStats->getCopyCount() < 1) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }

    if (setStats->getMoveCount() < 1) {
      infoStream() << "Move count is smaller than expected: "
                   << setStats->getMoveCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 4);
  EXPECT_TRUE(initStats->getMoveCount() <= 4);

  if (initStats->getCopyCount() < 4) {
    infoStream() << "Copy count for init value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }

  if (initStats->getCopyCount() < 4) {
    infoStream() << "Copy count for init value is smaller than expected: "
                 << initStats->getMoveCount() << std::endl;
  }
}

TEST(DSTCopyAndMoveCount, SetSomeValuesInBigTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 1024ul * 1024, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(1004, 2005, std::move(setCounter));
    EXPECT_TRUE(setStats->getCopyCount() <= 8);
    EXPECT_TRUE(setStats->getMoveCount() <= 1);

    if (setStats->getCopyCount() < 8) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }

    if (setStats->getMoveCount() < 1) {
      infoStream() << "Move count is smaller than expected: "
                   << setStats->getMoveCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 28);
  EXPECT_TRUE(initStats->getMoveCount() <= 28);

  if (initStats->getCopyCount() < 28) {
    infoStream() << "Copy count of initial value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }

  if (initStats->getMoveCount() < 28) {
    infoStream() << "Move count of initial value is smaller than expected: "
                 << initStats->getMoveCount() << std::endl;
  }
}

TEST(DSTCopyAndMoveCount, SetManyNumbersInBigTree) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 1024ul * 1024, initCounter);

  EXPECT_EQ(initStats->getCopyCount(), 1);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyAndMoveCounter::init();

    segTree.set(1004, 1024ul * 512, std::move(setCounter));
    EXPECT_TRUE(setStats->getCopyCount() <= 10);
    EXPECT_TRUE(setStats->getMoveCount() <= 1);

    if (setStats->getCopyCount() < 10) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }

    if (setStats->getMoveCount() < 1) {
      infoStream() << "MoveCount is smallr than expected: "
                   << setStats->getMoveCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 18);
  EXPECT_TRUE(initStats->getMoveCount() <= 18);

  if (initStats->getCopyCount() < 18) {
    infoStream() << "Copy count of initial value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }

  if (initStats->getMoveCount() < 18) {
    infoStream() << "Move count of initial value is smaller than expected: "
                 << initStats->getMoveCount() << std::endl;
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-*)
