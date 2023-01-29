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
          class ValValOp = std::plus<ValueT>,
          class ValKeyOp = std::multiplies<void>>
class LazySegmentTree{
private:
    using _Node = Node<ValueT, ValValOp>;
public:
    LazySegmentTree(KeyT start, KeyT end, const ValueT& value);
    void update(KeyT start, KeyT end, ValueT&& toUpdate);
    ValueT get(KeyT key) const;
    ValueT rangeGet(KeyT begin, KeyT end) const;
private:
    void _updateImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    ValueT _getImpl(KeyT key, KeyT currBegin, KeyT currEnd,
                    _Node* currNode, const ValueT& currValue) const;
    ValueT _rangeGetImpl(KeyT begin, KeyT end,
                         KeyT currBegin, KeyT currEnd,
                         _Node* currNode) const;
private:
    const ValValOp _valValOp{};
    const ValKeyOp _valKeyOp{};
    mutable _Node _rootNode;
    KeyT _begin;
    KeyT _end;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::LazySegmentTree(
        KeyT begin, KeyT end, const ValueT& value)
    : _rootNode(value), _begin(begin), _end(end) {}

//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
void LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::update(
        KeyT begin, KeyT end, ValueT&& toUpdate) {
    _updateImpl(begin, end, _begin, _end, &_rootNode,
                std::forward<ValueT>(toUpdate));
}

//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
ValueT LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::get(KeyT key) const {
    if (key >= _end || key < _begin) {
        return ValueT{};
    }
    return _getImpl(key, _begin, _end, &_rootNode, ValueT{});
}

//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
ValueT LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::rangeGet(
        KeyT begin, KeyT end) const {
    return _rangeGetImpl(begin, end, _begin, _end, &_rootNode);
}

//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
void LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::_updateImpl(
        KeyT begin, KeyT end, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const ValueT& toUpdate) {
    if (begin >= currEnd || currBegin >= end) {
        return;
    }
    if (end >= currEnd && begin <= currBegin) {
        currNode->apply(toUpdate);
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
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
ValueT
LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::_getImpl(
        KeyT key, KeyT currBegin, KeyT currEnd,
        _Node* currNode, const ValueT& currValue) const {
    if (currNode->isList()) {
        return _valValOp(currValue, currNode->getValue());
    }
    currNode->siftDown();
    if (auto m = (currBegin + currEnd) / 2; key >= m) {
        _Node* visitedNode = currNode->getRight();
        return _getImpl(key, m, currEnd, visitedNode, currValue);
    } else {
        _Node* visitedNode = currNode->getLeft();
        return _getImpl(key, currBegin, m, visitedNode, currValue);
    }
}

//----------------------------------------------------------------------------//
template <std::integral KeyT, class ValueT, class ValValOp, class ValKeyOp>
ValueT
LazySegmentTree<KeyT, ValueT, ValValOp, ValKeyOp>::_rangeGetImpl(
        KeyT begin, KeyT end, KeyT currBegin,
        KeyT currEnd, _Node* currNode) const {
    if (begin >= currEnd || currBegin >= end) {
        return ValueT{};
    }
    if (end >= currEnd && begin <= currBegin && currNode->isList()) {
        return _valKeyOp(currNode->getValue(), end - begin);
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
    return _valValOp(lVal, rVal);
}


}

#endif // LAZY_SEGMENT_TREE_HPP
