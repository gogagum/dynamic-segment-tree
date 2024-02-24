////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <concepts>
#include <dst/disable_operations.hpp>

namespace dst::conc {

////////////////////////////////////////////////////////////////////////////////
// Combiners concepts.                                                        //
////////////////////////////////////////////////////////////////////////////////
template <class T, class GetValueT>
concept ValueGetCombiner =
    requires(const T& segComb, const GetValueT& left, const GetValueT& right) {
      { segComb(left, right) } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept ValueAndBordersGetCombiner =
    requires(const T& segComb, const GetValueT& left, const GetValueT& right,
             KeyT leftBegin, KeyT separation, KeyT rightEnd) {
      {
        segComb(left, right, leftBegin, separation, rightEnd)
        } -> std::same_as<GetValueT>;
    };

template <class T, class GetValueT, class KeyT>
concept GetCombiner = ValueGetCombiner<T, GetValueT> ||
                      ValueAndBordersGetCombiner<T, GetValueT, KeyT>;

////////////////////////////////////////////////////////////////////////////////
// Initializers concepts.                                                     //
////////////////////////////////////////////////////////////////////////////////
template <class T, class ValueT, class GetValueT>
concept ValueGetInitializer = requires(const T& segInit, const ValueT& value) {
                                {
                                  segInit(value)
                                  } -> std::convertible_to<GetValueT>;
                              };

template <class T, class ValueT, class KeyT, class GetValueT>
concept ValueAndBordersGetInitializer =
    requires(const T& segInit, const ValueT& value, KeyT begin, KeyT end) {
      { segInit(value, begin, end) } -> std::convertible_to<GetValueT>;
    };

template <class T, class ValueT, class KeyT, class GetValueT>
concept GetInitializer =
    ValueGetInitializer<T, ValueT, GetValueT> ||
    ValueAndBordersGetInitializer<T, ValueT, KeyT, GetValueT>;

////////////////////////////////////////////////////////////////////////////////
// Update operations concepts.                                                //
////////////////////////////////////////////////////////////////////////////////
template <class T, class ValueT>
concept OneArgUpdateOp = requires(const T& operation, const ValueT& val) {
                           { operation(val) } -> std::convertible_to<ValueT>;
                         };

template <class T, class ValueT, class UpdateArgT>
concept TwoArgsUpdateOp = requires(const T& operation, const ValueT& val,
                                   const UpdateArgT& updateArg) {
                            {
                              operation(val, updateArg)
                              } -> std::convertible_to<ValueT>;
                          };

template <class T, class ValueT, class UpdateArgT>
concept UpdateOp =
    OneArgUpdateOp<T, ValueT> || TwoArgsUpdateOp<T, ValueT, UpdateArgT>;

////////////////////////////////////////////////////////////////////////////////
// Optional parameters concepts.                                              //
////////////////////////////////////////////////////////////////////////////////
template <class T, class GetValueT, class KeyT>
concept OptGetCombiner =
    GetCombiner<T, GetValueT, KeyT> || std::is_same_v<T, NoRangeGetOp>;

template <class T, class ValueT, class KeyT, class GetValueT>
concept OptGetInitializer =
    GetInitializer<T, ValueT, KeyT, GetValueT> || std::is_same_v<T, NoRangeGetOp>;

template <class T, class ValueT, class UpdateArgT>
concept OptUpdateOp =
    UpdateOp<T, ValueT, UpdateArgT> || std::is_same_v<T, NoUpdateOp>;

}  // namespace dst::conc

#endif  // CONCEPTS_HPP
