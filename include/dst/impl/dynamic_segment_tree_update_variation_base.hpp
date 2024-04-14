////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP
#define DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP

#include <dst/concepts.hpp>
#include <dst/disable_operations.hpp>
#include <dst/impl/node.hpp>

namespace dst::impl {

////////////////////////////////////////////////////////////////////////////////
// Update variation.                                                          //
////////////////////////////////////////////////////////////////////////////////
template <class Derived>
class DynamicSegmentTreeUpdateVariationBase;

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit, class UpdateOp,
          class UpdateArgT, class Allocator>
  requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
class DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
            UpdateArgT, Allocator>> {
 protected:
  using Derived_ = Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                           UpdateOp, UpdateArgT, Allocator>;
  using Node_ = Node<ValueT, std::optional<UpdateArgT>, Allocator>;
  using NodeAlloc_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Node_>;

 public:
  /**
   * @brief Apply one argument operation on a range.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   * @param toUpdate argument for update operation.
   */
  void update(KeyT begin, KeyT end, const UpdateArgT& toUpdate);

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
      : updateOp_(updateOp) {
  }

 protected:
  void updateImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                   Node_* currNode, const UpdateArgT& toUpdate,
                   NodeAlloc_& allocator);

  void optionalSiftNodeUpdate_(Node_* nodePtr, NodeAlloc_& allocator) const {
    nodePtr->siftOptUpdate(updateOp_, allocator);
  }

 private:
  UpdateOp updateOp_;
};

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit, class UpdateOp,
          class UpdateArgT, class Allocator>
  requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
void DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
            UpdateArgT, Allocator>>::update(KeyT begin, KeyT end,
                                            const UpdateArgT& toUpdate) {
  updateImpl_(begin, end, static_cast<Derived_*>(this)->begin_,
              static_cast<Derived_*>(this)->end_,
              &static_cast<Derived_*>(this)->rootNode_, toUpdate,
              static_cast<Derived_*>(this)->nodeAllocator_);
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit, class UpdateOp,
          class UpdateArgT, class Allocator>
  requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
void DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp,
            UpdateArgT, Allocator>>::updateImpl_(KeyT begin, KeyT end,
                                                 KeyT currBegin, KeyT currEnd,
                                                 Node_* currNode,
                                                 const UpdateArgT& toUpdate,
                                                 NodeAlloc_& allocator) {
  if (begin >= currEnd || currBegin >= end) {
    return;
  }
  if (end >= currEnd && begin <= currBegin) {
    currNode->update(updateOp_, toUpdate, allocator);
    return;
  }
  if (currNode->isLeaf()) {
    currNode->initChildren(allocator);
  }
  if constexpr (conc::UpdateOp<UpdateOp, ValueT, UpdateArgT>) {
    currNode->siftOptUpdate(updateOp_, allocator);
  }
  assert(currEnd >= currBegin + 2);
  const auto mid = (currBegin + currEnd) / 2;
  updateImpl_(begin, end, currBegin, mid, currNode->getLeft(), toUpdate,
              allocator);
  updateImpl_(begin, end, mid, currEnd, currNode->getRight(), toUpdate,
              allocator);
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit,
          conc::OneArgUpdateOp<ValueT> UpdateOp, class Allocator>
class DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, void,
            Allocator>> {
 protected:
  using Derived_ = Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                           UpdateOp, void, Allocator>;
  using Node_ = Node<ValueT, bool, Allocator>;
  using NodeAlloc_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Node_>;

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
      : updateOp_(updateOp) {
  }

 public:
  /**
   * @brief Apply no arguments update operation on a range.
   *
   * @param begin beginning of an updated segment.
   * @param end ending of an updated segment (not included).
   */
  void update(KeyT begin, KeyT end) {
    updateImpl_(begin, end, static_cast<Derived_*>(this)->begin_,
                static_cast<Derived_*>(this)->end_,
                &static_cast<Derived_*>(this)->rootNode_,
                static_cast<Derived_*>(this)->nodeAllocator_);
  }

 protected:
  template <class NodeAlloc>
  void updateImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                   Node_* currNode, NodeAlloc& allocator);
  void optionalSiftNodeUpdate_(Node_* nodePtr, NodeAlloc_& allocator) const {
    nodePtr->siftOptUpdate(updateOp_, allocator);
  }

 private:
  UpdateOp updateOp_;
};

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit,
          conc::OneArgUpdateOp<ValueT> UpdateOp, class Allocator>
template <class NodeAlloc>
void DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, UpdateOp, void,
            Allocator>>::updateImpl_(KeyT begin, KeyT end, KeyT currBegin,
                                     KeyT currEnd, Node_* currNode,
                                     NodeAlloc& allocator) {
  if (begin >= currEnd || currBegin >= end) {
    return;
  }
  if (end >= currEnd && begin <= currBegin) {
    currNode->update(updateOp_, allocator);
    return;
  }
  if (currNode->isLeaf()) {
    currNode->initChildren(allocator);
  }
  currNode->siftOptUpdate(updateOp_, allocator);
  const auto mid = (currBegin + currEnd) / 2;
  if (mid >= currBegin + 1) {
    auto* const leftNodePtr = currNode->getLeft();
    updateImpl_(begin, end, currBegin, mid, leftNodePtr, allocator);
  }
  if (currEnd >= mid + 1) {
    auto* const rightNodePtr = currNode->getRight();
    updateImpl_(begin, end, mid, currEnd, rightNodePtr, allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class KeyT, class ValueT,
          class GetValueT, class SegGetComb, class SegGetInit, class Allocator>
class DynamicSegmentTreeUpdateVariationBase<
    Derived<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit, NoUpdateOp, void,
            Allocator>> {
 protected:
  using Node_ = Node<ValueT, void, Allocator>;
  using NodeAlloc_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Node_>;

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(NoUpdateOp){};

 protected:
  void optionalSiftNodeUpdate_(Node_*, NodeAlloc_&) const {
  }
};

}  // namespace dst::impl

#endif  // DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP
