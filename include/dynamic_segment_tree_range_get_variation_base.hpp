#ifndef DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP
#define DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP

#include <concepts.hpp>
#include <disable_operations.hpp>

namespace dst::impl {

template <class KeyT, class ValueT, class GetValueT, class GetInit>
class DynamicSegmentTreeRangeGetInitVariationBase;

template <class KeyT, class ValueT, class GetValueT>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT,
                                                  GetValueT, NoRangeGetOp> {
protected:
    DynamicSegmentTreeRangeGetInitVariationBase(NoRangeGetOp) {};
};

template <class KeyT,
          class ValueT,
          class GetValueT,
          dst::conc::ValueGetInitializer<ValueT, GetValueT> SegGetInit>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT,
                                                  GetValueT, SegGetInit> {
protected:
    DynamicSegmentTreeRangeGetInitVariationBase(const SegGetInit& segInit)
        : _segInitializer(segInit) {}
protected:

    GetValueT _initGet([[maybe_unused]] KeyT /*begin*/,
                       [[maybe_unused]] KeyT /*end*/,
                       const ValueT& val) const {
        return this->_segInitializer(val);
    }

protected:
    const SegGetInit _segInitializer;
};

template <class KeyT,
          class ValueT,
          class GetValueT,
          dst::conc::ValueAndLengthGetInitializer<
              ValueT, KeyT, GetValueT> SegGetInit>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT,
                                                  GetValueT, SegGetInit> {
protected:
    DynamicSegmentTreeRangeGetInitVariationBase(const SegGetInit& segInit)
        : _segInitializer(segInit) {}
protected:

    GetValueT _initGet(KeyT begin, KeyT end, const ValueT& val) const {
        return this->_segInitializer(val, end - begin);
    }
protected:
    const SegGetInit _segInitializer;
};

template <class KeyT,
          class ValueT,
          class GetValueT,
          dst::conc::ValueAndBordersGetInitializer<
              ValueT, KeyT, GetValueT> SegGetInit>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT,
                                                  GetValueT, SegGetInit> {
protected:
    DynamicSegmentTreeRangeGetInitVariationBase(const SegGetInit& segInit)
        : _segInitializer(segInit) {}
protected:

    GetValueT _initGet(KeyT begin, KeyT end, const ValueT& val) const {
        return this->_segInitializer(val, begin, end);
    }
protected:
    const SegGetInit _segInitializer;
};

template <class KeyT, class GetValueT, class GetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase;

template <class KeyT, class GetValueT>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     NoRangeGetOp> {
protected:
    DynamicSegmentTreeRangeGetCombineVariationBase(NoRangeGetOp) {}
};

template <class KeyT,
          class GetValueT,
          dst::conc::ValueGetCombiner<GetValueT> SegGetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     SegGetComb> {
protected:
    DynamicSegmentTreeRangeGetCombineVariationBase(const SegGetComb& segComb)
        : _segCombiner(segComb) {}
protected:

    GetValueT _combineGet(const GetValueT& leftValue,
                          const GetValueT& rightValue,
                          [[maybe_unused]] KeyT /*leftBegin*/,
                          [[maybe_unused]] KeyT /*separation*/,
                          [[maybe_unused]] KeyT /*rightEnd*/) const {
        return _segCombiner(leftValue, rightValue);
    }
protected:
    const SegGetComb _segCombiner;
};

template <class KeyT,
          class GetValueT,
          dst::conc::ValueAndLengthGetCombiner<GetValueT, KeyT> SegGetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     SegGetComb> {
protected:
    DynamicSegmentTreeRangeGetCombineVariationBase(const SegGetComb& segComb)
        : _segCombiner(segComb) {}
protected:
    GetValueT _combineGet(const GetValueT& leftValue,
                          const GetValueT& rightValue,
                          KeyT leftBegin,
                          KeyT separation,
                          KeyT rightEnd) const {
        const KeyT leftLength = separation - leftBegin;
        const KeyT rightLength = rightEnd - separation;
        return _segCombiner(leftValue, rightValue, leftLength, rightLength);
    }
protected:
    const SegGetComb _segCombiner;
};

template <class KeyT,
          class GetValueT,
          dst::conc::ValueAndBordersGetCombiner<GetValueT, KeyT> SegGetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     SegGetComb> {
protected:
    DynamicSegmentTreeRangeGetCombineVariationBase(const SegGetComb& segComb)
        : _segCombiner(segComb) {}
protected:
    GetValueT _combineGet(const GetValueT& leftValue,
                          const GetValueT& rightValue,
                          KeyT leftBegin,
                          KeyT separation,
                          KeyT rightEnd) const {
        return _segCombiner(leftValue, rightValue, leftBegin, separation, rightEnd);
    }
protected:
    const SegGetComb _segCombiner;
};

}

#endif // DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP
