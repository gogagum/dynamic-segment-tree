////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <array>
#include <boost/container/adaptive_pool.hpp>
#include <boost/container/allocator.hpp>
#include <dst/disable_operations.hpp>
#include <dst/partial/dynamic_simple_get_set_segment_tree.hpp>
#include <random>
#include <ranges>

#include "reference/seg_tree_reference_base.hpp"
#include "tools/generate_index_range.hpp"

using std::bind_front;
using std::size_t;
using std::ranges::equal;
using std::views::iota;
using std::views::transform;
using GenerateIndRng = GenerateIndexRange<std::size_t>;

template <class AllocatorType>
class AllocatorsCommonTest : public testing::Test {
 public:
  template <typename KeyT, class ValueT>
  using DST = dst::DynamicSimpleGetSetSegmentTree<KeyT, ValueT, AllocatorType>;
};

using MyTypes =
    ::testing::Types<std::allocator<int>, boost::container::adaptive_pool<int>,
                     boost::container::allocator<int>>;

TYPED_TEST_SUITE(AllocatorsCommonTest, MyTypes);

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp, cert-msc32-c)

TYPED_TEST(AllocatorsCommonTest, Construct) {
  constexpr auto kTreeBegin = -11;
  constexpr auto kTreeEnd = 17;
  auto tree =
      typename TestFixture::template DST<int, int>(kTreeBegin, kTreeEnd, 2);
}

TYPED_TEST(AllocatorsCommonTest, RangeSet) {
  using DST = TestFixture::template DST<int, int>;

  constexpr auto kTreeBegin = -17;
  constexpr auto kTreeEnd = 93;
  constexpr auto kFillValue = 77;
  auto tree = DST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto values = kIndicies | transform(bind_front(&DST::get, &tree));

  EXPECT_TRUE(equal(kExpectedVals, values));
}

