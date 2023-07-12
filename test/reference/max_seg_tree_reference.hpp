////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MAX_SEG_TREE_REFERENCE_HPP
#define MAX_SEG_TREE_REFERENCE_HPP

#include <algorithm>
#include <concepts>
#include <ranges>

#include "seg_tree_reference_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief The MaxSegTreeReference<KeyT, ValueT> class.
/// A reference of a data structure with a function of finding a maximal
/// value on a range.
template <std::integral KeyT, class ValueT>
class MaxSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
 public:
  using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
  /**
   * @param begin beginning index.
   * @param end ending index (not included).
   * @return maximal value on a range from begin to end, not including
   * end.
   */
  [[nodiscard]] ValueT rangeGet(KeyT begin, KeyT end) const {
    return rng::max(this->getValuesRng_(begin, end));
  }
};

#endif  // MAX_SEG_TREE_REFERENCE_HPP
