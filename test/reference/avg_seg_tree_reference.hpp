#ifndef AVG_SEG_TREE_REFERENCE_HPP
#define AVG_SEG_TREE_REFERENCE_HPP

#include <concepts>

#include "seg_tree_reference_base.hpp"

template <std::integral KeyT, class ValueT>
class AvgSegTreeReference : public SegTreeReferenceBase<KeyT, ValueT> {
public:
    using SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase;
    ValueT rangeGet(KeyT begin, KeyT end) const;
};

template <std::integral KeyT, class ValueT>
ValueT AvgSegTreeReference<KeyT, ValueT>::rangeGet(KeyT begin, KeyT end) const {
    ValueT ret = ValueT{};
    for (KeyT i = begin; i < end; ++i) {
        ret += this->_getValue(i);
    }
    return ret / (end - begin);
}


#endif // AVG_SEG_TREE_REFERENCE_HPP
