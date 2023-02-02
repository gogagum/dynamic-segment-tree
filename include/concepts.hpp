#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <concepts>

namespace dst::conc {

template <class T, class  GetValueT>
concept ValueSegmentCombiner =
    requires(const T& segComb, const GetValueT& left, const GetValueT& right) {
         { segComb(left, right) } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept ValueAndLengthCombiner =
    requires(const T& segComb, const GetValueT& left, const GetValueT& right,
             KeyT leftLength, KeyT rightLength) {
        { segComb(left, right, leftLength, rightLength) } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept SegmentCombiner = ValueSegmentCombiner<T, GetValueT>
                          || ValueAndLengthCombiner<T, GetValueT, KeyT>;

template <class T, class ValueT, class GetValueT>
concept ValueSegmentInitializer =
    requires(const T& segInit, const ValueT& value) {
         { segInit(value) } -> std::convertible_to<GetValueT>;
    };

template <class T, class ValueT, class KeyT, class GetValueT>
concept ValueAndLengthSegmentInitializer =
    requires(const T& segInit, const ValueT& value, KeyT length) {
        { segInit(value, length) } -> std::convertible_to<GetValueT>;
    };

template <class T, class ValueT, class KeyT, class GetValueT>
concept SegmentInitializer = ValueSegmentInitializer<T, ValueT, GetValueT>
                          || ValueAndLengthSegmentInitializer<T, ValueT, KeyT, GetValueT>;

template <class T, class ValueT>
concept OneArgUpdateOp = requires(const T& op, const ValueT& val) {
        { op(val) } -> std::convertible_to<ValueT>;
    };

template <class T, class ValueT, class UpdateArgT>
concept TwoArgsUpdateOp = requires(const T& op, const ValueT& val, const UpdateArgT& updateArg) {
        { op(val, updateArg) } -> std::convertible_to<ValueT>;
    };

template <class T,  class ValueT, class UpdateArgT>
concept UpdateOp = OneArgUpdateOp<T, ValueT> || TwoArgsUpdateOp<T, ValueT, UpdateArgT>;

}

#endif // CONCEPTS_HPP
