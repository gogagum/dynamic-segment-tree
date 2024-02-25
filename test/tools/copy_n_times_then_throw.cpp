////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "copy_n_times_then_throw.hpp"

CopyNTimesThenThrow::CopyNTimesThenThrow(const CopyNTimesThenThrow& other)
    : counter_{other.counter_} {
  if (*counter_ > 0) {
    --*counter_;
  } else {
    throw Exception();
  }
}