////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef SEG_TREE_REFERENCE_BASE_HPP
#define SEG_TREE_REFERENCE_BASE_HPP

#include <algorithm>
#include <concepts>
#include <ranges>
#include <vector>

namespace rng = std::ranges;
namespace rngv = std::ranges::views;

////////////////////////////////////////////////////////////////////////////////
/// \brief The SegTreeReferenceBase class - base segment tree mock class.
///
template <std::integral KeyT, class ValueT>
class SegTreeReferenceBase {
 public:
  /**
   * @brief SegTreeReferenceBase constructor.
   * @param begin begining of a working area.
   * @param end ending of a working area (not included).
   * @param value default velue, which is set on a range.
   */
  SegTreeReferenceBase(KeyT begin, KeyT end, const ValueT& value);

  /**
   * @brief update - apply update operation on a range with argument
   * `toUpdate.
   * @param begin beginning of a range.
   * @param end ending of a range (not included).
   * @param updateOp applied operation.
   * @param toUpdate operation argument.
   */
  template <class UpdateOp, class UpdateArgT>
  void update(KeyT begin, KeyT end, const UpdateOp& updateOp,
              const UpdateArgT& toUpdate);

  /**
   * @brief update - apply update operation on a range (with no argument).
   * @param begin beginning of a range.
   * @param end ending of a range (not included).
   * @param updateOp operation argument.
   */
  template <class UpdateOp>
  void update(KeyT begin, KeyT end, const UpdateOp& updateOp);

  /**
   * @brief set - set value on a range.
   * @param begin beginning of a range.
   * @param end ending of a range (not included).
   * @param toSet value to set.
   */
  void set(KeyT begin, KeyT end, const ValueT& toSet) {
    rng::fill(getValuesRng_(begin, end), toSet);
  }

  /**
   * @brief get - get value by key.
   * @param key key.
   * @return value, received by key.
   */
  [[nodiscard]] const ValueT& get(KeyT key) const {
    return getValue_(key);
  }

 protected:
  [[nodiscard]] ValueT& getValue_(KeyT key) {
    return values_[key - begin_];
  }
  [[nodiscard]] const ValueT& getValue_(KeyT key) const {
    return values_[key - begin_];
  }

  [[nodiscard]] auto getValuesRng_(KeyT begin, KeyT end) const;

  [[nodiscard]] auto getValuesRng_(KeyT begin, KeyT end);

 private:
  KeyT begin_;
  std::vector<ValueT> values_;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase(KeyT begin, KeyT end,
                                                         const ValueT& value)
    : begin_(begin), values_(end - begin, value) {
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
template <class UpdateOp, class UpdateArgT>
void SegTreeReferenceBase<KeyT, ValueT>::update(KeyT begin, KeyT end,
                                                const UpdateOp& updateOp,
                                                const UpdateArgT& toUpdate) {
  for (auto& val : getValuesRng_(begin, end)) {
    val = updateOp(val, toUpdate);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
template <class UpdateOp>
void SegTreeReferenceBase<KeyT, ValueT>::update(KeyT begin, KeyT end,
                                                const UpdateOp& updateOp) {
  for (auto& val : getValuesRng_(begin, end)) {
    val = updateOp(val);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
auto SegTreeReferenceBase<KeyT, ValueT>::getValuesRng_(KeyT begin, KeyT end) {
  return values_ | rngv::drop(begin - begin_) | rngv::take(end - begin);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT>
auto SegTreeReferenceBase<KeyT, ValueT>::getValuesRng_(KeyT begin,
                                                       KeyT end) const {
  return values_ | rngv::drop(begin - begin_) | rngv::take(end - begin);
}

#endif  // SEG_TREE_REFERENCE_BASE_HPP
