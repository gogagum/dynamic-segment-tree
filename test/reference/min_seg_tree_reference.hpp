////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MIN_SEG_TREE_REFERENCE_HPP
#define MIN_SEG_TREE_REFERENCE_HPP

#include <algorithm>
#include <concepts>
#include <ranges>

#include "seg_tree_reference_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief The MinSegTreeReference<KeyT, ValueT> class.
/// A reference of a data structure with a function of finding a minimal
/// value on a range.
template <std::integral KeyT, class ValueT>
class MinSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
 public:
  using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
  /**
   * @param begin beginning index.
   * @param end ending index (not included).
   * @return maximal value on a range from begin to end, not including
   * end.
   */
  [[nodiscard]] ValueT rangeGet(KeyT begin, KeyT end) const {
    return rng::min(this->getValuesRng_(begin, end));
  }
};

#endif  // MIN_SEG_TREE_REFERENCE_HPP
