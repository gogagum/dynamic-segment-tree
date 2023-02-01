#ifndef DYNAMIC_SEGMENT_TREE_HPP
#define DYNAMIC_SEGMENT_TREE_HPP

#include <concepts>
#include <functional>

#include <node.hpp>
#include <concepts.hpp>

namespace dst{

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator = std::allocator<ValueT>>
class DynamicSegmentTree{
private:
    using _Node = Node<ValueT, Allocator>;

public:
    DynamicSegmentTree(KeyT start, KeyT end, const ValueT& value);
    void update(
            KeyT begin, KeyT end,
            const ValueT& toUpdate) requires (!std::is_same_v<UpdateOp, void>);
    void set(KeyT begin, KeyT end, const ValueT& toSet);
    ValueT get(KeyT key) const;
    ValueT rangeGet(KeyT begin, KeyT end) const;
private:
    void _updateImpl(
            KeyT start, KeyT end, KeyT currStart, KeyT currEnd, _Node* currNode,
            const ValueT& toUpdate) requires (!std::is_same_v<UpdateOp, void>);
    void _setImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    ValueT _getImpl(KeyT key, KeyT currBegin, KeyT currEnd,
                    _Node* currNode) const;
    ValueT _rangeGetImpl(KeyT begin, KeyT end,
                         KeyT currBegin, KeyT currEnd,
                         _Node* currNode) const;
    void _siftNodeDown(_Node* node) const;
    void _updateNode(
            _Node* node,
            const ValueT& updateValue) const requires (!std::is_same_v<UpdateOp, void>);
private:
    const SegCombiner _segCombiner{};
    const SegInitializer _segInitializer{};
    mutable _Node _rootNode;
    KeyT _begin;
    KeyT _end;
};

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, Allocator>::DynamicSegmentTree(
        KeyT begin, KeyT end, const ValueT& value)
    : _rootNode(value), _begin(begin), _end(end) {}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, Allocator>::update(
        KeyT begin, KeyT end, const ValueT& toUpdate) requires (!std::is_same_v<UpdateOp, void>) {
    _updateImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, Allocator>::set(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    _setImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, Allocator>::get(KeyT key) const {
    if (key >= _end || key < _begin) {
        return ValueT{};
    }
    return _getImpl(key, _begin, _end, &_rootNode);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                          SegInitializer, UpdateOp, Allocator>::rangeGet(
        KeyT begin, KeyT end) const {
    return _rangeGetImpl(begin, end, _begin, _end, &_rootNode);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, Allocator>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const ValueT& toUpdate) requires (!std::is_same_v<UpdateOp, void>) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        _updateNode(currNode, toUpdate);
        return;
    }
    const auto m = (currBegin + currEnd) / 2;
    _siftNodeDown(currNode);
    if (m >= currBegin + 1) {
        auto leftNodePtr = currNode->getLeft();
        _updateImpl(begin, end, currBegin, m, leftNodePtr, toUpdate);
    }
    if (currEnd >= m + 1) {
        auto rightNodePtr = currNode->getRight();
        _updateImpl(begin, end, m, currEnd, rightNodePtr, toUpdate);
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, Allocator>::_setImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const ValueT& toUpdate) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->setValue(toUpdate);
        return;
    }
    const auto m = (currBegin + currEnd) / 2;
    _siftNodeDown(currNode);
    if (m >= currBegin + 1) {
        auto leftNodePtr = currNode->getLeft();
        _setImpl(begin, end, currBegin, m, leftNodePtr, toUpdate);
    }
    if (currEnd >= begin + 1) {
        auto rightNodePtr = currNode->getRight();
        _setImpl(begin, end, m, currEnd, rightNodePtr, toUpdate);
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                          SegInitializer, UpdateOp, Allocator>::_getImpl(
        KeyT key, KeyT currBegin, KeyT currEnd,
        _Node* currNode) const {
    if (currNode->isLeaf()) {
        return currNode->getValue();
    }
    _siftNodeDown(currNode);
    if (auto m = (currBegin + currEnd) / 2; key >= m) {
        _Node* visitedNode = currNode->getRight();
        return _getImpl(key, m, currEnd, visitedNode);
    } else {
        _Node* visitedNode = currNode->getLeft();
        return _getImpl(key, currBegin, m, visitedNode);
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, Allocator>::_rangeGetImpl(
        KeyT begin, KeyT end, KeyT currBegin,
        KeyT currEnd, _Node* currNode) const {
    if (begin >= currEnd || currBegin >= end) {
        assert(false);
        return ValueT{};
    }
    if (end >= currEnd && begin <= currBegin && currNode->isLeaf()) {
        if constexpr (conc::ValueSegmentInitializer<SegInitializer, ValueT, GetValueT>) {
            return _segInitializer(currNode->getValue());
        } else {
            return _segInitializer(currNode->getValue(), currEnd - currBegin);
        }
    }
    _siftNodeDown(currNode);
    const auto m = (currBegin + currEnd) / 2;

    _Node* rightNodePtr = currNode->getRight();
    _Node* leftNodePtr = currNode->getLeft();

    if (begin >= m) { // only right
        return _rangeGetImpl(begin, end, m, currEnd, rightNodePtr);
    }
    if (end <= m) { // only left
        return _rangeGetImpl(begin, end, currBegin, m, leftNodePtr);
    }

    const ValueT lVal = _rangeGetImpl(begin, end, currBegin, m, leftNodePtr);
    const ValueT rVal = _rangeGetImpl(begin, end, m, currEnd, rightNodePtr);

    if constexpr (conc::ValueAndLengthCombiner<SegCombiner, ValueT, KeyT>) {
        const KeyT leftLength = m - begin;
        const KeyT rightLength = end - m;
        return _segCombiner(lVal, rVal, leftLength, rightLength);
    } else if constexpr (conc::ValueSegmentCombiner<SegCombiner, ValueT>) {
        return _segCombiner(lVal, rVal);
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, Allocator>::_siftNodeDown(
        _Node* node) const {
    if (node->isLeaf()) {
        node->initChildren();
    } else {
        if constexpr (std::is_same_v<UpdateOp, void>) {
            assert(!node->hasValue() && "Non leaf node can not have value while there are no update operations.");
        } else {
            if (node->hasValue()) {
                _updateNode(node->getLeft(), node->getValue());
                _updateNode(node->getRight(), node->getValue());
                node->setNullValue();
            }
        }
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class Allocator>
void
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, Allocator>::_updateNode(
        _Node* node, const ValueT& updateValue) const requires (!std::is_same_v<UpdateOp, void>) {
    if (!node->isLeaf()) {
        // _value means delayed update.
        if (node->hasValue()) {
            _updateNode(node->getLeft(), node->getValue());  // update left with old update
            _updateNode(node->getRight(), node->getValue()); // update right with old update
        }
        // _value continues to have delayed update meaning.
        node->setUpdateValue(updateValue);
    } else { // isLeaf()
        UpdateOp updateOp;
        assert(node->hasValue() && "Leaf must have a value.");
        node->setValue(updateOp(node->getValue(), updateValue));
    }
}

}

#endif // DYNAMIC_SEGMENT_TREE_HPP
