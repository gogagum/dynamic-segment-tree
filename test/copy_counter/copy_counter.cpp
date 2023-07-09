////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "copy_counter.hpp"

auto CopyCounter::init() -> Init {
  return {};
}

CopyCounter::CopyCounter(const CopyCounter& other)
    : stats_(other.stats_) {
  stats_->increaseCopyCount_();
}

CopyCounter& CopyCounter::operator=(const CopyCounter& other) {
  if (this == &other) {}
  if (stats_.get() != other.stats_.get()) {
    // throw exception
  }
  stats_->increaseCopyCount_();
  return *this;
}

std::size_t CopyCounter::Stats::getCopyCount() const {
  return copyCount_;
}

void CopyCounter::Stats::increaseCopyCount_() {
  ++copyCount_;
}