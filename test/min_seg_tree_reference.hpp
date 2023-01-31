#ifndef MIN_SEG_TREE_REFERENCE_HPP
#define MIN_SEG_TREE_REFERENCE_HPP

#include "seg_tree_reference_base.hpp"

#include <concepts>
#include <algorithm>

template<std::integral KeyT, class ValueT, class ValValUpdateOp = std::plus<ValueT>>
class MinSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
public:
    MinSegTreeReference(KeyT begin, KeyT end, const ValueT& value)
        : SegTreeReferenceBase<KeyT, ValueT>(begin, end, value) {};
    ValueT rangeGet(KeyT begin, KeyT end) const;
};

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
ValueT MinSegTreeReference<KeyT, ValueT, ValValUpdateOp>::rangeGet(KeyT begin, KeyT end) const {
    auto ret = this->_getValue(begin);
    for (KeyT i = begin + 1; i < end; ++i) {
        ret = std::min(ret, this->_getValue(i));
    }
    return ret;
}

#endif // MIN_SEG_TREE_REFERENCE_HPP
