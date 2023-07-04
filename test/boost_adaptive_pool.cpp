////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <boost/container/adaptive_pool.hpp>
#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>
#include <random>

#include "reference/seg_tree_reference_base.hpp"

using dst::DynamicSimpleGetSetSegmentTree;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(BoostAdaptivePoolSimpleTree, Construct) {
  auto tree =
      DynamicSimpleGetSetSegmentTree<int, int,
                                     boost::container::adaptive_pool<int>>(
          0, 100, 0);
}

TEST(BoostAdaptivePoolSimpleTree, RangeSet) {
  auto tree =
      DynamicSimpleGetSetSegmentTree<int, long long,
                                     boost::container::adaptive_pool<int>>(
          0, 42, 77);

  tree.set(13, 17, 56);

  EXPECT_EQ(tree.get(13), 56);
  EXPECT_EQ(tree.get(15), 56);
  EXPECT_EQ(tree.get(16), 56);
  EXPECT_EQ(tree.get(17), 77);
  EXPECT_EQ(tree.get(8), 77);
  EXPECT_EQ(tree.get(37), 77);
}

TEST(BoostAdaptivePoolSimpleTree, FuzzTestSetGet) {
  auto tree =
      DynamicSimpleGetSetSegmentTree<std::size_t, int,
                                     boost::container::adaptive_pool<int>>(
          0, 1000, 0);
  auto reference = SegTreeReferenceBase<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(42);

  for (std::size_t i = 0; i < 100; ++i) {
    const std::size_t rngStart = generator() % 500;           // [0..500)
    const std::size_t rngLen = generator() % 500;             // [0..500)
    const int setVal = static_cast<int>(generator()) % 1000;  // [0..100)
    tree.set(rngStart, rngStart + rngLen, setVal);
    reference.set(rngStart, rngStart + rngLen, setVal);
  }

  for (std::size_t i = 0; i < 50; ++i) {
    const std::size_t idx = generator() % 1000;  // [0..1000)
    auto treeRes = tree.get(idx);
    auto refRes = reference.get(idx);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(BoostAdaptivePoolSimpleTree, FuzzTestSetGetMixed) {
  auto tree =
      DynamicSimpleGetSetSegmentTree<std::size_t, int,
                                     boost::container::adaptive_pool<int>>(
          0, 1000, 0);
  auto reference = SegTreeReferenceBase<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(42);

  for (std::size_t i = 0; i < 100; ++i) {
    const std::size_t rngStart = generator() % 500;  // [0..500)
    const std::size_t rngLen = generator() % 500;    // [0..500)

    if (const int opType = static_cast<int>(generator()) % 2; opType == 0) {
      const int setVal = static_cast<int>(generator()) % 1000;  // [0..1000)
      tree.set(rngStart, rngStart + rngLen, setVal);
      reference.set(rngStart, rngStart + rngLen, setVal);
    } else {
      const std::size_t idx = generator() % 1000;  // [0..1000)
      auto treeRes = tree.get(idx);
      auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)