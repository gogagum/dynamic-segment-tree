////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "copy_only_copy_counter.hpp"

////////////////////////////////////////////////////////////////////////////////
auto CopyOnlyCopyCounter::init() -> Init {
  return {};
}

////////////////////////////////////////////////////////////////////////////////
CopyOnlyCopyCounter::CopyOnlyCopyCounter(const CopyOnlyCopyCounter& other)
    : stats_(other.stats_) {
  stats_->increaseCopyCount_();
}

////////////////////////////////////////////////////////////////////////////////
CopyOnlyCopyCounter& CopyOnlyCopyCounter::operator=(
    const CopyOnlyCopyCounter& other) {
  if (this == &other) {
  }
  if (stats_.get() != other.stats_.get()) {
    stats_ = other.stats_;
  }
  stats_->increaseCopyCount_();
  return *this;
}