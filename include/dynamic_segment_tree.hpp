#ifndef DYNAMIC_SEGMENT_TREE_HPP
#define DYNAMIC_SEGMENT_TREE_HPP

#include <concepts>
#include <functional>

#include <node.hpp>
#include <concepts.hpp>

namespace dst{

template <class ValueT, class UpdateOp>
struct DefaultUpdateArg {
    using Type = ValueT;
};

template <class ValueT>
struct DefaultUpdateArg<ValueT, void> {
    using Type = void;
};

template <class ValueT, class UpdateOp>
using DefaultUpdateArgT = typename DefaultUpdateArg<ValueT, UpdateOp>::Type;

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp = void,
          class UpdateArgT = DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
class DynamicSegmentTree {
private:
    using _Node = Node<ValueT, UpdateArgT, Allocator>;

public:
    DynamicSegmentTree(KeyT start, KeyT end, const ValueT& value = ValueT{});
    void update(KeyT begin, KeyT end, const auto& toUpdate);
    void set(KeyT begin, KeyT end, const ValueT& toSet);
    ValueT get(KeyT key) const;
    ValueT rangeGet(KeyT begin, KeyT end) const;
private:
    void _updateImpl(
            KeyT start, KeyT end, KeyT currStart, KeyT currEnd, _Node* currNode,
            const UpdateArgT* toUpdate) requires (!std::is_same_v<UpdateArgT, void>);
    void _setImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    ValueT _getImpl(KeyT key, KeyT currBegin, KeyT currEnd,
                    _Node* currNode) const;
    ValueT _rangeGetImpl(KeyT begin, KeyT end,
                         KeyT currBegin, KeyT currEnd,
                         _Node* currNode) const;
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
          class UpdateArgT,
          class Allocator>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, UpdateArgT,
                   Allocator>::DynamicSegmentTree(
        KeyT begin, KeyT end, const ValueT& value)
    : _rootNode(value), _begin(begin), _end(end) {}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, UpdateArgT, Allocator>::update(
        KeyT begin, KeyT end, const auto& toUpdate) {
    _updateImpl(begin, end, _begin, _end, &_rootNode,
                &static_cast<const UpdateArgT&>(toUpdate));
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, UpdateArgT, Allocator>::set(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    _setImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, UpdateArgT, Allocator>::get(KeyT key) const {
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
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                          SegInitializer, UpdateOp, UpdateArgT, Allocator>::rangeGet(
        KeyT begin, KeyT end) const {
    return _rangeGetImpl(begin, end, _begin, _end, &_rootNode);
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, UpdateArgT, Allocator>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const UpdateArgT* toUpdate) requires (!std::is_same_v<UpdateArgT, void>) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->update(UpdateOp(), *toUpdate);
        return;
    }
    const auto m = (currBegin + currEnd) / 2;
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    if constexpr (!std::is_same_v<UpdateOp, void>) {
        currNode->siftOptUpdate(UpdateOp());
    }
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
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                        SegInitializer, UpdateOp, UpdateArgT, Allocator>::_setImpl(
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
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    if constexpr (!std::is_same_v<UpdateOp, void>) {
        if (currNode->hasUpdateValue()) {
            currNode->siftUpdate(UpdateOp());
        }
    }
    if (m >= currBegin + 1) {
        _setImpl(begin, end, currBegin, m, currNode->getLeft(), toUpdate);
    }
    if (currEnd >= begin + 1) {
        _setImpl(begin, end, m, currEnd, currNode->getRight(), toUpdate);
    }
}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                          SegInitializer, UpdateOp, UpdateArgT, Allocator>::_getImpl(
        KeyT key, KeyT currBegin, KeyT currEnd,
        _Node* currNode) const {
    if (currNode->isLeaf()) {
        return currNode->getValue();
    }
    if constexpr (!std::is_same_v<UpdateOp, void>) {
        currNode->siftOptUpdate(UpdateOp());
    }
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
          class UpdateArgT,
          class Allocator>
ValueT
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner,
                   SegInitializer, UpdateOp, UpdateArgT, Allocator>::_rangeGetImpl(
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
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    if constexpr (!std::is_same_v<UpdateOp, void>) {
        currNode->siftOptUpdate(UpdateOp());
    }
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

}

#endif // DYNAMIC_SEGMENT_TREE_HPP
