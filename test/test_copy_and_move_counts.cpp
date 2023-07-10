////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>

#include "counters/copy_and_move_counter.hpp"

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)

TEST(DSTCopyAndMoveCount, ConstructCopying) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 42, copyAndMoveCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyAndMoveCount, ConstructAndMoving) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 42, std::move(copyAndMoveCounter));

  EXPECT_EQ(stats->getMoveCount(), 1);
}

TEST(DSTCopyAndMoveCount, ConstructAndSet) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 42, std::move(copyAndMoveCounter));

  auto [setStats, setCopyAndMoveCounter] = CopyAndMoveCounter::init();

  segTree.set(21, 42, std::move(setCopyAndMoveCounter));

  EXPECT_EQ(setStats->getMoveCount(), 1);
  EXPECT_EQ(setStats->getCopyCount(), 0);
}

TEST(DSTCopyAndMoveCount, SetToAlignedQuaterRange) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  auto [setStats, setCounter] = CopyAndMoveCounter::init();

  segTree.set(24, 32, std::move(setCounter));

  EXPECT_EQ(setStats->getMoveCount(), 1);
  EXPECT_EQ(setStats->getCopyCount(), 0);

  EXPECT_EQ(initStats->getMoveCount(), 1);
  EXPECT_EQ(initStats->getCopyCount(), 4);
}

TEST(DSTCopyAndMoveCount, SetToNonAlignedQuaterRange) {
  auto [initStats, initCounter] = CopyAndMoveCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyAndMoveCounter>(
          0, 32, std::move(initCounter));

  EXPECT_EQ(initStats->getCopyCount(), 0);
  EXPECT_EQ(initStats->getMoveCount(), 1);

  auto [setStats, setCounter] = CopyAndMoveCounter::init();

  segTree.set(20, 28, std::move(setCounter));

  EXPECT_EQ(setStats->getMoveCount(), 1);
  EXPECT_EQ(setStats->getCopyCount(), 1);

  EXPECT_EQ(initStats->getMoveCount(), 1);
  EXPECT_EQ(initStats->getCopyCount(), 8);
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)
