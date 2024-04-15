////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef NON_MOVABLE_AND_NON_COPYABLE_AVG_COMBINER_HPP
#define NON_MOVABLE_AND_NON_COPYABLE_AVG_COMBINER_HPP

#include "non_movable_and_non_copyable.hpp"

template <class T, class KeyT>
struct NonMovableAndNonCopyableAvgCombiner {
  NonMovableAndNonCopyable<T> operator()(
      const NonMovableAndNonCopyable<T>& left,
      const NonMovableAndNonCopyable<T>& right, KeyT leftBegin, KeyT separation,
      KeyT rightEnd) const {
    return NonMovableAndNonCopyable<T>{(left.get() * (separation - leftBegin) +
                                        right.get() * (rightEnd - separation)) /
                                       (rightEnd - leftBegin)};
  }
};

#endif  // NON_MOVABLE_AND_NON_COPYABLE_AVG_COMBINER_HPP