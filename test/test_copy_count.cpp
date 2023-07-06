#include <gtest/gtest.h>

#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>

#include "copy_counter/copy_counter.hpp"

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)

TEST(DSTCopyCount, Construct) {
  auto [stats, copyCounter] = CopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyCounter>(
      0, 42, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(DSTCopyCount, SetOneValueInSmallTree) {
  auto [stats, copyCounter] = CopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyCounter>(
      0, 16, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 4, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 5, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 10);
}

TEST(DSTCopyCount, SetOneValueInBigTree) {
  auto [stats, copyCounter] = CopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyCounter>(
      0, 1024ul * 1024, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1000, 1000, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1004, 1005, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 42);
}

TEST(DSTCopyCount, SetSomeNumbersInSmallTree) {
    auto [stats, copyCounter] = CopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyCounter>(
      0, 16, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 4, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(4, 10, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 11);
}

TEST(DSTCopyCount, SetSomeNumbersInBigTree) {
  auto [stats, copyCounter] = CopyCounter::init();

  auto segTree = dst::DynamicSimpleGetSetSegmentTree<std::size_t, CopyCounter>(
      0, 1024ul * 1024, copyCounter);

  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1000, 1000, copyCounter);
  EXPECT_EQ(stats->getCopyCount(), 1);

  segTree.set(1004, 2005, copyCounter);
  EXPECT_TRUE(stats->getCopyCount() <= 66);
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-magic-numbers)
