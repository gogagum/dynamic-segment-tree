#ifndef LAZY_SEGMENT_TREE_HPP
#define LAZY_SEGMENT_TREE_HPP

#include <concepts>
#include <functional>

#include <node.hpp>

namespace {

////////////////////////////////////////////////////////////////////////////////
/// \brief The LazySegmentTree class
///
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class ValValGetOp = std::plus<ValueT>,
          class ValKeyGetOp = std::multiplies<void>,
          class Allocator = std::allocator<ValueT>>
class LazySegmentTree{
private:
    using _Node = Node<ValueT, ValValUpdateOp, ValValGetOp, Allocator>;
public:
    LazySegmentTree(KeyT start, KeyT end, const ValueT& value);
    void update(KeyT begin, KeyT end, const ValueT& toUpdate);
    void set(KeyT begin, KeyT end, const ValueT& toSet);
    ValueT get(KeyT key) const;
    ValueT rangeGet(KeyT begin, KeyT end) const;
private:
    void _updateImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    void _setImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    ValueT _getImpl(KeyT key, KeyT currBegin, KeyT currEnd,
                    _Node* currNode) const;
    ValueT _rangeGetImpl(KeyT begin, KeyT end,
                         KeyT currBegin, KeyT currEnd,
                         _Node* currNode) const;
private:
    const ValKeyGetOp _valKeyGetOp{};
    const ValValGetOp _valValGetOp{};
    mutable _Node _rootNode;
    KeyT _begin;
    KeyT _end;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                ValValGetOp,  ValKeyGetOp, Allocator>::LazySegmentTree(
        KeyT begin, KeyT end, const ValueT& value)
    : _rootNode(value), _begin(begin), _end(end) {}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
void LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                     ValValGetOp,  ValKeyGetOp, Allocator>::update(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    _updateImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
void LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                     ValValGetOp,  ValKeyGetOp, Allocator>::set(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    _setImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
ValueT LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                       ValValGetOp,  ValKeyGetOp, Allocator>::get(
        KeyT key) const {
    if (key >= _end || key < _begin) {
        return ValueT{};
    }
    return _getImpl(key, _begin, _end, &_rootNode);
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
ValueT LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                       ValValGetOp,  ValKeyGetOp, Allocator>::rangeGet(
        KeyT begin, KeyT end) const {
    return _rangeGetImpl(begin, end, _begin, _end, &_rootNode);
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
void LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                     ValValGetOp,  ValKeyGetOp, Allocator>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const ValueT& toUpdate) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->update(toUpdate);
        return;
    }
    const auto m = (currBegin + currEnd) / 2;
    currNode->siftDown();
    if (m >= currBegin + 1) {
        auto leftNodePtr = currNode->getLeft();
        _updateImpl(begin, end, currBegin, m, leftNodePtr, toUpdate);
    }
    if (currEnd >= begin + 1) {
        auto rightNodePtr = currNode->getRight();
        _updateImpl(begin, end, m, currEnd, rightNodePtr, toUpdate);
    }
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
void LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                     ValValGetOp,  ValKeyGetOp, Allocator>::_setImpl(
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
    currNode->siftDown();
    if (m >= currBegin + 1) {
        auto leftNodePtr = currNode->getLeft();
        _setImpl(begin, end, currBegin, m, leftNodePtr, toUpdate);
    }
    if (currEnd >= begin + 1) {
        auto rightNodePtr = currNode->getRight();
        _setImpl(begin, end, m, currEnd, rightNodePtr, toUpdate);
    }
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
ValueT LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                       ValValGetOp,  ValKeyGetOp, Allocator>::_getImpl(
        KeyT key, KeyT currBegin, KeyT currEnd,
        _Node* currNode) const {
    if (currNode->hasValue()) {
        assert(currNode->isLeaf() && "Only leafs have values.");
        return currNode->getValue();
    }
    currNode->siftDown();
    if (auto m = (currBegin + currEnd) / 2; key >= m) {
        _Node* visitedNode = currNode->getRight();
        return _getImpl(key, m, currEnd, visitedNode);
    } else {
        _Node* visitedNode = currNode->getLeft();
        return _getImpl(key, currBegin, m, visitedNode);
    }
}

//----------------------------------------------------------------------------//
template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp,
          class ValValGetOp,
          class ValKeyGetOp,
          class Allocator>
ValueT
LazySegmentTree<KeyT, ValueT, ValValUpdateOp,
                ValValGetOp,  ValKeyGetOp, Allocator>::_rangeGetImpl(
        KeyT begin, KeyT end, KeyT currBegin,
        KeyT currEnd, _Node* currNode) const {
    if (begin >= currEnd || currBegin >= end) {
        return ValueT{};
    }
    if (end >= currEnd && begin <= currBegin && currNode->isLeaf()) {
        return _valKeyGetOp(currNode->getValue(), currEnd - currBegin);
    }
    currNode->siftDown();
    const auto m = (currBegin + currEnd) / 2;
    _Node* leftNodePtr = currNode->getLeft();
    const bool leftNodeExists = (m >= currBegin + 1);
    const ValueT lVal = leftNodeExists
            ? _rangeGetImpl(begin, end, currBegin, m, leftNodePtr)
            : ValueT{};
    _Node* rightNodePtr = currNode->getRight();
    const bool rightNodeExists = (currEnd >= m + 1);
    const ValueT rVal = rightNodeExists
            ? _rangeGetImpl(begin, end, m, currEnd, rightNodePtr)
            : ValueT{};
    return _valValGetOp(lVal, rVal);
}


}

#endif // LAZY_SEGMENT_TREE_HPP
