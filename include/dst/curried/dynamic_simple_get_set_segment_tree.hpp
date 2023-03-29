////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP
#define SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP

#include <concepts>
#include <dst/dynamic_segment_tree.hpp>
#include <memory>

#include "dst/disable_operations.hpp"

namespace dst {

template <std::integral KeyT, class ValueT,
          class Allocator = std::allocator<ValueT>>
using DynamicSimpleGetSetSegmentTree =
    DynamicSegmentTree<KeyT, ValueT, void, NoRangeGetOp, NoRangeGetOp,
                       NoUpdateOp, void, Allocator>;

}  // namespace dst

#endif  // SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP
