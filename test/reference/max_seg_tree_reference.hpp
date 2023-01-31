#ifndef MAX_SEG_TREE_REFERENCE_HPP
#define MAX_SEG_TREE_REFERENCE_HPP

#include "seg_tree_reference_base.hpp"

#include <concepts>
#include <algorithm>

template<std::integral KeyT, class ValueT, class ValValUpdateOp = std::plus<ValueT>>
class MaxSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
public:
    MaxSegTreeReference(KeyT begin, KeyT end, const ValueT& value)
        : SegTreeReferenceBase<KeyT, ValueT>(begin, end, value) {};
    ValueT rangeGet(KeyT begin, KeyT end) const;
};

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
ValueT MaxSegTreeReference<KeyT, ValueT, ValValUpdateOp>::rangeGet(KeyT begin, KeyT end) const {
    auto ret = this->_getValue(begin);
    for (KeyT i = begin + 1; i < end; ++i) {
        ret = std::max(ret, this->_getValue(i));
    }
    return ret;
}

#endif // MAX_SEG_TREE_REFERENCE_HPP
