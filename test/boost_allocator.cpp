////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <boost/container/allocator.hpp>
#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/seg_tree_reference_base.hpp"
#include "tools/generate_index_range.hpp"

using std::size_t;
using std::views::iota;
using GenerateIndRng = GenerateIndexRange<size_t>;

template <class KeyT, class ValueT>
using SimpleBoostAllocDST =
    dst::DynamicSimpleGetSetSegmentTree<KeyT, ValueT,
                                        boost::container::allocator<ValueT>>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(BoostAllocSimpleTree, Construct) {
  auto tree = SimpleBoostAllocDST<int, int>(0, 100, 0);
}

TEST(BoostAllocSimpleTree, RangeSet) {
  auto tree = SimpleBoostAllocDST<int, long long>(0, 42, 77);

  tree.set(13, 17, 56);

  EXPECT_EQ(tree.get(13), 56);
  EXPECT_EQ(tree.get(15), 56);
  EXPECT_EQ(tree.get(16), 56);
  EXPECT_EQ(tree.get(17), 77);
  EXPECT_EQ(tree.get(8), 77);
  EXPECT_EQ(tree.get(37), 77);
}

TEST(BoostAllocSimpleTree, FuzzTestSetGet) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = SimpleBoostAllocDST<size_t, int>(0, treeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, treeEnd, 0);

  std::mt19937 generator(42);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);
    const auto valueToSet = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, valueToSet);
    reference.set(rngBegin, rngEnd, valueToSet);
  }

  for (size_t i : iota(0, 50)) {
    const auto idx = std::uniform_int_distribution<size_t>(0, 999)(generator);
    auto treeRes = tree.get(idx);
    auto refRes = reference.get(idx);
    EXPECT_EQ(treeRes, refRes);
  }
}

TEST(BoostAllocSimpleTree, FuzzTestSetGetMixed) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = SimpleBoostAllocDST<size_t, int>(0, treeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, treeEnd, 0);

  std::mt19937 generator(42);

  for (size_t i : iota(0, 100)) {
    if (std::bernoulli_distribution()(generator)) {
      const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);
      const auto valueToSet = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, valueToSet);
      reference.set(rngBegin, rngEnd, valueToSet);
    } else {
      const auto idx = std::uniform_int_distribution<size_t>(0, 999)(generator);
      auto treeRes = tree.get(idx);
      auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
