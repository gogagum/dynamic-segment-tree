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

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)

TEST(DSTCopyCount, Construct) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 42, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetOneValueInSmallTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 16, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 5, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 10);

  if (stats->getCopyCount() < 10) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetOneValueInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 1024ul * 1024, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    segTree.set(1004, 1005, setCounter);
    EXPECT_TRUE(setStats->getCopyCount() <= 1);

    if (setStats->getCopyCount() < 1) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 40);
  if (initStats->getCopyCount() < 40) {
    infoStream() << "Copy count of initial value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, EmptyRangeSetOperation) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 16, initCounter);

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

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 16, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    segTree.set(4, 10, setCounter);
    EXPECT_TRUE(setStats->getCopyCount() <= 2);

    if (setStats->getCopyCount() < 2) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 8);

  if (initStats->getCopyCount() < 8) {
    infoStream() << "Copy count is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, EmptyRangeSetOperationOnBigTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 1024ul * 1024, copyCounter);

  segTree.set(785, 785, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetSomeValuesInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 1024ul * 1024, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    segTree.set(1004, 2005, setCounter);
    EXPECT_TRUE(setStats->getCopyCount() <= 9);

    if (setStats->getCopyCount() < 9) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 56);
  if (initStats->getCopyCount() < 56) {
    infoStream() << "Copy count of initial value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetManyNumbersInBigTree) {
  auto [initStats, initCounter] = CopyOnlyCopyCounter::init();

  auto segTree =
      dst::DynamicSimpleGetSetSegmentTree<size_t, CopyOnlyCopyCounter>(
          0, 1024ul * 1024, initCounter);

  initStats->reset();
  {
    auto [setStats, setCounter] = CopyOnlyCopyCounter::init();

    segTree.set(1004, 1024ul * 512, setCounter);
    EXPECT_TRUE(setStats->getCopyCount() <= 11);

    if (setStats->getCopyCount() < 11) {
      infoStream() << "Copy count is smaller than expected: "
                   << setStats->getCopyCount() << std::endl;
    }
  }

  EXPECT_TRUE(initStats->getCopyCount() <= 36);
  if (initStats->getCopyCount() < 36) {
    infoStream() << "Copy count of initial value is smaller than expected: "
                 << initStats->getCopyCount() << std::endl;
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)
