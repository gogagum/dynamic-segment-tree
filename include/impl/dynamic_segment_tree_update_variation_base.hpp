#ifndef DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP
#define DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP

#include "node.hpp"
#include "concepts.hpp"
#include "disable_operations.hpp"

namespace dst::impl {

template <class ValueT, class UpdateOp, class UpdateArgT, class Allocator>
class DynamicSegmentTreeUpdateVariationBase;

template <class ValueT,
          class UpdateArgT,
          conc::TwoArgsUpdateOp<ValueT, UpdateArgT> UpdateOp,
          class Allocator>
class DynamicSegmentTreeUpdateVariationBase<ValueT,
                                            UpdateOp,
                                            UpdateArgT,
                                            Allocator>{
protected:
    using _Node = Node<ValueT, std::optional<UpdateArgT>, Allocator>;
protected:
    DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
        : _updateOp(updateOp) {}
    template <class KeyT>
    void _updateImpl(
        KeyT begin, KeyT end, KeyT currStart, KeyT currEnd, _Node* currNode,
        const UpdateArgT& toUpdate);
    void _optionalSiftNodeUpdate(_Node* nodePtr) const {
        nodePtr->siftOptUpdate(_updateOp);
    }

protected:
    UpdateOp _updateOp;
};

template <class ValueT,
          class UpdateArgT,
          conc::TwoArgsUpdateOp<ValueT, UpdateArgT> UpdateOp,
          class Allocator>
template <class KeyT>
void
DynamicSegmentTreeUpdateVariationBase<ValueT, UpdateOp,
                                      UpdateArgT, Allocator>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const UpdateArgT& toUpdate) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->update(this->_updateOp, toUpdate);
        return;
    }
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    if constexpr (conc::UpdateOp<UpdateOp, ValueT, UpdateArgT>) {
        currNode->siftOptUpdate(this->_updateOp);
    }
    assert(currEnd >= currBegin + 2);
    const auto m = (currBegin + currEnd) / 2;
    _updateImpl(begin, end, currBegin, m, currNode->getLeft(), toUpdate);
    _updateImpl(begin, end, m, currEnd, currNode->getRight(), toUpdate);
}

template <class ValueT, conc::OneArgUpdateOp<ValueT> UpdateOp, class Allocator>
class DynamicSegmentTreeUpdateVariationBase<ValueT, UpdateOp, void, Allocator>{
protected:
    using _Node = Node<ValueT, bool, Allocator>;
protected:
    DynamicSegmentTreeUpdateVariationBase(const UpdateOp& updateOp)
        : _updateOp(updateOp) {}
    template <class KeyT>
    void _updateImpl(
            KeyT start, KeyT end, KeyT currStart,
            KeyT currEnd, _Node* currNode);
    void _optionalSiftNodeUpdate(_Node* nodePtr) const {
        nodePtr->siftOptUpdate(_updateOp);
    }
protected:
    UpdateOp _updateOp;
};

template <class ValueT, conc::OneArgUpdateOp<ValueT> UpdateOp, class Allocator>
template <class KeyT>
void DynamicSegmentTreeUpdateVariationBase<ValueT, UpdateOp,
                                           void, Allocator>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->update(this->_updateOp);
        return;
    }
    const auto m = (currBegin + currEnd) / 2;
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    currNode->siftOptUpdate(this->_updateOp);
    if (m >= currBegin + 1) {
        auto leftNodePtr = currNode->getLeft();
        _updateImpl(begin, end, currBegin, m, leftNodePtr);
    }
    if (currEnd >= m + 1) {
        auto rightNodePtr = currNode->getRight();
        _updateImpl(begin, end, m, currEnd, rightNodePtr);
    }
}

template <class ValueT, class Allocator>
class DynamicSegmentTreeUpdateVariationBase<ValueT, NoUpdateOp,
                                            void, Allocator>{
protected:
    using _Node = Node<ValueT, void, Allocator>;
protected:
    DynamicSegmentTreeUpdateVariationBase(NoUpdateOp) {};
    void _optionalSiftNodeUpdate(_Node* nodePtr) const { }
};



}

#endif // DYNAMIC_SEGMENT_TREE_UPDATE_VARIATION_BASE_HPP
