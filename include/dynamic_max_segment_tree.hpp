#ifndef DYNAMIC_MAX_SEGMENT_TREE_HPP
#define DYNAMIC_MAX_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class GetValueT = ValueT,
          class UpdateOp = void,
          class UpdateArgT = impl::DefaultUpdateArgT<ValueT, UpdateOp>,
          class Allocator = std::allocator<ValueT>>
using DynamicMaxSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        GetValueT,
        decltype([](const GetValueT& left, const GetValueT& right) { return std::max(left, right); }),
        decltype([](const ValueT& val) { return static_cast<GetValueT>(val); }),
        UpdateOp,
        UpdateArgT,
        Allocator
    >;

}

#endif // DYNAMIC_MAX_SEGMENT_TREE_HPP
