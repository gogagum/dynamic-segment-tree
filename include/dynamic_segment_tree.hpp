#ifndef DYNAMIC_SEGMENT_TREE_HPP
#define DYNAMIC_SEGMENT_TREE_HPP


#include <concepts>
#include <functional>

#include <node.hpp>
#include <concepts.hpp>
#include <dynamic_segment_tree_update_variation_base.hpp>
#include <dynamic_segment_tree_range_get_variation_base.hpp>
#include <mpimpl.hpp>

namespace dst{

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegGetComb,
          class SegGetInit,
          class UpdateOp = void,
          class UpdateArgT = impl::DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
class DynamicSegmentTree
        : protected impl::DynamicSegmentTreeUpdateVariationBase<
            ValueT, UpdateOp, UpdateArgT, Allocator
        >,
        protected impl::DynamicSegmentTreeRangeGetCombineVariationBase<
            KeyT, GetValueT, SegGetComb
        >,
        protected impl::DynamicSegmentTreeRangeGetInitVariationBase<
            KeyT, ValueT, GetValueT, SegGetInit
        >{
private:
    using _UpdateVariationBase =
        impl::DynamicSegmentTreeUpdateVariationBase<
            ValueT, UpdateOp, UpdateArgT, Allocator
        >;

    using _RangeGetCombineVariationBase =
        impl::DynamicSegmentTreeRangeGetCombineVariationBase<
            KeyT, GetValueT, SegGetComb
        >;

    using _RangeGetInitVariationBase =
        impl::DynamicSegmentTreeRangeGetInitVariationBase<
            KeyT, ValueT, GetValueT, SegGetInit
        >;

    using _Node = typename _UpdateVariationBase::_Node;
public:

    /**
     *  @param begin - beginning of a working area.
     *  @param end - ending of a working area  (not included).
     *  @param alloc - allocator.
     */
    template <class = void> requires std::is_same_v<SegGetComb, void>
                                && std::is_same_v<SegGetInit, void>
    DynamicSegmentTree(KeyT begin,
                       KeyT end,
                       const ValueT& value = ValueT{},
                       const Allocator& alloc = Allocator());

    /**
     *  @param begin - beginning of a working area.
     *  @param end - ending of a working area  (not included).
     *  @param segGetComb - functor called when two segments are combined in
     *  range get operation.
     *  @param segGetInit - functor called for get result initialization on an
     *  equally filled segment.
     *  @param alloc - allocator.
     */
    template <class = void> requires conc::SegmentCombiner<SegGetComb, GetValueT, KeyT>
                                && conc::SegmentInitializer<SegGetInit, ValueT, KeyT, GetValueT>
                                && std::is_same_v<UpdateOp, void>
    DynamicSegmentTree(KeyT begin,
                       KeyT end,
                       const ValueT& value = ValueT{},
                       const SegGetComb& segGetComb = SegGetComb(),
                       const SegGetInit& seGetInit = SegGetInit(),
                       const Allocator& alloc = Allocator());

    /**
     *  @param begin - beginning of a working area.
     *  @param end - ending of a working area (not included).
     *  @param segGetComb - functor called when two segments are combined in
     *  range get operation.
     *  @param segGetInit - functor called for get result initialization on an
     *  equally filled segment.
     *  @param updateOp - update operation.
     *  @param alloc - allocator.
     */
    template <class _UpdateOp = UpdateOp>
        requires conc::SegmentCombiner<SegGetComb, GetValueT, KeyT>
            && conc::SegmentInitializer<SegGetInit, ValueT, KeyT, GetValueT>
            && conc::UpdateOp<_UpdateOp, ValueT, UpdateArgT>
    DynamicSegmentTree(KeyT begin,
                       KeyT end,
                       const ValueT& value = ValueT(),
                       const SegGetComb& segGetComb = SegGetComb(),
                       const SegGetInit& segGetInit = SegGetInit(),
                       const _UpdateOp& updateOp = UpdateOp(),
                       const Allocator& alloc = Allocator());

    /**
     * @brief update - apply one argument operation on a range.
     * @param begin - beginning of an updated segment.
     * @param end - ending of an updated segment (not included).
     * @param toUpdate - argument for update operation.
     */
    template <class _UpdateArg> requires conc::TwoArgsUpdateOp<UpdateOp,
                                                               ValueT,
                                                               UpdateArgT>
    void update(KeyT begin,
                KeyT end,
                const _UpdateArg& toUpdate);
    /**
     * @brief update - apply no arguments update operation on a range.
     * @param begin - beginning of an updated segment.
     * @param end - ending of an updated segment (not included).
     */
    void update(KeyT begin,
                KeyT end) requires conc::OneArgUpdateOp<UpdateOp, ValueT>;

    /**
     * @brief set - set value on a range.
     * @param begin - beginning of an updated segment.
     * @param end - ending of an updated segment (not included).
     * @param toSet - value to set.
     */
    void set(KeyT begin, KeyT end, const ValueT& toSet);

    /**
     * @brief get - get value by index.
     * @param key - index.
     * @return value in index.
     */
    ValueT get(KeyT key) const;

    /**
     * @brief rangeGet - get result on a range.
     * @param begin - beginning of a range.
     * @param end - ending of a range.
     * @return - range get operation result.
     */
    ValueT rangeGet(KeyT begin, KeyT end) const;

private:
    void _setImpl(KeyT start, KeyT end, KeyT currStart, KeyT currEnd,
                     _Node* currNode, const ValueT& toUpdate);
    ValueT _getImpl(KeyT key, KeyT currBegin, KeyT currEnd,
                    _Node* currNode) const;
    ValueT _rangeGetImpl(KeyT begin, KeyT end,
                         KeyT currBegin, KeyT currEnd,
                         _Node* currNode) const requires conc::SegmentCombiner<SegGetComb, GetValueT, KeyT>
                                                      && conc::SegmentInitializer<SegGetInit, ValueT, KeyT, GetValueT>;
private:
    mutable _Node _rootNode;
    const KeyT _begin;
    const KeyT _end;
};

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegGetComb,
          class SegGetInit,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
template <class> requires conc::SegmentCombiner<SegGetComb, GetValueT, KeyT>
                            && conc::SegmentInitializer<SegGetInit, ValueT, KeyT, GetValueT>
                            && std::is_same_v<UpdateOp, void>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                   UpdateOp, UpdateArgT, Allocator>::DynamicSegmentTree(
        KeyT begin,
        KeyT end,
        const ValueT& value,
        const SegGetComb& segCombiner,
        const SegGetInit& segInitializer,
        const Allocator& alloc)
    : _rootNode(value),
      _begin(begin),
      _end(end),
      _RangeGetCombineVariationBase(segCombiner),
      _RangeGetInitVariationBase(segInitializer),
      _UpdateVariationBase() {}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegGetComb,
          class SegGetInit,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
template <class _UpdateOp>
    requires conc::SegmentCombiner<SegGetComb, GetValueT, KeyT>
          && conc::SegmentInitializer<SegGetInit, ValueT, KeyT, GetValueT>
          && conc::UpdateOp<_UpdateOp, ValueT, UpdateArgT>
DynamicSegmentTree<KeyT, ValueT, GetValueT, SegGetComb, SegGetInit,
                   UpdateOp, UpdateArgT, Allocator>::DynamicSegmentTree(
        KeyT begin,
        KeyT end,
        const ValueT& value,
        const SegGetComb& segCombiner,
        const SegGetInit& segInitializer,
        const _UpdateOp& updateOp,
        const Allocator& alloc)
    : _rootNode(value),
      _begin(begin),
      _end(end),
      _RangeGetInitVariationBase(segInitializer),
      _RangeGetCombineVariationBase(segCombiner),
      _UpdateVariationBase(updateOp) {}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
template <class _UpdateArgT> requires conc::TwoArgsUpdateOp<UpdateOp,
                                                            ValueT,
                                                            UpdateArgT>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                        UpdateOp, UpdateArgT, Allocator>::update(
        KeyT begin,
        KeyT end,
        const _UpdateArgT& toUpdate)  {
    this->_updateImpl(begin, end, _begin, _end, &_rootNode,
                &static_cast<const UpdateArgT&>(toUpdate));
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                        UpdateOp, UpdateArgT, Allocator>::update(
        KeyT begin, KeyT end) requires conc::OneArgUpdateOp<UpdateOp, ValueT> {
    this->_updateImpl(begin, end, _begin, _end, &_rootNode);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                        UpdateOp, UpdateArgT, Allocator>::set(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    _setImpl(begin, end, _begin, _end, &_rootNode, toUpdate);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                          UpdateOp, UpdateArgT, Allocator>::get(
        KeyT key) const {
    if (key >= _end || key < _begin) {
        return ValueT{};
    }
    return _getImpl(key, _begin, _end, &_rootNode);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                          UpdateOp, UpdateArgT, Allocator>::rangeGet(
        KeyT begin, KeyT end) const {
    return _rangeGetImpl(begin, end, _begin, _end, &_rootNode);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
void DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                        UpdateOp, UpdateArgT, Allocator>::_setImpl(
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
    this->_optionalSiftNodeUpdate(currNode);
    _setImpl(begin, end, currBegin, m, currNode->getLeft(), toUpdate);
    _setImpl(begin, end, m, currEnd, currNode->getRight(), toUpdate);
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                          UpdateOp, UpdateArgT, Allocator>::_getImpl(
        KeyT key, KeyT currBegin, KeyT currEnd,
        _Node* currNode) const {
    if (currNode->isLeaf()) {
        return currNode->getValue();
    }
    this->_optionalSiftNodeUpdate(currNode);
    if (auto m = (currBegin + currEnd) / 2; key >= m) {
        _Node* visitedNode = currNode->getRight();
        return _getImpl(key, m, currEnd, visitedNode);
    } else {
        _Node* visitedNode = currNode->getLeft();
        return _getImpl(key, currBegin, m, visitedNode);
    }
}

////////////////////////////////////////////////////////////////////////////////
template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner,
          class SegInitializer,
          class UpdateOp,
          class UpdateArgT,
          class Allocator>
ValueT DynamicSegmentTree<KeyT, ValueT, GetValueT, SegCombiner, SegInitializer,
                          UpdateOp, UpdateArgT, Allocator>::_rangeGetImpl(
        KeyT begin, KeyT end, KeyT currBegin,
        KeyT currEnd, _Node* currNode) const requires conc::SegmentCombiner<SegCombiner, GetValueT, KeyT>
                                                   && conc::SegmentInitializer<SegInitializer, ValueT, KeyT, GetValueT> {
    if (begin > currEnd || currBegin > end) {
        assert(false &&
               "_rangeGetImpl must not be called out of initial get range.");
        return ValueT{};
    }
    if (end >= currEnd && begin <= currBegin && currNode->isLeaf()) {
        return _RangeGetInitVariationBase::_initGet(
                    currBegin, currEnd, currNode->getValue());
    }
    if (currNode->isLeaf()) {
        currNode->initChildren();
    }
    _UpdateVariationBase::_optionalSiftNodeUpdate(currNode);
    const auto m = (currBegin + currEnd) / 2;

    _Node* rightNodePtr = currNode->getRight();
    _Node* leftNodePtr = currNode->getLeft();

    if (begin >= m) { // only right
        return _rangeGetImpl(begin, end, m, currEnd, rightNodePtr);
    }

    if (end <= m) { // only left
        return _rangeGetImpl(begin, end, currBegin, m, leftNodePtr);
    }

    const ValueT rVal = _rangeGetImpl(begin, end, m, currEnd, rightNodePtr);
    const ValueT lVal = _rangeGetImpl(begin, end, currBegin, m, leftNodePtr);

    return _RangeGetCombineVariationBase::_combineGet(
                lVal, rVal, currBegin, m, currEnd);
}

}

#endif // DYNAMIC_SEGMENT_TREE_HPP
