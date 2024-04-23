////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_SEGMENT_TREE_HPP
#define DYNAMIC_SEGMENT_TREE_HPP

#include <algorithm>
#include <concepts>
#include <dst/concepts.hpp>
#include <dst/disable_operations.hpp>
#include <dst/impl/dynamic_segment_tree_range_get_variation_base.hpp>
#include <dst/impl/dynamic_segment_tree_update_variation_base.hpp>
#include <dst/impl/node.hpp>
#include <dst/mp.hpp>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace dst {

////////////////////////////////////////////////////////////////////////////////
/// \brief The DynamicSegmentTree class
///
/// \tparam KeyT integral key type. Indicies of elements in a segment tree.
/// \tparam ValueT value type of the elements in a segment tree.
/// \tparam GetValueT type of a value returned from get and rangeGet operations.
/// \tparam SegGetComb two segments combiner for rangeGet operation. It has
/// three possible forms: 1. combiner of two get-values (makes range get
/// operation result from two results of two parts); 2. combiner of two
/// get-values and ranges borders (makes range get operation result from two
/// results of two parts, beginning index of a first range, separating index,
/// and second range end index); 3. NoRangeGetOp, if rangeGet operation
/// is turned off.
/// \tparam SegGetInit rangeGet operation initializer. It has
/// three possible forms: 1. initializer of a segment from one value (const
/// ValueT& -> GetValueT); 2. initializer of a segment from one value and range
/// borders ((const ValueT&, KeyT, KeyT) -> GetValueT); 3. NoRangeGetOp, if
/// rangeGet operation is turned off.
/// \tparam UpdateOp update operation functor. It has three possible forms:
/// 1. update operation, that does not take any parameters, except for updated
/// value; 2. update operation, that takes an additional parameter; 3.
/// NoUpdateOp, if update operation is turned off.
/// \tparam UpdateArgT an argument for update operation. Used only for the
/// second form of UpdateOp functor. In other cases this parameter is ignored.
/// \tparam Allocator allocator for nodes of a dynamic segment tree.
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb = NoRangeGetOp,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit =
              NoRangeGetOp,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = mp::DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
