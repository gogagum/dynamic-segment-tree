////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/partial/dynamic_max_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/max_seg_tree_reference.hpp"

namespace rng = std::ranges;
using dst::DynamicMaxSegmentTree;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(DynamicMaxSegmentTree, Construct) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
}

TEST(DynamicMaxSegmentTree, SimpleRangeGet) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
  EXPECT_EQ(tree.rangeGet(5, 17), 34);
}

TEST(DynamicMaxSegmentTree, RangeGetAfterUpdate) {
  auto tree = DynamicMaxSegmentTree<int, int, int, std::plus<int>>(0, 42, 34);
  tree.update(12, 22, 4);
  EXPECT_EQ(tree.rangeGet(5, 17), 34 + 4);
  EXPECT_EQ(tree.rangeGet(12, 18), 34 + 4);
}

TEST(DynamicMaxSegmentTree, RangeGetAfterSet) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 34);
  tree.set(12, 22, 4);
  EXPECT_EQ(tree.rangeGet(13, 20), 4);
  EXPECT_EQ(tree.rangeGet(2, 35), 34);
}

TEST(DynamicMaxSegmentTree, UpdateAndSet) {
  auto tree = DynamicMaxSegmentTree<int, int, int, std::plus<int>>(0, 42, 34);
  tree.update(12, 22, 4);
  tree.set(17, 27, 66);
  EXPECT_EQ(tree.rangeGet(5, 17), 34 + 4);
  EXPECT_EQ(tree.rangeGet(12, 18), 66);
}

TEST(DynamicMaxSegmentTree, LadderUpRight) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
  tree.set(41, 42, 10000000);
  tree.set(40, 41, 1000000);
  tree.set(39, 40, 100000);
  tree.set(38, 39, 10000);
  tree.set(37, 38, 1000);
  tree.set(36, 37, 100);
  tree.set(35, 36, 10);
  tree.set(34, 35, 1);

  EXPECT_EQ(tree.rangeGet(0, 42), 10000000);
  EXPECT_EQ(tree.rangeGet(36, 42), 10000000);
  EXPECT_EQ(tree.rangeGet(0, 36), 10);
  EXPECT_EQ(tree.rangeGet(39, 42), 10000000);
  EXPECT_EQ(tree.rangeGet(0, 39), 10000);
}

TEST(DynamicMaxSegmentTree, LadderUpLeft) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
  tree.set(0, 1, 10000000);
  tree.set(1, 2, 1000000);
  tree.set(2, 3, 100000);
  tree.set(3, 4, 10000);
  tree.set(4, 5, 1000);
  tree.set(5, 6, 100);
  tree.set(6, 7, 10);
  tree.set(7, 7, 1);

  EXPECT_EQ(tree.rangeGet(0, 42), 10000000);
  EXPECT_EQ(tree.rangeGet(0, 6), 10000000);
  EXPECT_EQ(tree.rangeGet(6, 42), 10);
  EXPECT_EQ(tree.rangeGet(0, 3), 10000000);
  EXPECT_EQ(tree.rangeGet(3, 42), 10000);
}

TEST(DynamicMaxSegmentTree, LadderDownRight) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
  tree.set(41, 42, -10000000);
  tree.set(40, 41, -1000000);
  tree.set(39, 40, -100000);
  tree.set(38, 39, -10000);
  tree.set(37, 38, -1000);
  tree.set(36, 37, -100);
  tree.set(35, 36, -10);
  tree.set(34, 35, -1);

  EXPECT_EQ(tree.rangeGet(0, 42), 0);
  EXPECT_EQ(tree.rangeGet(36, 42), -100);
  EXPECT_EQ(tree.rangeGet(0, 36), 0);
  EXPECT_EQ(tree.rangeGet(39, 42), -100000);
  EXPECT_EQ(tree.rangeGet(0, 39), 0);
}

TEST(DynamicMaxSegmentTree, LadderDownLeft) {
  auto tree = DynamicMaxSegmentTree<int, int>(0, 42, 0);
  tree.set(0, 1, -10000000);
  tree.set(1, 2, -1000000);
  tree.set(2, 3, -100000);
  tree.set(3, 4, -10000);
  tree.set(4, 5, -1000);
  tree.set(5, 6, -100);
  tree.set(6, 7, -10);
  tree.set(7, 7, -1);

  EXPECT_EQ(tree.rangeGet(0, 42), 0);
  EXPECT_EQ(tree.rangeGet(0, 6), -100);
  EXPECT_EQ(tree.rangeGet(6, 42), 0);
  EXPECT_EQ(tree.rangeGet(0, 3), -100000);
  EXPECT_EQ(tree.rangeGet(3, 42), 0);
}

