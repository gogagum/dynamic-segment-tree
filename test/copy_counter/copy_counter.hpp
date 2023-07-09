////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// CopyCounter - copy operation counter.
class CopyCounter {
 public:
  //////////////////////////////////////////////////////////////////////////////
  // Stats - copy counter statistics.
  struct Stats;

 public:
  struct Init;

  static Init init();

 private:
  explicit CopyCounter(std::shared_ptr<Stats> stats)
      : stats_{std::move(stats)} {};

 public:
  CopyCounter(const CopyCounter& other);

  CopyCounter(CopyCounter&& other) noexcept = delete;

  CopyCounter& operator=(const CopyCounter& other);

  CopyCounter& operator=(CopyCounter&& other) = delete;

  ~CopyCounter() = default;

 private:
  std::shared_ptr<Stats> stats_;
};

class CopyCounter::Stats {
private:
 Stats() = default;
public:
 Stats(const Stats& other) = default;
 Stats(Stats&& other) noexcept = default;
 Stats& operator=(const Stats& other) = default;
 Stats& operator=(Stats&& other) = default;
 [[nodiscard]] std::size_t getCopyCount() const;
 ~Stats() = default;
private:
 void increaseCopyCount_();

private:
 std::size_t copyCount_{};
private:
 friend class CopyCounter;
};

struct CopyCounter::Init {
  Init() : counter(stats) {
  }
  std::shared_ptr<Stats> stats{new Stats()};
  CopyCounter counter;
};
