////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef NON_MOVABLE_AND_NON_COPYABLE
#define NON_MOVABLE_AND_NON_COPYABLE

#include <boost/noncopyable.hpp>
#include <utility>

template <class T>
class NonMovableAndNonCopyable : boost::noncopyable {
 public:
  struct Construct {
    NonMovableAndNonCopyable<T> operator()(const T& value) const {
      return NonMovableAndNonCopyable<T>{value};
    }
    NonMovableAndNonCopyable<T> operator()(T&& value) const {
      return NonMovableAndNonCopyable<T>{std::move(value)};
    }
  };

 public:
  explicit NonMovableAndNonCopyable(const T& value) : value_{value} {
  }
  explicit NonMovableAndNonCopyable(T&& value) noexcept
      : value_{std::move(value)} {
  }
  T& get() {
    return value_;
  }
  [[nodiscard]] const T& get() const {
    return value_;
  }

 private:
  T value_;
};

static_assert(!std::is_copy_constructible_v<NonMovableAndNonCopyable<int>>);
static_assert(!std::is_move_constructible_v<NonMovableAndNonCopyable<int>>);

#endif  // NON_MOVABLE_AND_NON_COPYABLE
