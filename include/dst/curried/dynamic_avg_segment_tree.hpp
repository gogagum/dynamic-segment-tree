////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_AVG_SEGMENT_TREE_HPP
#define DYNAMIC_AVG_SEGMENT_TREE_HPP

#include <concepts>
#include <dst/dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT, class ValueT, class GetValueT = ValueT,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicAvgSegmentTree =
    DynamicSegmentTree<KeyT, ValueT, GetValueT,
                       decltype([](const GetValueT& left,
                                   const GetValueT& right, KeyT leftBegin,
                                   KeyT separation, KeyT rightEnd) {
                         assert(separation > leftBegin);
                         assert(rightEnd > separation);
                         return (left * (separation - leftBegin) +
                                 right * (rightEnd - separation)) /
                                (rightEnd - leftBegin);
                       }),
                       std::identity, UpdateOp, UpdateArgT, Allocator>;

}  // namespace dst

#endif  // DYNAMIC_AVG_SEGMENT_TREE_HPP
