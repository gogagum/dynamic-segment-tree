////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <memory>
#include <utility>

class ConstructionAndDestructionCounter {
 public:
  struct Stats;

  using Init = std::shared_ptr<Stats>;

 public:
  static Init init() {
    return std::make_shared<Stats>(0, 0);
  }

  explicit ConstructionAndDestructionCounter(Init init);
  ConstructionAndDestructionCounter(
      const ConstructionAndDestructionCounter& other);
  ConstructionAndDestructionCounter(
      ConstructionAndDestructionCounter&& other) noexcept;

  ConstructionAndDestructionCounter& operator=(
      const ConstructionAndDestructionCounter&) = default;
  ConstructionAndDestructionCounter& operator=(
      ConstructionAndDestructionCounter&& other) noexcept {
    init_ = other.init_;
    return *this;
  };

  ~ConstructionAndDestructionCounter();

 private:
  Init init_;
};

struct ConstructionAndDestructionCounter::Stats {
  std::size_t constructionCnt;
  std::size_t destructionCnt;
};
