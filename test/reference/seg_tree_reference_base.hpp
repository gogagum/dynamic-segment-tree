#ifndef SEG_TREE_REFERENCE_BASE_HPP
#define SEG_TREE_REFERENCE_BASE_HPP

#include <concepts>
#include <functional>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// \brief The SegTreeReferenceBase class - base serment tree mock class.
///
template<std::integral KeyT, class ValueT>
class SegTreeReferenceBase {
public:
    SegTreeReferenceBase(KeyT begin, KeyT end, const ValueT& value);

    /**
     * @brief update - apply update operation on a range with argument
     * `toUpdate.
     * @param begin - beginning of a range.
     * @param end - ending of a range (not included).
     * @param updateOp - applied operation.
     * @param toUpdate - operation argument.
     */
    template <class UpdateOp, class UpdateArgT>
    void update(KeyT begin,
                KeyT end,
                const UpdateOp& updateOp,
                const UpdateArgT& toUpdate);

    /**
     * @brief update - apply update operation on a range (with no argument).
     * @param begin - beginning of a range.
     * @param end - ending of a range (not included).
     * @param updateOp - operation argument.
     */
    template <class UpdateOp>
    void update(KeyT begin,
                KeyT end,
                const UpdateOp& updateOp);

    /**
     * @brief set - set value on a range.
     * @param begin - beginning of a range.
     * @param end - ending of a range (not included).
     * @param toSet - value to set.
     */
    void set(KeyT begin, KeyT end, const ValueT& toSet);

    /**
     * @brief get - get value by key.
     * @param key - key.
     * @return value, received by key.
     */
    const ValueT& get(KeyT key) const       { return _getValue(key); }
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
