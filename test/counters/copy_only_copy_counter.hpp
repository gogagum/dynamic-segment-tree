////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
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
   * @brief Construct a new CopyOnlyCopyCounter object
   *
   * @param stats newly created or previously collected statistics.
   */
  explicit CopyOnlyCopyCounter(std::shared_ptr<Stats> stats)
      : stats_{std::move(stats)} {};

 public:
  /**
   * @brief Construct a new Copy Only Copy Counter object
   *
   * @param other counter to copy from.
   */
  CopyOnlyCopyCounter(const CopyOnlyCopyCounter& other);

  CopyOnlyCopyCounter(CopyOnlyCopyCounter&& other) noexcept = delete;

  /**
   * @param other counter to copy from.
   * @return CopyOnlyCopyCounter&
   */
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

  /**
   * @param other statistics to copy from.
   * @return Stats& reference to itself.
   */
  Stats& operator=(const Stats& other) = default;

  /**
   * @param other statistics to move from.
   * @return Stats& reference to itself.
   */
  Stats& operator=(Stats&& other) = default;

  /**
   * @brief Get copy count
   *
   * @return std::size_t copy count.
   */
  [[nodiscard]] std::size_t getCopyCount() const {
    return copyCount_;
  }

  /**
   * @brief set copy count to zero.
   */
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
  std::shared_ptr<Stats> stats{new Stats()};
  CopyOnlyCopyCounter counter{stats};
};
