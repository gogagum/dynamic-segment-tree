#ifndef SEG_TREE_REFERENCE_BASE_HPP
#define SEG_TREE_REFERENCE_BASE_HPP

#include <concepts>
#include <functional>
#include <vector>

template<std::integral KeyT, class ValueT>
class SegTreeReferenceBase {
public:
    SegTreeReferenceBase(KeyT begin, KeyT end, const ValueT& value);
    template <class UpdateOp, class UpdateArgT>
    void update(KeyT begin,
                KeyT end,
                const UpdateOp& updateOp,
                const UpdateArgT& toUpdate);
    template <class UpdateOp>
    void update(KeyT begin,
                KeyT end,
                const UpdateOp& updateOp);
    void set(KeyT begin, KeyT end, const ValueT& toSet);
    ValueT get(KeyT key) const              { return _getValue(key); }
protected:
    ValueT& _getValue(KeyT key)             { return _values[key - _begin]; }
    const ValueT& _getValue(KeyT key) const { return _values[key - _begin]; }
private:
    KeyT _begin;
    std::vector<ValueT> _values;
};

////////////////////////////////////////////////////////////////////////////////
template<std::integral KeyT, class ValueT>
SegTreeReferenceBase<KeyT, ValueT>::SegTreeReferenceBase(
        KeyT begin, KeyT end, const ValueT& value)
    : _begin(begin), _values(end - begin, value) {}

////////////////////////////////////////////////////////////////////////////////
template<std::integral KeyT, class ValueT>
template <class UpdateOp, class UpdateArgT>
void SegTreeReferenceBase<KeyT, ValueT>::update(KeyT begin,
                                                KeyT end,
                                                const UpdateOp& updateOp,
                                                const UpdateArgT& toUpdate) {
    for (KeyT i = begin; i < end; ++i) {
        _getValue(i) = updateOp(_getValue(i), toUpdate);
    }
}

////////////////////////////////////////////////////////////////////////////////
template<std::integral KeyT, class ValueT>
template <class UpdateOp>
void SegTreeReferenceBase<KeyT, ValueT>::update(
        KeyT begin, KeyT end, const UpdateOp& updateOp) {    
    for (KeyT i = begin; i < end; ++i) {
        _getValue(i) = updateOp(_getValue(i));
    }
}

////////////////////////////////////////////////////////////////////////////////
template<std::integral KeyT, class ValueT>
void SegTreeReferenceBase<KeyT, ValueT>::set(
        KeyT begin, KeyT end, const ValueT& toSet) {
    for (KeyT key = begin; key < end; ++key) {
        _getValue(key) = toSet;
    }
}

#endif // SEG_TREE_REFERENCE_BASE_HPP
