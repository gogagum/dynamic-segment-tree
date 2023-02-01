#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <optional>
#include <cassert>

namespace {

template<class T,
         class Allocator = std::allocator<T>>
class Node{
private:
    using _Type = Node<T, Allocator>;
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<_Type>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

public:
    Node() = default;
    Node(const T& value) : _value(value) {}
    Node(T&& value) : _value(std::move(value)) {}
    bool isLeaf() const;
    const T& getValue() const     { return _value.value(); }
    bool hasValue() const         { return _value.has_value(); }
    void setValue(const T& value);
    void setNullValue()           { _value = std::nullopt; }
    void initChildren();
    template <class UpdateOp> requires (!std::is_same_v<UpdateOp, void>)
    void update(const UpdateOp& updateOp, const T& updateValue);
    _Type* getLeft() const    { return _left; }
    _Type* getRight() const   { return _right; }
    ~Node();

private:
    _Allocator _allocator;
    std::optional<T> _value;
    _Type* _left {nullptr};
    _Type* _right {nullptr};
};

template<class T, class Allocator>
bool Node<T, Allocator>::isLeaf() const {
    return _left == nullptr && _right == nullptr;
}

template<class T, class Allocator>
void Node<T, Allocator>::setValue(const T& value) {
    _value = value;
    if (!isLeaf()) {
        _left->~Node<T, Allocator>();
        _right->~Node<T, Allocator>();
        _allocator.deallocate(_left, 2);
        _left = nullptr;
        _right = nullptr;
    }
}

template<class T, class Allocator>
void Node<T, Allocator>::initChildren() {
    assert(isLeaf() && "Can only init children for a leaf.");
    auto nodesPtr = _AllocatorTraits::allocate(_allocator, 2);
    _left = nodesPtr;
    _right = nodesPtr + 1;
    std::construct_at(_left);
    std::construct_at(_right);
    _left->setValue(_value.value());
    _right->setValue(_value.value());
    _value = std::nullopt;
}

template<class T, class Allocator>
template<class UpdateOp> requires (!std::is_same_v<UpdateOp, void>)
void Node<T, Allocator>::update(const UpdateOp& updateOp, const T& updateValue)  {
    if (!isLeaf()) {
        // _value means delayed update.
        if (_value.has_value()) {
            _left->update(updateOp, _value.value());  // update left with old update
            _right->update(updateOp, _value.value()); // update right with old update
        }
        // _value continues to have delayed update meaning.
        _value = updateValue;
    } else { // isLeaf()
        assert(_value.has_value() && "Leaf must have a value.");
        _value = updateOp(_value.value(), updateValue);
    }
}

template<class T, class Allocator>
Node<T, Allocator>::~Node() {
    if (!isLeaf()) {
        _left->~Node<T, Allocator>();
        _right->~Node<T, Allocator>();
        _allocator.deallocate(_left, 2);
    }
}



}

#endif // NODE_HPP
