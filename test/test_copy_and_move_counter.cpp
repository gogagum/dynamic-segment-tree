////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <ranges>

#include "counters/copy_and_move_counter.hpp"

using std::views::iota;

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-*)

static_assert(std::copyable<CopyAndMoveCounter>);

TEST(CopyAndMoveCounter, Init) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();
}

TEST(CopyAndMoveCounter, InitialCounts) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  EXPECT_EQ(stats->getCopyCount(), 0);
  EXPECT_EQ(stats->getMoveCount(), 0);
}

TEST(CopyAndMoveCounter, MakeOneCopy) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  [[maybe_unused]] auto onlyCopy = copyAndMoveCounter;

  EXPECT_EQ(stats->getCopyCount(), 1);
  EXPECT_EQ(stats->getMoveCount(), 0);
}

TEST(CopyAndMoveCounter, MakeOneCopyAndReset) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  [[maybe_unused]] auto onlyCopy = copyAndMoveCounter;

  stats->reset();

  EXPECT_EQ(stats->getCopyCount(), 0);
  EXPECT_EQ(stats->getMoveCount(), 0);
}

TEST(CopyAndMoveCounter, MakeOneMove) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  auto moved = std::move(copyAndMoveCounter);

  EXPECT_EQ(stats->getCopyCount(), 0);
  EXPECT_EQ(stats->getMoveCount(), 1);
}

TEST(CopyAndMoveCounter, Make42Copies) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  for ([[maybe_unused]] auto iterNum : iota(0, 42)) {
    [[maybe_unused]] auto copy = copyAndMoveCounter;
  }

  EXPECT_EQ(stats->getCopyCount(), 42);
  EXPECT_EQ(stats->getMoveCount(), 0);
}

TEST(CopyAndMoveCounter, Make42CopiesAndReset) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  for ([[maybe_unused]] auto iterNum : iota(0, 42)) {
    [[maybe_unused]] auto copy = copyAndMoveCounter;
  }

  stats->reset();

  EXPECT_EQ(stats->getCopyCount(), 0);
  EXPECT_EQ(stats->getMoveCount(), 0);
}

TEST(CopyAndMoveCounter, Make42CopiesAndMoves) {
  auto [stats, copyAndMoveCounter] = CopyAndMoveCounter::init();

  for ([[maybe_unused]] auto iterNum : iota(0, 42)) {
    auto copy = copyAndMoveCounter;
    [[maybe_unused]] auto moved = std::move(copy);
  }

  EXPECT_EQ(stats->getCopyCount(), 42);
  EXPECT_EQ(stats->getMoveCount(), 42);
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-*)
