#include "copy_n_times_then_throw.hpp"

CopyNTimesThenThrow::CopyNTimesThenThrow(const CopyNTimesThenThrow& other)
    : counter_{other.counter_} {
  if (*counter_ > 0) {
    --*counter_;
  } else {
    throw Exception();
  }
}