TYPED_TEST(AllocatorsCommonTest, SetAndCopy) {
  using DST = TestFixture::template DST<size_t, int>;

  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = DST(0, kTreeEnd, kFillValue);

  tree.set(13, 17, 56);

  auto copy = std::as_const(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = kIndicies | transform(bind_front(&DST::get, &tree));
  const auto treeVals = kIndicies | transform(bind_front(&DST::get, &copy));

  EXPECT_TRUE(equal(kExpectedVals, copyVals));
  EXPECT_TRUE(equal(kExpectedVals, treeVals));
}

TYPED_TEST(AllocatorsCommonTest, FuzzSetOnBothAndCopy) {
  using DST = TestFixture::template DST<size_t, int>;

  constexpr auto kTreeBegin = size_t{13};
  constexpr auto kTreeEnd = size_t{37};
  constexpr auto kFillValue = 19;
  auto tree = DST(kTreeBegin, kTreeEnd, kFillValue);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for (auto i : iota(0, 15)) {
    const auto [rngBegin, rngEnd] =
        GenerateIndRng(kTreeBegin, kTreeEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
  }

  constexpr auto kDestBegin = size_t{11};
  constexpr auto kDestEnd = size_t{43};
  constexpr auto kDestFillValue = 17;

  auto dest = DST(kDestBegin, kDestEnd, kFillValue);

  for (auto i : iota(0, 15)) {
    const auto [rngBegin, rngEnd] =
        GenerateIndRng(kDestBegin, kDestEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    dest.set(rngBegin, rngEnd, setVal);
  }

  dest = std::as_const(tree);

  constexpr auto ids = iota(kTreeBegin, kTreeEnd);
  auto treeVals = ids | transform(bind_front(&DST::get, &tree));
  auto destVals = ids | transform(bind_front(&DST::get, &dest));
  EXPECT_TRUE(equal(treeVals, destVals));
}

TYPED_TEST(AllocatorsCommonTest, SetAndCopyAssign) {
  using DST = TestFixture::template DST<size_t, int>;

  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = DST(0, kTreeEnd, kFillValue);

  auto copy = DST(5, 13, 67);

  tree.set(13, 17, 56);

  copy = std::as_const(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto copyVals = kIndicies | transform(bind_front(&DST::get, &tree));
  const auto treeVals = kIndicies | transform(bind_front(&DST::get, &copy));

  EXPECT_TRUE(equal(kExpectedVals, copyVals));
  EXPECT_TRUE(equal(kExpectedVals, treeVals));
}

TYPED_TEST(AllocatorsCommonTest, SetAndMove) {
  using DST = TestFixture::template DST<size_t, int>;

  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = DST(0, kTreeEnd, kFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  auto moved = std::move(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals = kIndicies | transform(bind_front(&DST::get, &moved));

  EXPECT_TRUE(equal(kExpectedVals, movedVals));
}

TYPED_TEST(AllocatorsCommonTest, SetAndMoveAssign) {
  using DST = TestFixture::template DST<size_t, int>;

  constexpr auto kTreeEnd = 42;
  constexpr auto kFillValue = uint64_t{77};
  auto tree = DST(0, kTreeEnd, kFillValue);

  constexpr auto kDestTreeBegin = 5;
  constexpr auto kDestTreeEnd = 13;
  constexpr auto kDestTreeFillValue = 67;
  auto moved = DST(kDestTreeBegin, kDestTreeEnd, kDestTreeFillValue);

  constexpr auto kSetOpBegin = 13;
  constexpr auto kSetOpEnd = 17;
  constexpr auto kSetValue = uint64_t{56};
  tree.set(kSetOpBegin, kSetOpEnd, kSetValue);

  moved = std::move(tree);

  constexpr auto kIndicies = std::array<int, 6>{13, 15, 16, 17, 8, 37};
  constexpr auto kExpectedVals = std::array<int, 6>{56, 56, 56, 77, 77, 77};
  const auto movedVals = kIndicies | transform(bind_front(&DST::get, &moved));

  EXPECT_TRUE(equal(kExpectedVals, movedVals));
}

TYPED_TEST(AllocatorsCommonTest, FuzzTestSetGet) {
  using DST = TestFixture::template DST<size_t, int>;
  using Ref = SegTreeReferenceBase<size_t, int>;
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = DST(0, kTreeEnd, 0);
  auto reference = Ref(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t iterNum : iota(0, 100)) {
    const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
    const auto setVal = std::uniform_int_distribution(0, 1000)(generator);
    tree.set(rngBegin, rngEnd, setVal);
    reference.set(rngBegin, rngEnd, setVal);

    constexpr auto kids = iota(size_t{0}, kTreeEnd);
    const auto treeVals = kids | transform(bind_front(&DST::get, &tree));
    const auto refVals = kids | transform(bind_front(&Ref::get, &reference));
    EXPECT_TRUE(equal(treeVals, refVals));
  }
}

TYPED_TEST(AllocatorsCommonTest, FuzzGetSetMixed) {
  constexpr auto kTreeEnd = size_t{1000};
  auto tree = typename TestFixture::template DST<size_t, int>(0, kTreeEnd, 0);
  auto reference = SegTreeReferenceBase<size_t, int>(0, kTreeEnd, 0);

  constexpr auto kGenSeed = 42U;
  std::mt19937 generator(kGenSeed);

  for ([[maybe_unused]] size_t i : iota(0, 100)) {
    if (std::bernoulli_distribution()(generator)) {
      const auto [rngBegin, rngEnd] = GenerateIndRng(0, kTreeEnd)(generator);
      const auto valueToSet = std::uniform_int_distribution(0, 1000)(generator);
      tree.set(rngBegin, rngEnd, valueToSet);
      reference.set(rngBegin, rngEnd, valueToSet);
    } else {
      const auto idx = std::uniform_int_distribution<size_t>(0, 999)(generator);
      const auto treeRes = tree.get(idx);
      const auto refRes = reference.get(idx);
      EXPECT_EQ(treeRes, refRes);
    }
  }
};

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cert-err58-cpp, cert-msc32-c)
