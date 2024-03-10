////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef GENERATE_INDEX_RANGE
#define GENERATE_INDEX_RANGE

#include <algorithm>
#include <concepts>
#include <random>

template <std::integral T>
class GenerateIndexRange {
 private:
  struct Rng_ {
    T begin;
    T end;
  };

 public:
  GenerateIndexRange(const T& begin, const T& end) : begin_{begin}, end_{end} {
  }

  template <class GenT>
  Rng_ operator()(GenT& gen) {
    const auto first = std::uniform_int_distribution(begin_, end_)(gen);
    const auto second = std::uniform_int_distribution(begin_, end_)(gen);

    const auto [begin, end] = std::minmax(first, second);

    return {begin, end};
  }

 private:
  T begin_;
  T end_;
};

#endif  // GENERATE_INDEX_RANGE
