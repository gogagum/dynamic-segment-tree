////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "copy_n_times_then_throw.hpp"

CopyNTimesThenThrow::CopyNTimesThenThrow(const CopyNTimesThenThrow& other)
    : counter_{[](const CopyNTimesThenThrow& other_) {
        if (const auto& counter = other_.counter_; *counter > 0) {
          --*counter;
          return counter;
        }
        throw Exception();
      }(other)} {
}

CopyNTimesThenThrow& CopyNTimesThenThrow::operator=(const CopyNTimesThenThrow& other) {
  if (const auto& counter = other.counter_; *counter > 0) {
    --*counter;
    counter_ = counter;
    return *this;
  }
  throw Exception();
}
