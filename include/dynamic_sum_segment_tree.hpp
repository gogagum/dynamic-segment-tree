#ifndef DYNAMIC_SUM_SEGMENT_TREE_HPP
#define DYNAMIC_SUM_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class UpdateOp = void,
          class Allocator = std::allocator<ValueT>>
using DynamicSumSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        std::plus<ValueT>,
        std::multiplies<>,
        UpdateOp,
        Allocator
    >;

}

#endif // DYNAMIC_SUM_SEGMENT_TREE_HPP
