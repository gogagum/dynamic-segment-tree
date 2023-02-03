#ifndef SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP
#define SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP

#include <dynamic_segment_tree.hpp>

namespace dst {

template <class KeyT, class ValueT>
using SimpleGetSetDynamicSegmentTree =
    DynamicSegmentTree<KeyT, ValueT, void>;

}

#endif // SIMPLE_GET_SET_DYNAMIC_SEGMENT_TREE_HPP
