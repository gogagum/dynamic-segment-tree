////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP
#define DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP

#include <dst/concepts.hpp>
#include <dst/disable_operations.hpp>

namespace dst::impl {

////////////////////////////////////////////////////////////////////////////////
// Init variation                                                             //
////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, class GetValueT, class GetInit>
class DynamicSegmentTreeRangeGetInitVariationBase;

template <class KeyT, class ValueT, class GetValueT>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT, GetValueT,
                                                  NoRangeGetOp> {
 protected:
  explicit DynamicSegmentTreeRangeGetInitVariationBase(NoRangeGetOp){};
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, class GetValueT,
          dst::conc::ValueGetInitializer<ValueT, GetValueT> SegGetInit>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT, GetValueT,
                                                  SegGetInit> {
 protected:
  explicit DynamicSegmentTreeRangeGetInitVariationBase(
      const SegGetInit& segInit)
      : segInitializer_(segInit) {
  }

 protected:
  [[nodiscard]] GetValueT initGet_(KeyT /*begin*/, KeyT /*end*/,
                                   const ValueT& val) const {
    return segInitializer_(val);
  }

 private:
  SegGetInit segInitializer_;
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class ValueT, class GetValueT,
          dst::conc::ValueAndBordersGetInitializer<ValueT, KeyT, GetValueT>
              SegGetInit>
class DynamicSegmentTreeRangeGetInitVariationBase<KeyT, ValueT, GetValueT,
                                                  SegGetInit> {
 protected:
  explicit DynamicSegmentTreeRangeGetInitVariationBase(
      const SegGetInit& segInit)
      : segInitializer_(segInit) {
  }

 protected:
  [[nodiscard]] GetValueT initGet_(KeyT begin, KeyT end,
                                   const ValueT& val) const {
    return segInitializer_(val, begin, end);
  }

 private:
  SegGetInit segInitializer_;
};

////////////////////////////////////////////////////////////////////////////////
// Get variation                                                              //
////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class GetValueT, class GetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase;

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class GetValueT>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     NoRangeGetOp> {
 protected:
  explicit DynamicSegmentTreeRangeGetCombineVariationBase(NoRangeGetOp) {
  }
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class GetValueT,
          dst::conc::ValueGetCombiner<GetValueT> SegGetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     SegGetComb> {
 protected:
  explicit DynamicSegmentTreeRangeGetCombineVariationBase(
      const SegGetComb& segComb)
      : segCombiner_(segComb) {
  }

 protected:
  [[nodiscard]] GetValueT combineGet_(const GetValueT& leftValue,
                                      const GetValueT& rightValue,
                                      KeyT /*leftBegin*/, KeyT /*separation*/,
                                      KeyT /*rightEnd*/) const {
    return segCombiner_(leftValue, rightValue);
  }

 private:
  SegGetComb segCombiner_;
};

////////////////////////////////////////////////////////////////////////////////
template <class KeyT, class GetValueT,
          dst::conc::ValueAndBordersGetCombiner<GetValueT, KeyT> SegGetComb>
class DynamicSegmentTreeRangeGetCombineVariationBase<KeyT, GetValueT,
                                                     SegGetComb> {
 protected:
  explicit DynamicSegmentTreeRangeGetCombineVariationBase(
      const SegGetComb& segComb)
      : segCombiner_(segComb) {
  }

 protected:
  [[nodiscard]] GetValueT combineGet_(const GetValueT& leftValue,
                                      const GetValueT& rightValue,
                                      KeyT leftBegin, KeyT separation,
                                      KeyT rightEnd) const {
    return segCombiner_(leftValue, rightValue, leftBegin, separation, rightEnd);
  }

 private:
  SegGetComb segCombiner_;
};

}  // namespace dst::impl

#endif  // DYNAMIC_SEGMENT_TREE_RANGE_GET_VARIATION_BASE_HPP
