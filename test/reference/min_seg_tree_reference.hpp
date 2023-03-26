////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MIN_SEG_TREE_REFERENCE_HPP
#define MIN_SEG_TREE_REFERENCE_HPP

#include <concepts>
#include <algorithm>

#include "seg_tree_reference_base.hpp"

template<std::integral KeyT, class ValueT>
class MinSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
public:
    using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
    ValueT rangeGet(KeyT begin, KeyT end) const;
};

template<std::integral KeyT, class ValueT>
ValueT MinSegTreeReference<KeyT, ValueT>::rangeGet(KeyT begin, KeyT end) const {
    auto ret = this->_getValue(begin);
    for (KeyT i = begin + 1; i < end; ++i) {
        ret = std::min(ret, this->_getValue(i));
    }
    return ret;
}

#endif // MIN_SEG_TREE_REFERENCE_HPP
