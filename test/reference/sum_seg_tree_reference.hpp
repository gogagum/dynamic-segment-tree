#ifndef SUM_SEG_TREE_REFERENCE_HPP
#define SUM_SEG_TREE_REFERENCE_HPP

#include "seg_tree_reference_base.hpp"

#include <concepts>
#include <algorithm>

template<std::integral KeyT, class ValueT, class ValValUpdateOp = std::plus<ValueT>>
class SumSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
public:
    SumSegTreeReference(KeyT begin, KeyT end, const ValueT& value)
        : SegTreeReferenceBase<KeyT, ValueT>(begin, end, value) {};
    ValueT rangeGet(KeyT begin, KeyT end) const;
};

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
ValueT SumSegTreeReference<KeyT, ValueT, ValValUpdateOp>::rangeGet(KeyT begin, KeyT end) const {
    auto ret = 0;
    for (KeyT i = begin; i < end; ++i) {
        ret += this->_getValue(i);
    }
    return ret;
}

#endif // SUM_SEG_TREE_REFERENCE_HPP
