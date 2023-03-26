////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

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
