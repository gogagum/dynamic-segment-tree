#ifndef MIN_LAZY_TREE_HPP
#define MIN_LAZY_TREE_HPP

#include <lazy_segment_tree.hpp>
#include <operations.hpp>

namespace lst {

template <std::integral KeyT,
          class ValueT,
          class ValValUpdateOp = std::plus<ValueT>,
          class Allocator = std::allocator<ValueT>>
using LazyMinSegmentTree = LazySegmentTree<KeyT,
                                           ValueT,
                                           ValValUpdateOp,
                                           op::ValValMinGetOp<ValueT>,
                                           op::ValKeyKeyIgnoringGetOp<ValueT>,
                                           Allocator>;

}

#endif // MIN_LAZY_TREE_HPP
