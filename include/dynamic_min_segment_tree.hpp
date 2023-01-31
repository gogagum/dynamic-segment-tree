#ifndef DYNAMIC_MIN_SEGMENT_TREE_HPP
#define DYNAMIC_MIN_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicMinSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        ValValUpdateOp,
        decltype([](const auto& left, const auto& right) { return std::min(left, right); }),
        std::identity,
        Allocator
    >;

}

#endif // DYNAMIC_MIN_SEGMENT_TREE_HPP
