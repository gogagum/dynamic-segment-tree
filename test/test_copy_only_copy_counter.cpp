////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <ranges>
#include <gtest/gtest.h>
#include "counters/copy_only_copy_counter.hpp"

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*, readability-identifier-length)

namespace rng = std::ranges;

TEST(CopyOnlyCopyCounter, Construct) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  EXPECT_EQ(stats->getCopyCount(), 0);
}

TEST(CopyOnlyCopyCounter, PerformOneCopy) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  auto copy = copyCounter;

  EXPECT_EQ(stats->getCopyCount(), 1);
}

TEST(CopyOnlyCopyCounter, Loop) {
  auto [stats, copyCounter] = CopyOnlyCopyCounter::init();

  for ([[maybe_unused]] std::size_t _: rng::iota_view(0, 42)) {
    [[maybe_unused]] auto copy = copyCounter;
  }

  EXPECT_EQ(stats->getCopyCount(), 42);
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*, readability-identifier-length)
