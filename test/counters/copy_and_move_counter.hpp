////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// CopyAndMoveCounter - copy and move operations counter.
class CopyAndMoveCounter {
 public:
  struct Stats;

 public:
  struct Init;

  static Init init();

 private:
  explicit CopyAndMoveCounter(std::shared_ptr<Stats> stats)
      : stats_(std::move(stats)) {
  }

 public:
  CopyAndMoveCounter(const CopyAndMoveCounter& other);

  CopyAndMoveCounter(CopyAndMoveCounter&& other) noexcept;

  CopyAndMoveCounter& operator=(const CopyAndMoveCounter& other);

  CopyAndMoveCounter& operator=(CopyAndMoveCounter&& other) noexcept;

  ~CopyAndMoveCounter() = default;

 private:
  std::shared_ptr<Stats> stats_;
};

class CopyAndMoveCounter::Stats {
 private:
  Stats() = default;

 public:
  Stats(const Stats& other) = default;
  Stats(Stats&& other) noexcept = default;
  Stats& operator=(const Stats& other) = default;
  Stats& operator=(Stats&& other) noexcept = default;
  [[nodiscard]] std::size_t getCopyCount() const {
    return copyCount_;
  }
  [[nodiscard]] std::size_t getMoveCount() const {
    return moveCount_;
  }
  void reset() {
    copyCount_ = 0;
    moveCount_ = 0;
  }
  ~Stats() = default;

 private:
  void increaseCopyCount_() {
    ++copyCount_;
  }
  void increaseMoveCount_() {
    ++moveCount_;
  }

 private:
  std::size_t copyCount_{};
  std::size_t moveCount_{};

 private:
  friend class CopyAndMoveCounter;
};

struct CopyAndMoveCounter::Init {
  Init() : counter(stats) {
  }
  std::shared_ptr<Stats> stats{new Stats()};
  CopyAndMoveCounter counter;
};
