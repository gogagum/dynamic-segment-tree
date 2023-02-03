#ifndef DYNAMIC_SUM_SEGMENT_TREE_HPP
#define DYNAMIC_SUM_SEGMENT_TREE_HPP

#include <concepts>

#include <dynamic_segment_tree.hpp>

namespace dst {

namespace impl {

template <class SumT>
struct SumComb {
    SumT operator()(const SumT& val1, const SumT& val2) const {
        return val1 + val2;
    };
};

template <class KeyT, class ValueT, class SumT>
struct SumInit {
    SumT operator()(const ValueT& val, KeyT length) const {
        return val * length;
    };
};

}


template <std::integral KeyT,
          class ValueT,
          class SumT = ValueT,
          class UpdateOp = void,
          class UpdateArgT = impl::DefaultUpdateArgT<ValueT, UpdateOp>,
          class Allocator = std::allocator<ValueT>>
using DynamicSumSegmentTree =
    DynamicSegmentTree<
        KeyT,
        ValueT,
        SumT,
        impl::SumComb<SumT>,
        impl::SumInit<KeyT, ValueT, SumT>,
        UpdateOp,
        UpdateArgT,
        Allocator
    >;

}

#endif // DYNAMIC_SUM_SEGMENT_TREE_HPP
