////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_SUM_SEGMENT_TREE_HPP
#define DYNAMIC_SUM_SEGMENT_TREE_HPP

#include <concepts>
#include <dst/dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT, class ValueT, class SumT = ValueT,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicSumSegmentTree =
    DynamicSegmentTree<KeyT, ValueT, SumT,
                       decltype([](const SumT& val1, const SumT& val2) -> SumT {
                         return val1 + val2;
                       }),
                       decltype([](const ValueT& val, KeyT begin,
                                   KeyT end) -> SumT {
                         return val * (end - begin);
                       }),
                       UpdateOp, UpdateArgT, Allocator>;

}  // namespace dst

#endif  // DYNAMIC_SUM_SEGMENT_TREE_HPP
