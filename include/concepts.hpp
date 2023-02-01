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


}

#endif // CONCEPTS_HPP
