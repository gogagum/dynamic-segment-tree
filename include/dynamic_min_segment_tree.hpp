#ifndef DYNAMIC_MIN_SEGMENT_TREE_HPP
#define DYNAMIC_MIN_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class UpdateOp = void,
          class Allocator = std::allocator<ValueT>>
using DynamicMinSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        ValueT,
        decltype([](const auto& left, const auto& right) { return std::min(left, right); }),
        std::identity,
        UpdateOp,
        Allocator
    >;

}

#endif // DYNAMIC_MIN_SEGMENT_TREE_HPP
