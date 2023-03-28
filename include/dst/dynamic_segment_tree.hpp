////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_SEGMENT_TREE_HPP
#define DYNAMIC_SEGMENT_TREE_HPP

#include <algorithm>
#include <concepts>
#include <functional>

#include <dst/impl/dynamic_segment_tree_range_get_variation_base.hpp>
#include <dst/impl/dynamic_segment_tree_update_variation_base.hpp>
#include <dst/impl/node.hpp>

#include <dst/concepts.hpp>
#include <dst/disable_operations.hpp>
#include <dst/mp.hpp>

namespace dst {

////////////////////////////////////////////////////////////////////////////////
/// \brief The DynamicSegmentTree class
///
template <std::integral KeyT, class ValueT, class GetValueT,
          class SegGetComb = NoRangeGetOp, class SegGetInit = NoRangeGetOp,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
class DynamicSegmentTree
    : protected impl::DynamicSegmentTreeUpdateVariationBase<
          ValueT, UpdateOp, UpdateArgT, Allocator>,
      protected impl::DynamicSegmentTreeRangeGetCombineVariationBase<
          KeyT, GetValueT, SegGetComb>,
      protected impl::DynamicSegmentTreeRangeGetInitVariationBase<
          KeyT, ValueT, GetValueT, SegGetInit> {
private:
  using UpdateVariationBase_ =
      impl::DynamicSegmentTreeUpdateVariationBase<ValueT, UpdateOp, UpdateArgT,
                                                  Allocator>;

  using RangeGetCombineVariationBase_ =
      impl::DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                           SegGetComb>;

  using RangeGetInitVariationBase_ =
      impl::DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT, GetValueT,
                                                        SegGetInit>;

  using Node_ = typename UpdateVariationBase_::Node_;

public:
  /**
   *  @param begin - beginning of a working area.
   *  @param end - ending of a working area (not included).
   *  @param value - default filling value.
   *  @param segGetComb - functor called when two segments are combined in
   *  range get operation.
   *  @param segGetInit - functor called for get result initialization on an
   *  equally filled segment.
   *  @param updateOp - update operation.
   *  @param alloc - allocator.
   */
  DynamicSegmentTree(KeyT begin, KeyT end, const ValueT &value = ValueT(),
                     const SegGetComb &segGetComb = SegGetComb(),
                     const SegGetInit &segGetInit = SegGetInit(),
                     const UpdateOp &updateOp = UpdateOp(),
                     const Allocator &alloc = Allocator());

  /**
   * @brief update - apply one argument operation on a range.
   * @param begin - beginning of an updated segment.
   * @param end - ending of an updated segment (not included).
   * @param toUpdate - argument for update operation.
   */
  template <class UpdateArg_>
    requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
  void update(KeyT begin, KeyT end, const UpdateArg_ &toUpdate);
  /**
   * @brief update - apply no arguments update operation on a range.
   * @param begin - beginning of an updated segment.
   * @param end - ending of an updated segment (not included).
   */
  void update(KeyT begin, KeyT end)
    requires conc::OneArgUpdateOp<UpdateOp, ValueT>;

  /**
   * @brief set - set value on a range.
   * @param begin - beginning of an updated segment.
   * @param end - ending of an updated segment (not included).
   * @param toSet - value to set.
   */
  void set(KeyT begin, KeyT end, const ValueT &toSet);

  /**
   * @brief get - get value by index.
   * @param key - index.
   * @return value in index.
   */
  const ValueT& get(KeyT key) const;

  /**
   * @brief rangeGet - get result on a range.
   * @param begin - beginning of a range.
   * @param end - ending of a range.
   * @return - range get operation result.
   */
  ValueT rangeGet(KeyT begin, KeyT end) const
    requires conc::GetCombiner<SegGetComb, GetValueT, KeyT> &&
             conc::GetInitializer<SegGetInit, ValueT, KeyT, GetValueT>;

private:
  void setImpl_(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                Node_ *currNode, const ValueT &toUpdate);
  const ValueT &getImpl_(KeyT key, KeyT currBegin, KeyT currEnd,
                         Node_ *currNode) const;
  ValueT rangeGetImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                       Node_ *currNode) const;

private:
  const ValueT outerVal_;
  mutable Node_ rootNode_;
  const KeyT begin_;
  const KeyT end_;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::DynamicSegmentTree(KeyT begin, KeyT end, const ValueT &value,
                                   const SegGetComb &segCombiner,
                                   const SegGetInit &segInitializer,
                                   const UpdateOp &updateOp,
                                   const Allocator & /*alloc*/)
    : outerVal_{}, rootNode_(value), begin_(begin), end_(end),
      RangeGetInitVariationBase_(segInitializer),
      RangeGetCombineVariationBase_(segCombiner),
      UpdateVariationBase_(updateOp) {}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
template <class UpdateArgT_>
  requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT,
                        Allocator>::update(KeyT begin, KeyT end,
                                           const UpdateArgT_ &toUpdate) {
  UpdateVariationBase_::updateImpl_(begin, end, begin_, end_, &rootNode_,
                                    static_cast<const UpdateArgT &>(toUpdate));
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT, Allocator>::update(KeyT begin,
                                                                 KeyT end)
  requires conc::OneArgUpdateOp<UpdateOp, ValueT>
{
  UpdateVariationBase_::updateImpl_(begin, end, begin_, end_, &rootNode_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT,
                        Allocator>::set(KeyT begin, KeyT end,
                                        const ValueT &toUpdate) {
  setImpl_(begin, end, begin_, end_, &rootNode_, toUpdate);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
const ValueT &
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::get(KeyT key) const {
  if (key >= end_ || key < begin_) {
    return outerVal_;
  }
  return getImpl_(key, begin_, end_, &rootNode_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::rangeGet(KeyT begin, KeyT end) const
  requires conc::GetCombiner<SegGetComb, GetValueT, KeyT> &&
           conc::GetInitializer<SegGetInit, ValueT, KeyT, GetValueT>
{
  return rangeGetImpl_(begin, end, begin_, end_, &rootNode_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT,
                        Allocator>::setImpl_(KeyT begin, KeyT end,
                                             KeyT currBegin, KeyT currEnd,
                                             Node_ *currNode,
                                             const ValueT &toUpdate) {
  if (begin >= currEnd || currBegin >= end) {
    return;
  }
  if (end >= currEnd && begin <= currBegin) {
    currNode->setValue(toUpdate);
    return;
  }
  if (currNode->isLeaf()) {
    currNode->initChildren();
  }
  const auto m = (currBegin + currEnd) / 2;
  this->optionalSiftNodeUpdate_(currNode);
  setImpl_(begin, end, currBegin, m, currNode->getLeft(), toUpdate);
  setImpl_(begin, end, m, currEnd, currNode->getRight(), toUpdate);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
const ValueT &
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::getImpl_(KeyT key, KeyT currBegin,
                                                    KeyT currEnd,
                                                    Node_ *currNode) const {
  if (currNode->isLeaf()) {
    return currNode->getValue();
  }
  this->optionalSiftNodeUpdate_(currNode);
  if (auto m = (currBegin + currEnd) / 2; key >= m) {
    return getImpl_(key, m, currEnd, currNode->getRight());
  } else {
    return getImpl_(key, currBegin, m, currNode->getLeft());
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT, class SegGetComb,
          class SegGetInit, class UpdateOp, class UpdateArgT, class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::rangeGetImpl_(KeyT begin, KeyT end,
                                                         KeyT currBegin,
                                                         KeyT currEnd,
                                                         Node_ *currNode)
    const {
  if (begin > currEnd || currBegin > end) {
    assert(false &&
           "_rangeGetImpl must not be called out of initial get range.");
    return ValueT{};
  }
  if (end >= currEnd && begin <= currBegin && currNode->isLeaf()) {
    return RangeGetInitVariationBase_::initGet_(currBegin, currEnd,
                                                currNode->getValue());
  }
  if (currNode->isLeaf()) {
    currNode->initChildren();
  }

  UpdateVariationBase_::optionalSiftNodeUpdate_(currNode);
  const auto m = (currBegin + currEnd) / 2;

  if (begin >= m) { // only right
    return rangeGetImpl_(begin, end, m, currEnd, currNode->getRight());
  }

  if (end <= m) { // only left
    return rangeGetImpl_(begin, end, currBegin, m, currNode->getLeft());
  }

  Node_ *const rightNodePtr = currNode->getRight();
  Node_ *const leftNodePtr = currNode->getLeft();
  const ValueT rVal = rangeGetImpl_(begin, end, m, currEnd, rightNodePtr);
  const ValueT lVal = rangeGetImpl_(begin, end, currBegin, m, leftNodePtr);

  return RangeGetCombineVariationBase_::combineGet_(
      lVal, rVal, std::max(currBegin, begin), m, std::min(currEnd, end));
}

} // namespace dst

#endif // DYNAMIC_SEGMENT_TREE_HPP
