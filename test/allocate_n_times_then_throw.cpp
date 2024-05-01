#include "tools/allocate_n_times_then_throw.hpp"

#include <gtest/gtest.h>

#include <memory>

TEST(AllocateNTimesThenThrow, Construct) {
  auto alloc = AllocateNTimesThenThrow<int>(3);
}

TEST(AllocateNTimesThenThrow, ThrowsOnNthTime) {
  using Alloc = AllocateNTimesThenThrow<int>;
  using AllocTraits = std::allocator_traits<Alloc>;

  auto alloc = Alloc(2);

  auto* const firstAllocation = AllocTraits::allocate(alloc, 4);
  auto* const secondAllocation = AllocTraits::allocate(alloc, 2);

  EXPECT_ANY_THROW(auto* const thirdAlloc = AllocTraits::allocate(alloc, 3));

  AllocTraits::deallocate(alloc, firstAllocation, 4);
  AllocTraits::deallocate(alloc, secondAllocation, 2);
}
