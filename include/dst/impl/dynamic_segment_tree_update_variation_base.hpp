////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
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
template <class KeyT, class ValueT, class UpdateOp, class UpdateArgT,
          class Allocator>
class DynamicSegmentTreeUpdateVariationBase;

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, class UpdateOp, class UpdateArgT,
          class Allocator>
  requires conc::TwoArgsUpdateOp<UpdateOp, ValueT, UpdateArgT>
class DynamicSegmentTreeUpdateVariationBase<KeyT, ValueT, UpdateOp, UpdateArgT,
                                            Allocator> {
 protected:
  using Node_ = Node<ValueT, std::optional<UpdateArgT>, Allocator>;

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
      : updateOp_(updateOp) {
  }

 protected:

  // TODO(gogagum): separate definition and implementation
  // Out-of-line definitions don't work on clang 15, 16.
  // This bug will be fixed in clang 17, and when this version will be released,
  // code here should be edited.
  void updateImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                   Node_* currNode, const UpdateArgT& toUpdate) {
    if (begin >= currEnd || currBegin >= end) {
      return;
    }
    if (end >= currEnd && begin <= currBegin) {
      currNode->update(updateOp_, toUpdate);
      return;
    }
    if (currNode->isLeaf()) {
      currNode->initChildren();
    }
    if constexpr (conc::UpdateOp<UpdateOp, ValueT, UpdateArgT>) {
      currNode->siftOptUpdate(updateOp_);
    }
    assert(currEnd >= currBegin + 2);
    const auto mid = (currBegin + currEnd) / 2;
    updateImpl_(begin, end, currBegin, mid, currNode->getLeft(), toUpdate);
    updateImpl_(begin, end, mid, currEnd, currNode->getRight(), toUpdate);
  }

  void optionalSiftNodeUpdate_(Node_* nodePtr) const {
    nodePtr->siftOptUpdate(updateOp_);
  }

 private:
  UpdateOp updateOp_;
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, conc::OneArgUpdateOp<ValueT> UpdateOp,
          class Allocator>
class DynamicSegmentTreeUpdateVariationBase<KeyT, ValueT, UpdateOp, void,
                                            Allocator> {
 protected:
  using Node_ = Node<ValueT, bool, Allocator>;

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
      : updateOp_(updateOp) {
  }

 protected:
  void updateImpl_(KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
                   Node_* currNode);
  void optionalSiftNodeUpdate_(Node_* nodePtr) const {
    nodePtr->siftOptUpdate(updateOp_);
  }

 private:
  UpdateOp updateOp_;
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, conc::OneArgUpdateOp<ValueT> UpdateOp,
          class Allocator>
void DynamicSegmentTreeUpdateVariationBase<
    KeyT, ValueT, UpdateOp, void, Allocator>::updateImpl_(KeyT begin, KeyT end,
                                                          KeyT currBegin,
                                                          KeyT currEnd,
                                                          Node_* currNode) {
  if (begin >= currEnd || currBegin >= end) {
    return;
  }
  if (end >= currEnd && begin <= currBegin) {
    currNode->update(updateOp_);
    return;
  }
  const auto mid = (currBegin + currEnd) / 2;
  if (currNode->isLeaf()) {
    currNode->initChildren();
  }
  currNode->siftOptUpdate(updateOp_);
  if (mid >= currBegin + 1) {
    auto leftNodePtr = currNode->getLeft();
    updateImpl_(begin, end, currBegin, mid, leftNodePtr);
  }
  if (currEnd >= mid + 1) {
    auto rightNodePtr = currNode->getRight();
    updateImpl_(begin, end, mid, currEnd, rightNodePtr);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, class Allocator>
class DynamicSegmentTreeUpdateVariationBase<KeyT, ValueT, NoUpdateOp, void,
                                            Allocator> {
 protected:
  using Node_ = Node<ValueT, void, Allocator>;

 protected:
  explicit DynamicSegmentTreeUpdateVariationBase(NoUpdateOp){};

 protected:
  void optionalSiftNodeUpdate_([[maybe_unused]] Node_* nodePtr) const {
  }
};

}  // namespace dst::impl

#endif  // DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP
