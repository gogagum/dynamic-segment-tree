#ifndef DYNAMIC_MAX_SEGMENT_TREE_HPP
#define DYNAMIC_MAX_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicMaxSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        ValValUpdateOp,
        decltype([](const auto& left, const auto& right) { return std::max(left, right); }),
        std::identity,
        Allocator
    >;

}

#endif // DYNAMIC_MAX_SEGMENT_TREE_HPP
