////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef AVG_SEG_TREE_REFERENCE_HPP
#define AVG_SEG_TREE_REFERENCE_HPP

#include <concepts>
#include <numeric>

#include "seg_tree_reference_base.hpp"

////////////////////////////////////////////////////////////////////////////////
/// \brief The AvgSegTreeReference<KeyT, ValueT> class.
/// A reference of a data structure with a function of finding an average
/// value on a range.
template <std::integral KeyT, class ValueT>
class AvgSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
 public:
  using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
  /**
   * @param begin beginning index.
   * @param end ending index (not included).
   * @return average value on a range from begin to end, not including
   * end.
   */
  [[nodiscard]] ValueT rangeGet(KeyT begin, KeyT end) const;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
ValueT AvgSegTreeReference<KeyT, ValueT>::rangeGet(KeyT begin, KeyT end) const {
  auto values = this->getValuesRng_(begin, end);
  return std::accumulate(values.begin(), values.end(), ValueT{0}) /
         (end - begin);
}

#endif  // AVG_SEG_TREE_REFERENCE_HPP
