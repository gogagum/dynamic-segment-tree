#ifndef MAX_LAZY_SEGMENT_TREE_HPP
#define MAX_LAZY_SEGMENT_TREE_HPP

#include <concepts>

#include <lazy_segment_tree.hpp>

namespace lst {

template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class Allocator = std::allocator<ValueT>>
using LazyMaxSegmentTree =
    LazySegmentTree<
        KeyT,
        ValueT,
        ValValUpdateOp,
        decltype([](const auto& left, const auto& right) { return std::max(left, right); }),
        std::identity,
        Allocator
    >;

}

#endif // MAX_LAZY_SEGMENT_TREE_HPP
