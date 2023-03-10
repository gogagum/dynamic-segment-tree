#ifndef DYNAMIC_NEGATE_SEGMENT_TREE_HPP
#define DYNAMIC_NEGATE_SEGMENT_TREE_HPP

#include <concepts>
#include <memory>

#include <dst/dynamic_segment_tree.hpp>

namespace dst {

namespace impl {

template <class ValueT>
struct Negate {
    ValueT operator()(const ValueT& x) const { return -x; };
};

}

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          class SegCombiner = NoRangeGetOp,
          class SegInitializer = NoRangeGetOp,
          class Allocator = std::allocator<ValueT>>
using DynamicNegateSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        GetValueT,
        SegCombiner,
        SegInitializer,
        impl::Negate<ValueT>,
        void,
        Allocator
    >;

}

#endif // DYNAMIC_NEGATE_SEGMENT_TREE_HPP
