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