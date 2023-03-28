////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef SUM_SEG_TREE_REFERENCE_HPP
#define SUM_SEG_TREE_REFERENCE_HPP

#include "seg_tree_reference_base.hpp"

#include <concepts>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
/// \brief The SumSegTreeReference<KeyT, ValueT> class.
/// A reference of a data structure with a function of calculatind a sum of
/// values on a range.
template <std::integral KeyT, class ValueT>
class SumSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
 public:
  using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
  /**
   * @param begin beginning index.
   * @param end ending index (not included).
   * @return sum on a range from begin to end, not including end.
   */
  ValueT rangeGet(KeyT begin, KeyT end) const;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
ValueT SumSegTreeReference<KeyT, ValueT>::rangeGet(KeyT begin, KeyT end) const {
  auto ret = 0;
  for (KeyT i = begin; i < end; ++i) {
    ret += this->getValue_(i);
  }
  return ret;
}

#endif  // SUM_SEG_TREE_REFERENCE_HPP
