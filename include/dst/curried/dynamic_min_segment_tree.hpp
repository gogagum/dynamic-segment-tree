#ifndef DYNAMIC_MIN_SEGMENT_TREE_HPP
#define DYNAMIC_MIN_SEGMENT_TREE_HPP

#include <concepts>

#include <dst/dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class GetValueT = ValueT,
          class UpdateOp = NoUpdateOp,
          class UpdateArgT = DefaultUpdateArgT<UpdateOp, ValueT>,
          class Allocator = std::allocator<ValueT>>
using DynamicMinSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        GetValueT,
        decltype([](const auto& left, const auto& right) { return std::min(left, right); }),
        decltype([](const ValueT& val) { return static_cast<GetValueT>(val); }),
        UpdateOp,
        UpdateArgT,
        Allocator
    >;

}

#endif // DYNAMIC_MIN_SEGMENT_TREE_HPP
