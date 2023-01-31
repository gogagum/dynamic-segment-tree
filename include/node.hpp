#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <optional>
#include <cassert>

namespace {

template<class T,
         class UpdateOp,
         class Allocator = std::allocator<T>>
class Node{
private:
    using _Type = Node<T, UpdateOp, Allocator>;
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<_Type>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

    template <class _UpdateOp>
    struct _UpdateOpHolder {};

    template <class _UpdateOp> requires (!std::is_same_v<_UpdateOp, void>)
    struct _UpdateOpHolder<_UpdateOp>{
        _UpdateOp _updateOp{};
    };

public:
    Node() = default;
    Node(const T& value) : _value(value) {}
    Node(T&& value) : _value(std::move(value)) {}
    bool isLeaf() const;
    const T& getValue() const     { return _value.value(); }
    bool hasValue() const         { return _value.has_value(); }
    void setValue(const T& value);
    void initChildren();
    void siftDown();
    void update(const T& updateValue) requires (!std::is_same_v<UpdateOp, void>);
    _Type* getLeft() const    { return _left; }
    _Type* getRight() const   { return _right; }
    ~Node();

private:
    /**
     * @brief _updateOpHolder
     * Empty struct if there is no update operation, or a struct with an
     * only operation - update function.
     */
    static const _UpdateOpHolder<UpdateOp> _updateOpHolder;
    _Allocator _allocator;
    std::optional<T> _value;
    _Type* _left {nullptr};
    _Type* _right {nullptr};
};

template<class T, class UpdateOp, class Allocator>
const typename Node<T, UpdateOp, Allocator>::template _UpdateOpHolder<UpdateOp>
Node<T, UpdateOp, Allocator>::_updateOpHolder = Node<T, UpdateOp, Allocator>::template _UpdateOpHolder<UpdateOp>();

template<class T, class UpdateOp, class Allocator>
bool Node<T, UpdateOp, Allocator>::isLeaf() const {
    return _left == nullptr && _right == nullptr;
}

template<class T, class UpdateOp, class Allocator>
void Node<T, UpdateOp, Allocator>::setValue(const T& value) {
    _value = value;
    if (!isLeaf()) {
        _left->~Node<T, UpdateOp, Allocator>();
        _right->~Node<T, UpdateOp, Allocator>();
        _allocator.deallocate(_left, 2);
        _left = nullptr;
        _right = nullptr;
    }
}

template<class T, class UpdateOp, class Allocator>
void Node<T, UpdateOp, Allocator>::initChildren() {
    auto nodesPtr = _AllocatorTraits::allocate(_allocator, 2);
    _left = nodesPtr;
    _right = nodesPtr + 1;
    std::construct_at(_left);
    std::construct_at(_right);
}

template<class T, class UpdateOp, class Allocator>
void Node<T, UpdateOp, Allocator>::siftDown() {
    if (isLeaf()) {
        initChildren();
        _left->setValue(_value.value());
        _right->setValue(_value.value());
        _value = std::nullopt;  // _value now has a meaning of
                                // a delayed operation.
    } else {
        if constexpr (!std::is_same_v<UpdateOp, void>) {
            if (_value.has_value()) {
                _left->update(_value.value());
                _right->update(_value.value());
                _value = std::nullopt;
            }
        } else {
            assert(!_value.has_value()
                   && "Non null value in internal node in a tree "
                      "with no update operation.");
        }
    }
}

template<class T, class UpdateOp, class Allocator>
void Node<T, UpdateOp, Allocator>::update(
        const T& updateValue) requires (!std::is_same_v<UpdateOp, void>) {
    if (!isLeaf()) {
        // _value means delayed update.
        if (_value.has_value()) {
            _left->update(_value.value());  // update left with old update
            _right->update(_value.value()); // update right with old update
        }
        // _value continues to have delayed update meaning.
        _value = updateValue;
    } else { // isLeaf()
        assert(_value.has_value() && "Leaf must have a value.");
        _value = Node<T, UpdateOp, Allocator>::_updateOpHolder._updateOp(_value.value(), updateValue);
    }
}

template<class T, class UpdateOp, class Allocator>
Node<T, UpdateOp, Allocator>::~Node() {
    if (!isLeaf()) {
        _left->~Node<T, UpdateOp, Allocator>();
        _right->~Node<T, UpdateOp, Allocator>();
        _allocator.deallocate(_left, 2);
    }
}



}

#endif // NODE_HPP
