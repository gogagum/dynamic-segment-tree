////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// CopyOnlyCopyCounter - copy operation counter.
class CopyOnlyCopyCounter {
 public:
  //////////////////////////////////////////////////////////////////////////////
  // Stats - copy counter statistics.
  struct Stats;

 public:
  struct Init;

  static Init init();

 private:
  explicit CopyOnlyCopyCounter(std::shared_ptr<Stats> stats)
      : stats_{std::move(stats)} {};

 public:
  CopyOnlyCopyCounter(const CopyOnlyCopyCounter& other);

  CopyOnlyCopyCounter(CopyOnlyCopyCounter&& other) noexcept = delete;

  CopyOnlyCopyCounter& operator=(const CopyOnlyCopyCounter& other);

  CopyOnlyCopyCounter& operator=(CopyOnlyCopyCounter&& other) = delete;

  ~CopyOnlyCopyCounter() = default;

 private:
  std::shared_ptr<Stats> stats_;
};

class CopyOnlyCopyCounter::Stats {
 private:
  Stats() = default;

 public:
  Stats(const Stats& other) = default;
  Stats(Stats&& other) noexcept = default;
  Stats& operator=(const Stats& other) = default;
  Stats& operator=(Stats&& other) = default;
  [[nodiscard]] std::size_t getCopyCount() const {
    return copyCount_;
  }
  void reset() {
    copyCount_ = 0;
  }
  ~Stats() = default;

 private:
  void increaseCopyCount_() {
    ++copyCount_;
  }

 private:
  std::size_t copyCount_{};

 private:
  friend class CopyOnlyCopyCounter;
};

struct CopyOnlyCopyCounter::Init {
  Init() : counter(stats) {
  }
  std::shared_ptr<Stats> stats{new Stats()};
  CopyOnlyCopyCounter counter;
};
