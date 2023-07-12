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
  //////////////////////////////////////////////////////////////////////////////
  // Stats - copy counter statistics.
  struct Stats;

 public:
  //////////////////////////////////////////////////////////////////////////////
  // Init - counter instance. A pair of statistics and counter.
  struct Init;

  /**
   * @brief get counter instance.
   *
   * @return Init counter instance. A structure of statistics pointer and
   * counter.
   */
  static Init init();

 private:
  /**
   * @brief Construct a new Copy And Move Counter object
   *
   * @param stats copy and move statistics previously collected, or newly
   * created.
   */
  explicit CopyAndMoveCounter(std::shared_ptr<Stats> stats)
      : stats_(std::move(stats)) {
  }

 public:
  /**
   * @brief Construct a new Copy And Move Counter object
   * 
   * @param other counter to copy from.
   */
  CopyAndMoveCounter(const CopyAndMoveCounter& other);

  /**
   * @brief Construct a new Copy And Move Counter object
   * 
   * @param other counter to move from.
   */
  CopyAndMoveCounter(CopyAndMoveCounter&& other) noexcept;

  /**
   * @param other counter to copy from.
   * @return CopyAndMoveCounter& - referense to itself.
   */
  CopyAndMoveCounter& operator=(const CopyAndMoveCounter& other);

  /**
   * @param other counter to move from.
   * @return CopyAndMoveCounter& - reference to itself.
   */
  CopyAndMoveCounter& operator=(CopyAndMoveCounter&& other) noexcept;

  ~CopyAndMoveCounter() = default;

 private:
  std::shared_ptr<Stats> stats_;
};

class CopyAndMoveCounter::Stats {
 private:
  Stats() = default;

 public:

  /**
   * @brief Construct a new Stats object
   * 
   * @param other statistics to copy from.
   */
  Stats(const Stats& other) = default;

  /**
   * @brief Construct a new Stats object
   * 
   * @param other statistics to move from.
   */
  Stats(Stats&& other) noexcept = default;

  Stats& operator=(const Stats& other) = default;
  Stats& operator=(Stats&& other) noexcept = default;
  
  /**
   * @brief copy count getter.
   * 
   * @return std::size_t copy count.
   */
  [[nodiscard]] std::size_t getCopyCount() const {
    return copyCount_;
  }

  /**
   * @brief Get the Move Count object.
   * 
   * @return std::size_t move count.
   */
  [[nodiscard]] std::size_t getMoveCount() const {
    return moveCount_;
  }

  /**
   * @brief reset - set counts to zeros.
   */
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
  std::shared_ptr<Stats> stats{new Stats()};
  CopyAndMoveCounter counter{stats};
};
