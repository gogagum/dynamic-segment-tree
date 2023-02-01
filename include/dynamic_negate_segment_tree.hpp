#ifndef DYNAMIC_NEGATE_SEGMENT_TREE_HPP
#define DYNAMIC_NEGATE_SEGMENT_TREE_HPP

#include <dynamic_segment_tree.hpp>

#include <memory>

namespace dst {

template <std::integral KeyT,
          class ValueT,
          class GetValueT,
          conc::SegmentCombiner<GetValueT, KeyT> SegCombiner,
          conc::SegmentInitializer<ValueT, KeyT, GetValueT> SegInitializer,
          class Allocator = std::allocator<ValueT>>
using DynamicNegateSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        GetValueT,
        SegCombiner,
        SegInitializer,
        decltype([](auto x) { return -x; }),
        void,
        Allocator
    >;

}

#endif // DYNAMIC_NEGATE_SEGMENT_TREE_HPP
