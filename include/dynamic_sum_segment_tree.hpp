#ifndef DYNAMIC_SUM_SEGMENT_TREE_HPP
#define DYNAMIC_SUM_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class SumT = ValueT,
          class UpdateOp = void,
          class Allocator = std::allocator<ValueT>>
using DynamicSumSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        SumT,
        std::plus<ValueT>,
        std::multiplies<>,
        UpdateOp,
        Allocator
    >;

}

#endif // DYNAMIC_SUM_SEGMENT_TREE_HPP
