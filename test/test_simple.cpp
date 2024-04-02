////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <array>
#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/seg_tree_reference_base.hpp"
#include "tools/generate_index_range.hpp"

using I_LL_SimpleDST = dst::DynamicSimpleGetSetSegmentTree<int, std::int64_t>;
constexpr auto kiLLValGetter = &I_LL_SimpleDST::get;
using std::bind_front;
using std::size_t;
using std::ranges::equal;
using std::views::iota;
using std::views::transform;
using GenerateIndRng = GenerateIndexRange<size_t>;

// NOLINTBEGIN(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)

TEST(SimpleGetSetDynamicSegmentTree, Construct) {
  auto tree = I_LL_SimpleDST(-5, 100, 77);
}

TEST(SimpleGetSetDynamicSegmentTree, RangeSet) {
  auto tree = I_LL_SimpleDST(0, 42, 77);
  tree.set(13, 17, 56);

  constexpr auto indicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto expectedValues = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto values = indicies | transform(bind_front(kiLLValGetter, &tree));

  EXPECT_TRUE(equal(expectedValues, values));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndCopy) {
  auto tree = I_LL_SimpleDST(0, 42, 77);

  tree.set(13, 17, 56);

  auto copy = std::as_const(tree);

  constexpr auto indicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto expectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = indicies | transform(bind_front(kiLLValGetter, &tree));
  const auto treeVals = indicies | transform(bind_front(kiLLValGetter, &copy));

  EXPECT_TRUE(equal(expectedVals, copyVals));
  EXPECT_TRUE(equal(expectedVals, treeVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndCopyAssign) {
  auto tree = I_LL_SimpleDST(0, 42, 77);
  auto copy = I_LL_SimpleDST(5, 13, 67);

  tree.set(13, 17, 56);

  copy = std::as_const(tree);

  constexpr auto indicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto expectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = indicies | transform(bind_front(kiLLValGetter, &tree));
  const auto treeVals = indicies | transform(bind_front(kiLLValGetter, &copy));

  EXPECT_TRUE(equal(expectedVals, copyVals));
  EXPECT_TRUE(equal(expectedVals, treeVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndMove) {
  auto tree = I_LL_SimpleDST(0, 42, 77);

  tree.set(13, 17, 56);

  auto moved = std::move(tree);

  constexpr auto indicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto expectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals =
      indicies | transform(bind_front(kiLLValGetter, &moved));

  EXPECT_TRUE(equal(expectedVals, movedVals));
}

TEST(SimpleGetSetDynamicSegmentTree, SetAndMoveAssign) {
  auto tree = I_LL_SimpleDST(0, 42, 77);
  auto moved = I_LL_SimpleDST(5, 13, 67);

  tree.set(13, 17, 56);

  moved = std::move(tree);

  constexpr auto indicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto expectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals =
      indicies | transform(bind_front(kiLLValGetter, &moved));

  EXPECT_TRUE(equal(expectedVals, movedVals));
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGet) {
  using DST = dst::DynamicSimpleGetSetSegmentTree<size_t, int>;
  using Ref = SegTreeReferenceBase<size_t, int>;
  constexpr auto treeEnd = size_t{1000};
  auto tree = DST(0, treeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, treeEnd, 0);

  std::mt19937 generator(42);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
    reference.set(rngBegin, rngEnd, setVal);

    constexpr auto ids = iota(size_t{0}, treeEnd);
    const auto treeVals = ids | transform(bind_front(&DST::get, &tree));
    const auto refVals = ids | transform(bind_front(&Ref::get, &reference));
    EXPECT_TRUE(equal(treeVals, refVals));
  }
}

TEST(SimpleGetSetDynamicSegmentTree, FuzzTestSetGetMixed) {
  constexpr auto treeEnd = size_t{1000};
  auto tree = dst::DynamicSimpleGetSetSegmentTree<size_t, int>(0, treeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, treeEnd, 0);

  std::mt19937 generator(42);

  for (size_t i : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, treeEnd)(generator);

    if (std::bernoulli_distribution()(generator)) {
      const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, setVal);
      reference.set(rngBegin, rngEnd, setVal);
    } else {
      const auto idx = std::uniform_int_distribution<size_t>(0, 999)(generator);
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
}

// NOLINTEND(cppcoreguidelines-*, cert-*, readability-magic-numbers,
// cert-err58-cpp)
