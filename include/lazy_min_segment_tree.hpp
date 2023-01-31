#ifndef MIN_LAZY_TREE_HPP
#define MIN_LAZY_TREE_HPP

#include <lazy_segment_tree.hpp>

namespace lst {

template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class Allocator = std::allocator<ValueT>>
using LazyMinSegmentTree =
    LazySegmentTree<
        KeyT,
        ValueT,
        ValValUpdateOp,
        decltype([](const auto& left, const auto& right) { return std::min(left, right); }),
        std::identity,
        Allocator
    >;

}

#endif // MIN_LAZY_TREE_HPP
