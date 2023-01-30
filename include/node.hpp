#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <optional>
#include <cassert>

namespace {

template<class T,
         class UpdateOp,
         class BinaryOp,
         class Allocator = std::allocator<T>>
class Node{
private:
    using _Type = Node<T, UpdateOp, BinaryOp, Allocator>;
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<_Type>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

    static BinaryOp _binaryOp;
    static UpdateOp _updateOp;

public:
    Node() = default;
    Node(const T& value) : _value(value) {}
    Node(T&& value) : _value(std::move(value)) {}
    bool isLeaf() const           { return _left == nullptr && _right == nullptr; }
    const T& getValue() const     { return _value.value(); }
    bool hasValue() const         { return _value.has_value(); }
    void setValue(const T& value);
    void initChildren();
    void siftDown();
    void update(const T& updateValue);
    _Type* getLeft() const    { return _left; }
    _Type* getRight() const   { return _right; }
    ~Node();

private:
    _Allocator _allocator;
    std::optional<T> _value;
    std::optional<T> _updateValue;
    _Type* _left {nullptr};
    _Type* _right {nullptr};
};

template<class T, class UpdateOp, class BinaryOp, class Allocator>
BinaryOp Node<T, UpdateOp, BinaryOp, Allocator>::_binaryOp = BinaryOp();

template<class T, class UpdateOp, class BinaryOp, class Allocator>
UpdateOp Node<T, UpdateOp, BinaryOp, Allocator>::_updateOp = UpdateOp();

template<class T, class UpdateOp, class BinaryOp, class Allocator>
void Node<T, UpdateOp, BinaryOp, Allocator>::setValue(const T& value) {
    _value = value;
    if (!isLeaf()) {
        _left->~Node<T, UpdateOp, BinaryOp, Allocator>();
        _right->~Node<T, UpdateOp, BinaryOp, Allocator>();
        _allocator.deallocate(_left, 2);
        _left = nullptr;
        _right = nullptr;
    }
}

template<class T, class UpdateOp, class BinaryOp, class Allocator>
void Node<T, UpdateOp, BinaryOp, Allocator>::initChildren() {
    auto nodesPtr = _AllocatorTraits::allocate(_allocator, 2);
    _left = nodesPtr;
    _right = nodesPtr + 1;
    std::construct_at(_left);
    std::construct_at(_right);
}

template<class T, class UpdateOp, class BinaryOp, class Allocator>
void Node<T, UpdateOp, BinaryOp, Allocator>::siftDown() {
    if (isLeaf()) {
        initChildren();
    }
    if (_value.has_value()) {
        _left->setValue(_value.value());
        _right->setValue(_value.value());
        _value = std::nullopt;
    }
    if (_updateValue.has_value()) {
        _left->update(_updateValue.value());
        _right->update(_updateValue.value());
        _updateValue = std::nullopt;
    }
}

template<class T, class UpdateOp, class BinaryOp, class Allocator>
void Node<T, UpdateOp, BinaryOp, Allocator>::update(const T& updateValue) {
    if (_updateValue.has_value()) {
        // _updateValue.has_value() means that there is a _value == std::nullopt
        // (it should have been updated in other case.)
        assert(!_value.has_value() && "There must be no value.");
        // If _value == std::nullopt this is not a leaf.
        assert(!isLeaf() && "It must not be a leaf.");
        _left->update(_updateValue.value());  // update left with old update
        _right->update(_updateValue.value()); // update right with old update
        _updateValue = updateValue;
    } else if (!_value.has_value()) {  // !_updateValue.has_value()
        // This node is not a leaf.
        assert(!isLeaf() && "It must not be a leaf.");
        _updateValue = updateValue;
    } else { // _value.has_value()
        // This node is a leaf.
        assert(isLeaf() && "It must be a leaf.");
        // So it must not have an _updateValue.
        assert(!_updateValue.has_value() && "There mast be no update value.");
        _value = _updateOp(_value.value(), updateValue);
    }
}

template<class T, class UpdateOp, class BinaryOp, class Allocator>
Node<T, UpdateOp, BinaryOp, Allocator>::~Node() {
    if (!isLeaf()) {
        _left->~Node<T, UpdateOp, BinaryOp, Allocator>();
        _right->~Node<T, UpdateOp, BinaryOp, Allocator>();
        _allocator.deallocate(_left, 2);
    }
}



}

#endif // NODE_HPP
