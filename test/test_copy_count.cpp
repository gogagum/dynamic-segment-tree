////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>

#include "counters/copy_only_copy_counter.hpp"

auto& infoStream() {
  return std::cerr << "[          ] INFO: ";
}

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)

TEST(DSTCopyCount, Construct) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 42, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetOneValueInSmallTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 16, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 4, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 5, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 10);

  if (stats->getCopyCount() < 10) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetOneValueInBigTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 1024ul * 1024, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1000, 1000, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1004, 1005, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 42);

  if (stats->getCopyCount() < 42) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetSomeNumbersInSmallTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 16, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 4, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 10, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 11);

  if (stats->getCopyCount() < 11) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetSomeNumbersInBigTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 1024ul * 1024, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1000, 1000, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1004, 2005, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 66);

  if (stats->getCopyCount() < 66) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

TEST(DSTCopyCount, SetManyNumbersInBigTree) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyOnlyCopyCounter>(
      0, 1024ul * 1024, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1000, 1000, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1004, 1024ul * 512, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 48);

  if (stats->getCopyCount() < 48) {
    infoStream() << "Copy count is smaller than expected: "
                 << stats->getCopyCount() << std::endl;
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)
