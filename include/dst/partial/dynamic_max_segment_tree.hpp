////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DYNAMIC_MAX_SEGMENT_TREE_HPP
#define DYNAMIC_MAX_SEGMENT_TREE_HPP

#include <concepts>
#include <dst/dynamic_segment_tree.hpp>
#include <dst/comb.hpp>

namespace dst {

template <std::integral KeyT, class ValueT, class GetValueT = ValueT,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = mp::DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicMaxSegmentTree =
    DynamicSegmentTree<KeyT, ValueT, GetValueT,
                       comb::Max<GetValueT>,
                       decltype([](const ValueT& val) {
                         return static_cast<GetValueT>(val);
                       }),
                       UpdateOp, UpdateArgT, Allocator>;

}  // namespace dst

#endif  // DYNAMIC_MAX_SEGMENT_TREE_HPP