TEST(DynamicMaxSegmentTree, FuzzTestSetUpdateGet) {
  auto tree =
      DynamicMaxSegmentTree<std::size_t, int, int, std::plus<int>>(0, 1000, 0);
  auto reference = MaxSegTreeReference<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(42);

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;           // [0..500)
    const std::size_t rngLen = generator() % 500;             // [0..500)
    const int setVal = static_cast<int>(generator() % 1000);  // [0..1000)

    tree.set(rngStart, rngStart + rngLen, setVal);
    reference.set(rngStart, rngStart + rngLen, setVal);
  }

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;                // [0..500)
    const std::size_t rngLen = generator() % 500;                  // [0..500)
    const int updateValue = static_cast<int>(generator() % 1000);  // [0..1000)

    tree.update(rngStart, rngStart + rngLen, updateValue);
    reference.update(rngStart, rngStart + rngLen, std::plus<int>(),
                     updateValue);
  }

  for (std::size_t i : rng::iota_view(0, 50)) {
    const std::size_t idx = generator() % 100;  // [0..1000)
    auto treeRes = tree.get(idx);
    auto refRes = reference.get(idx);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(DynamicMaxSegmentTree, FuzzTestMixedSetUpdateGet) {
  auto tree =
      DynamicMaxSegmentTree<std::size_t, int, int, std::plus<int>>(0, 1000, 0);
  auto reference = MaxSegTreeReference<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(54);

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;                 // [0..500)
    const std::size_t rngLen = generator() % 500;                   // [0..500)
    const int operationChoise = static_cast<int>(generator()) % 2;  // [0..2)

    if (operationChoise) {
      const int setVal = static_cast<int>(generator()) % 1000;  // [0..1000)
      tree.set(rngStart, rngStart + rngLen, setVal);
      reference.set(rngStart, rngStart + rngLen, setVal);
    } else {
      const int updateValue =
          static_cast<int>(generator()) % 1000;  // [0..1000)
      tree.update(rngStart, rngStart + rngLen, updateValue);
      reference.update(rngStart, rngStart + rngLen, std::plus<int>(),
                       updateValue);
    }
  }

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t idx = generator() % 100;  // [0..1000)
    auto treeRes = tree.get(idx);
    auto refRes = reference.get(idx);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(DynamicMaxSegmentTree, FuzzTestSetRangeGet) {
  auto tree = DynamicMaxSegmentTree<std::size_t, int>(0, 1000, 0);
  auto reference = MaxSegTreeReference<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(42);

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;           // [0..500)
    const std::size_t rngLen = generator() % 500;             // [0..500)
    const int setVal = static_cast<int>(generator()) % 1000;  // [0..100)
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

TEST(DynamicMaxSegmentTree, FuzzTestMixedSetUpdateRangeGet) {
  auto tree =
      DynamicMaxSegmentTree<std::size_t, int, int, std::plus<int>>(0, 1000, 0);
  auto reference = MaxSegTreeReference<std::size_t, int>(0, 1000, 0);

  std::mt19937 generator(54);

  for (std::size_t i : rng::iota_view(0, 100)) {
    const std::size_t rngStart = generator() % 500;  // [0..500)
    const std::size_t rngLen = generator() % 500;    // [0..500)

    const int operationChoise = static_cast<int>(generator()) % 2;

    if (operationChoise) {
      const int setVal = static_cast<int>(generator()) % 1000;  // [0..1000)
      tree.set(rngStart, rngStart + rngLen, setVal);
      reference.set(rngStart, rngStart + rngLen, setVal);
    } else {
      const int updateValue =
          static_cast<int>(generator()) % 1000;  // [0..1000)
      tree.update(rngStart, rngStart + rngLen, updateValue);
      reference.update(rngStart, rngStart + rngLen, std::plus<int>(),
                       updateValue);
    }
  }

  for (std::size_t i : rng::iota_view(0, 50)) {
    const std::size_t rngStart = generator() % 500;  // [0..500)
    const std::size_t rngLen = generator() % 500;    // [0..500)
    const auto treeRes = tree.rangeGet(rngStart, rngStart + rngLen);
    const auto refRes = reference.rangeGet(rngStart, rngStart + rngLen);
    EXPECT_EQ(treeRes, refRes);
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
