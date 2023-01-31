#ifndef SEG_TREE_REFERENCE_BASE_HPP
#define SEG_TREE_REFERENCE_BASE_HPP

#include <concepts>
#include <functional>
#include <vector>

template<std::integral KeyT, class ValueT, class ValValUpdateOp = std::plus<ValueT>>
class SegTreeReferenceBase {
protected:
    SegTreeReferenceBase(KeyT begin, KeyT end, const ValueT& value);
public:
    void update(KeyT begin, KeyT end, const ValueT& toUpdate);
    void set(KeyT begin, KeyT end, const ValueT& toSet);
    ValueT get(KeyT key) const { return _getValue(key); }
protected:
    ValueT& _getValue(KeyT key) { return _values[key - _begin]; }
    const ValueT& _getValue(KeyT key) const { return _values[key - _begin]; }
private:
    const static ValValUpdateOp _updateOp;
    KeyT _begin;
    std::vector<ValueT> _values;
};

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
const ValValUpdateOp SegTreeReferenceBase<KeyT, ValueT, ValValUpdateOp>::_updateOp = ValValUpdateOp();

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
SegTreeReferenceBase<KeyT, ValueT, ValValUpdateOp>::SegTreeReferenceBase(
        KeyT begin, KeyT end, const ValueT& value)
    : _begin(begin), _values(end - begin, value) {}

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
void SegTreeReferenceBase<KeyT, ValueT, ValValUpdateOp>::update(
        KeyT begin, KeyT end, const ValueT& toUpdate) {
    for (KeyT key = begin; key < end; ++key) {
        _values[key - _begin] = _updateOp(_values[key - _begin]);
    }
}

template<std::integral KeyT, class ValueT, class ValValUpdateOp>
void SegTreeReferenceBase<KeyT, ValueT, ValValUpdateOp>::set(
        KeyT begin, KeyT end, const ValueT& toSet) {
    for (KeyT key = begin; key < end; ++key) {
        _values[key - _begin] = toSet;
    }
}

#endif // SEG_TREE_REFERENCE_BASE_HPP
