#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <concepts>

namespace dst::conc {

////////////////////////////////////////////////////////////////////////////////
// Combiners concepts.                                                        //
////////////////////////////////////////////////////////////////////////////////
template <class T, class  GetValueT>
concept ValueGetCombiner =
    requires(const T& segComb, const GetValueT& left, const GetValueT& right) {
         { segComb(left, right) } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept ValueAndBordersGetCombiner =
    requires(const T& segComb,
             const GetValueT& left,
             const GetValueT& right,
             KeyT leftBegin,
             KeyT separation,
             KeyT rightEnd) {
        {
            segComb(left, right, leftBegin, separation, rightEnd)
        } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept GetCombiner = ValueGetCombiner<T, GetValueT>
                      || ValueAndBordersGetCombiner<T, GetValueT, KeyT>;

////////////////////////////////////////////////////////////////////////////////
// Initializers concepts.                                                     //
////////////////////////////////////////////////////////////////////////////////
template <class T, class ValueT, class GetValueT>
concept ValueGetInitializer =
    requires(const T& segInit, const ValueT& value) {
         { segInit(value) } -> std::convertible_to<GetValueT>;
    };

template <class T, class ValueT, class KeyT, class GetValueT>
concept ValueAndBordersGetInitializer =
    requires(const T& segInit,
             const ValueT& value,
             KeyT begin,
             KeyT end) {
        { segInit(value, begin, end) } -> std::convertible_to<GetValueT>;
    };

template <class T, class ValueT, class KeyT, class GetValueT>
concept GetInitializer =
        ValueGetInitializer<T, ValueT, GetValueT>
        || ValueAndBordersGetInitializer<T, ValueT, KeyT, GetValueT>;

////////////////////////////////////////////////////////////////////////////////
// Update operations concepts.                                                //
////////////////////////////////////////////////////////////////////////////////
template <class T, class ValueT>
concept OneArgUpdateOp = requires(const T& op, const ValueT& val) {
        { op(val) } -> std::convertible_to<ValueT>;
    };

template <class T, class ValueT, class UpdateArgT>
concept TwoArgsUpdateOp = requires(const T& op,
                                   const ValueT& val,
                                   const UpdateArgT& updateArg) {
        {
            op(val, updateArg)
        } -> std::convertible_to<ValueT>;
    };

template <class T,  class ValueT, class UpdateArgT>
concept UpdateOp = OneArgUpdateOp<T, ValueT>
    || TwoArgsUpdateOp<T, ValueT, UpdateArgT>;

}

#endif // CONCEPTS_HPP