class DynamicSegmentTree
    : public impl::DynamicSegmentTreeUpdateVariationBase<
          DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                             UpdateOp, UpdateArgT, Allocator>>,
      protected impl::DynamicSegmentTreeRangeGetCombineVariationBase<
          KeyT, GetValueT, SegGetComb>,
      protected impl::DynamicSegmentTreeRangeGetInitVariationBase<
          KeyT, ValueT, GetValueT, SegGetInit> {
 private:
  using This_ = DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb,
                                   SegGetInit, UpdateOp, UpdateArgT, Allocator>;

  using UpdateVariationBase_ =
      impl::DynamicSegmentTreeUpdateVariationBase<This_>;

  using RangeGetCombineVariationBase_ =
      impl::DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                           SegGetComb>;

  using RangeGetInitVariationBase_ =
      impl::DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT, GetValueT,
                                                        SegGetInit>;

  using Node_ = UpdateVariationBase_::Node_;

  using NodeAlloc_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Node_>;

  using NodeAllocTraits_ = std::allocator_traits<NodeAlloc_>;

 public:
  /**
   * @brief Construct a new Dynamic Segment Tree object making a copy of
   * `other`.
   *
   * @param other segment tree to copy from.
   */
  DynamicSegmentTree(const DynamicSegmentTree& other);

  /**
   * @brief Construct a new Dynamic Segment Tree object making a copy of
   * `other`, but with use of `other`'s `allocator`.
   *
   * @param other segment tree to copy from.
   * @param allocator new allocator.
   */
  DynamicSegmentTree(const DynamicSegmentTree& other,
                     const Allocator& allocator);

  /**
   * @brief Construct a new Dynamic Segment Tree object moving `other`.
   *
   * @param other segment tree to move from.
   */
  DynamicSegmentTree(DynamicSegmentTree&& other) noexcept;

  /**
   * @brief Construct a new Dynamic Segment Tree object moving, but with use of
   * `other`'s `allocator`.
   *
   * @param other segment tree to move from.
   * @param allocator new allocator.
   */
  DynamicSegmentTree(DynamicSegmentTree&& other,
                     const Allocator& allocator) noexcept;

  /**
   * @brief Construct a new Dynamic Segment Tree object.
   *
   * @param begin beginning of a working area.
   * @param end ending of a working area (not included).
   * @param value default filling value.
   * @param segGetComb functor called when two segments are combined in
   * range get operation.
   * @param segGetInit functor called for get result initialization on an
   * equally filled segment.
   * @param updateOp update operation.
   * @param alloc allocator.
   */
  DynamicSegmentTree(KeyT begin, KeyT end, const ValueT& value,
                     const SegGetComb& segGetComb = SegGetComb{},
                     const SegGetInit& segGetInit = SegGetInit{},
                     const UpdateOp& updateOp = UpdateOp{},
                     const Allocator& alloc = Allocator{});

  /**
   * @brief Construct a new Dynamic Segment Tree object moving filling value.
   *
   * @param begin beginning of a working area.
   * @param end ending of a working area (not included).
   * @param value default filling value.
   * @param segGetComb functor called when two segments are combined in
   * range get operation.
   * @param segGetInit functor called for get result initialization on an
   * equally filled segment.
   * @param updateOp update operation.
   * @param alloc allocator.
   */
  DynamicSegmentTree(KeyT begin, KeyT end, ValueT&& value = ValueT{},
                     const SegGetComb& segGetComb = SegGetComb{},
                     const SegGetInit& segGetInit = SegGetInit{},
                     const UpdateOp& updateOp = UpdateOp{},
                     const Allocator& alloc = Allocator{});

  /**
   * @brief Copy assign operator. Makes a full copy of a dynamic segment tree.
   *
   * @param other tree to copy from.
   * @return DynamicSegmentTree& reference to itself.
   */
  DynamicSegmentTree& operator=(const DynamicSegmentTree& other);

  /**
   * @brief Move assign operator. Moves a tree into another object.
   *
   * @param other tree to move from.
   * @return DynamicSegmentTree& reference to itself.
   */
  DynamicSegmentTree& operator=(DynamicSegmentTree&& other) noexcept;

  /**
   * @brief Apply one argument operation on a range.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   * @param toUpdate argument for update operation.
   */
  // void update(KeyT begin, KeyT end, const UpdateArgT& toUpdate);

  /**
   * @brief Apply no arguments update operation on a range.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   */
  // void update(KeyT begin, KeyT end)

  /**
   * @brief Set value on a range.
   *
   * If `begin` >= `end`, then set operation range is perceived as empty and no
   * changes happen.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   * @param toSet value to set.
   */
  void set(KeyT begin, KeyT end, const ValueT& toSet);

  /**
   * @brief Set value on a range.
   *
   * If `begin` >= `end`, then set operation range is perceived as empty and no
   * changes happen.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   * @param toSet value to set.
   */
  void set(KeyT begin, KeyT end, ValueT&& toSet);

  /**
   * @brief Get value by index.
   *
   * @param key index.
   * @return value in index.
   */
  const ValueT& get(KeyT key) const;

  /**
   * @brief Get result on a range.
   *
   * @param begin beginning of a range.
   * @param end ending of a range.
   * @return range get operation result.
   */
  GetValueT rangeGet(KeyT begin, KeyT end) const
    requires conc::GetCombiner<SegGetComb, GetValueT, KeyT> &&
             conc::GetInitializer<SegGetInit, ValueT, KeyT, GetValueT>;

  /**
   * @brief Destroy the Dynamic Segment Tree object
   */
  ~DynamicSegmentTree();

 private:
  template <class ValueT1>
    requires std::is_same_v<std::remove_cvref_t<ValueT1>, ValueT>
  void setImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                Node_* currNode, ValueT1&& toUpdate);
  const ValueT& getImpl_(KeyT key, KeyT currBegin, KeyT currEnd,
                         Node_* currNode) const;
  GetValueT rangeGetImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                          Node_* currNode) const;

 private:
  mutable NodeAlloc_ nodeAllocator_;
  mutable Node_ rootNode_{};
  KeyT begin_;
  KeyT end_;

 private:
  friend class impl::DynamicSegmentTreeUpdateVariationBase<This_>;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::DynamicSegmentTree(const DynamicSegmentTree& other)
    : nodeAllocator_(other.nodeAllocator_),
      begin_(other.begin_),
      end_(other.end_),
      RangeGetInitVariationBase_(other),
      RangeGetCombineVariationBase_(other),
      UpdateVariationBase_(other) {
  Node_::copySubtree(other.rootNode_, &this->rootNode_, nodeAllocator_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::DynamicSegmentTree(const DynamicSegmentTree& other,
                                   const Allocator& allocator)
    : nodeAllocator_(allocator),
      begin_(other.begin_),
      end_(other.end_),
      RangeGetInitVariationBase_(other),
      RangeGetCombineVariationBase_(other),
      UpdateVariationBase_(other) {
  Node_::copySubtree(other.rootNode_, &this->rootNode_, nodeAllocator_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::DynamicSegmentTree(DynamicSegmentTree&& other) noexcept
    : nodeAllocator_{other.nodeAllocator_},
      begin_{other.begin_},
      end_{other.end_},
      RangeGetInitVariationBase_(
          static_cast<RangeGetInitVariationBase_&&>(other)),
      RangeGetCombineVariationBase_(
          static_cast<RangeGetCombineVariationBase_&&>(other)),
      UpdateVariationBase_(static_cast<UpdateVariationBase_&&>(other)) {
  other.begin_ = 0;
  other.end_ = 0;
  std::construct_at(rootNode_.getValuePtr(),
                    std::move(other.rootNode_.getValue()));
  rootNode_.ptr = other.rootNode_.ptr;
  other.rootNode_.ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::DynamicSegmentTree(DynamicSegmentTree&& other,  // NOLINT
                                   const Allocator& allocator) noexcept
    : nodeAllocator_{allocator},
      begin_{other.begin_},
      end_{other.end_},
      RangeGetInitVariationBase_(
          static_cast<RangeGetInitVariationBase_&&>(other)),
      RangeGetCombineVariationBase_(
          static_cast<RangeGetCombineVariationBase_&&>(other)),
      UpdateVariationBase_(static_cast<UpdateVariationBase_&&>(other)) {
  other.begin_ = 0;
  other.end_ = 0;
  std::construct_at(rootNode_.getValuePtr(),
                    std::move(other.rootNode_.getValue()));
  rootNode_.ptr = other.rootNode_.ptr;
  other.rootNode_.ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT,
                   Allocator>::DynamicSegmentTree(KeyT begin, KeyT end,
                                                  const ValueT& value,
                                                  const SegGetComb& segGetComb,
                                                  const SegGetInit& segGetInit,
                                                  const UpdateOp& updateOp,
                                                  const Allocator& alloc)
    : nodeAllocator_(alloc),
      begin_(begin),
      end_(end),
      RangeGetInitVariationBase_(segGetInit),
      RangeGetCombineVariationBase_(segGetComb),
      UpdateVariationBase_(updateOp) {
  std::construct_at(rootNode_.getValuePtr(), value);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT,
                   Allocator>::DynamicSegmentTree(KeyT begin, KeyT end,
                                                  ValueT&& value,
                                                  const SegGetComb& segGetComb,
                                                  const SegGetInit& segGetInit,
                                                  const UpdateOp& updateOp,
                                                  const Allocator& alloc)
    : nodeAllocator_(alloc),
      begin_(begin),
      end_(end),
      RangeGetInitVariationBase_(segGetInit),
      RangeGetCombineVariationBase_(segGetComb),
      UpdateVariationBase_(updateOp) {
  std::construct_at(rootNode_.getValuePtr(), std::move(value));
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
auto DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::operator=(const DynamicSegmentTree& other) -> This_& {
  if (this == &other) {
    return *this;
  }
  UpdateVariationBase_::operator=(
      static_cast<const UpdateVariationBase_&>(other));
  RangeGetInitVariationBase_::operator=(
      static_cast<const RangeGetInitVariationBase_&>(other));
  RangeGetCombineVariationBase_::operator=(
      static_cast<const RangeGetCombineVariationBase_&>(other));
  nodeAllocator_ = other.nodeAllocator_;
  begin_ = other.begin_;
  end_ = other.end_;
  Node_::copySubtree(other.rootNode_, &rootNode_, nodeAllocator_);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
auto DynamicSegmentTree<
    KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, UpdateArgT,
    Allocator>::operator=(DynamicSegmentTree&& other) noexcept -> This_& {
  if (this == &other) {
    return *this;
  }
  UpdateVariationBase_::operator=(static_cast<UpdateVariationBase_&&>(other));
  RangeGetInitVariationBase_::operator=(
      static_cast<RangeGetInitVariationBase_&&>(other));
  RangeGetCombineVariationBase_::operator=(
      static_cast<RangeGetCombineVariationBase_&&>(other));
  std::swap(nodeAllocator_, other.nodeAllocator_);
  if (!rootNode_.isLeaf()) {
    rootNode_.clearChildren(nodeAllocator_);
  }
  if (!other.rootNode_.isLeaf()) {
    std::construct_at(rootNode_.getValuePtr(),
                      std::move(other.rootNode_.getValue()));
  }
  rootNode_.ptr = other.rootNode_.ptr;
  other.rootNode_.ptr = nullptr;
  std::swap(begin_, other.begin_);
  std::swap(end_, other.end_);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT,
                        Allocator>::set(KeyT begin, KeyT end,
                                        const ValueT& toSet) {
  if (begin < end) {
    setImpl_(begin, end, begin_, end_, &rootNode_, toSet);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT, Allocator>::set(KeyT begin,
                                                              KeyT end,
                                                              ValueT&& toSet) {
  if (begin < end) {
    setImpl_(begin, end, begin_, end_, &rootNode_, std::move(toSet));
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
const ValueT&
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::get(KeyT key) const {
  if (key >= end_ || key < begin_) {
    std::stringstream messageStream;
    messageStream << "Get operation for " << key << ", which is out of range.";
    throw std::out_of_range(messageStream.str());
  }
  return getImpl_(key, begin_, end_, &rootNode_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
GetValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::rangeGet(KeyT begin, KeyT end) const
  requires conc::GetCombiner<SegGetComb, GetValueT, KeyT> &&
           conc::GetInitializer<SegGetInit, ValueT, KeyT, GetValueT>
{
  return rangeGetImpl_(begin, end, begin_, end_, &rootNode_);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::~DynamicSegmentTree() {
  if (!rootNode_.isLeaf()) {
    rootNode_.clearChildren(nodeAllocator_);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
template <class ValueT1>
  requires std::is_same_v<std::remove_cvref_t<ValueT1>, ValueT>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                        UpdateOp, UpdateArgT,
                        Allocator>::setImpl_(KeyT begin, KeyT end,
                                             KeyT currBegin, KeyT currEnd,
                                             Node_* currNode,
                                             ValueT1&& toUpdate) {
  assert(currBegin < end && "currBegin must be checked before call.");
  assert(currEnd > begin && "currEnd must be checked before call.");
  assert(begin < end && "Function must not be called on empty range");
  if (end >= currEnd && begin <= currBegin) {
    currNode->setOrConstructValue(std::forward<ValueT1>(toUpdate),
                                  nodeAllocator_);
    return;
  }
  if (currNode->isLeaf()) {
    currNode->initChildrenSiftingValue(nodeAllocator_);
  }
  UpdateVariationBase_::optionalSiftNodeUpdate_(currNode, nodeAllocator_);
  const auto mid = (currBegin + currEnd) / 2;
  if (mid >= end) {
    // Only move to left as right is out of range.
    setImpl_(begin, end, currBegin, mid, currNode->getLeft(),
             std::forward<ValueT1>(toUpdate));
    return;
  }
  if (mid <= begin) {
    // Only move to right as left is out of range.
    setImpl_(begin, end, mid, currEnd, currNode->getRight(),
             std::forward<ValueT1>(toUpdate));
    return;
  }
  // Copy to left and move to right
  setImpl_(begin, end, currBegin, mid, currNode->getLeft(), toUpdate);
  setImpl_(begin, end, mid, currEnd, currNode->getRight(),
           std::forward<ValueT1>(toUpdate));
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
const ValueT&
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
                   UpdateArgT, Allocator>::getImpl_(KeyT key, KeyT currBegin,
                                                    KeyT currEnd,
                                                    Node_* currNode) const {
  if (currNode->isLeaf()) {
    return currNode->getValue();
  }
  UpdateVariationBase_::optionalSiftNodeUpdate_(currNode, nodeAllocator_);
  if (const auto mid = (currBegin + currEnd) / 2; key >= mid) {
    return getImpl_(key, mid, currEnd, currNode->getRight());
  } else {
    return getImpl_(key, currBegin, mid, currNode->getLeft());
  }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT, class ValueT, class GetValueT,
          conc::OptGetCombiner<GetValueT, KeyT> SegGetComb,
          conc::OptGetInitializer<ValueT, KeyT, GetValueT> SegGetInit,
          class UpdateOp, class UpdateArgT, class Allocator>
  requires conc::OptUpdateOp<UpdateOp, ValueT, UpdateArgT>
GetValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                             UpdateOp, UpdateArgT,
                             Allocator>::rangeGetImpl_(KeyT begin, KeyT end,
                                                       KeyT currBegin,
                                                       KeyT currEnd,
                                                       Node_* currNode) const {
  if (begin > currEnd || currBegin > end) {
    assert(false &&
           "_rangeGetImpl must not be called out of initial get range.");
    return RangeGetInitVariationBase_::initGet_(currBegin, currEnd,
                                                currNode->getValue());
  }
  if (end >= currEnd && begin <= currBegin && currNode->isLeaf()) {
    return RangeGetInitVariationBase_::initGet_(currBegin, currEnd,
                                                currNode->getValue());
  }
  if (currNode->isLeaf()) {
    currNode->initChildrenSiftingValue(nodeAllocator_);
  }

  UpdateVariationBase_::optionalSiftNodeUpdate_(currNode, nodeAllocator_);
  const auto mid = (currBegin + currEnd) / 2;

  if (begin >= mid) {  // only right
    return rangeGetImpl_(begin, end, mid, currEnd, currNode->getRight());
  }

  if (end <= mid) {  // only left
    return rangeGetImpl_(begin, end, currBegin, mid, currNode->getLeft());
  }

  Node_* const rightNodePtr = currNode->getRight();
  Node_* const leftNodePtr = currNode->getLeft();
  GetValueT rVal = rangeGetImpl_(begin, end, mid, currEnd, rightNodePtr);
  GetValueT lVal = rangeGetImpl_(begin, end, currBegin, mid, leftNodePtr);

  return RangeGetCombineVariationBase_::combineGet_(
      std::move(lVal), std::move(rVal), std::max(currBegin, begin), mid,
      std::min(currEnd, end));
}

}  // namespace dst

#endif  // DYNAMIC_SEGMENT_TREE_HPP
