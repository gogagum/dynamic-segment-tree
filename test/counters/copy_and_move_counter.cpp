////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "copy_and_move_counter.hpp"

////////////////////////////////////////////////////////////////////////////////
auto CopyAndMoveCounter::init() -> Init {
  return {};
}

////////////////////////////////////////////////////////////////////////////////
CopyAndMoveCounter::CopyAndMoveCounter(const CopyAndMoveCounter& other)
    : stats_(other.stats_) {
  stats_->increaseCopyCount_();
}

////////////////////////////////////////////////////////////////////////////////
CopyAndMoveCounter::CopyAndMoveCounter(CopyAndMoveCounter&& other) noexcept
    : stats_(std::move(other.stats_)) {
  stats_->increaseMoveCount_();
}

////////////////////////////////////////////////////////////////////////////////
CopyAndMoveCounter& CopyAndMoveCounter::operator=(
    const CopyAndMoveCounter& other) {
  if (this == &other) {
    // Nothing special.
  }
  if (stats_.get() != other.stats_.get()) {
    stats_ = other.stats_;
  }
  stats_->increaseCopyCount_();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
CopyAndMoveCounter& CopyAndMoveCounter::operator=(
    CopyAndMoveCounter&& other) noexcept {
  if (this == &other) {
    // Nothing special.
  }
  if (stats_.get() != other.stats_.get()) {
    stats_ = std::move(other.stats_);
  }
  stats_->increaseMoveCount_();
  return *this;
}
