#ifndef NODE_HPP
#define NODE_HPP

#include <memory>

namespace {

template<class T, class BinaryOp, class Allocator = std::allocator<T>>
class Node{
private:
    using _Type = Node<T, BinaryOp, Allocator>;
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<_Type>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

public:
    Node() = default;
    Node(const T& value) : _value(value) {}
    Node(T&& value) : _value(std::move(value)) {}
    bool isList() const       { return _left == nullptr && _right == nullptr; }
    const T& getValue() const { return _value; }
    T& getValue()             { return _value; }
    void initChildren();
    void apply(const T& toApply);
    void siftDown();
    _Type* getLeft() const    { return _left; }
    _Type* getRight() const   { return _right; }
    ~Node();
private:
    _Allocator _allocator;
    T _value;
    _Type* _left {nullptr};
    _Type* _right {nullptr};
};

template<class T, class BinaryOp, class Allocator>
void Node<T, BinaryOp, Allocator>::initChildren() {
    auto nodesPtr = _AllocatorTraits::allocate(_allocator, 2);
    _left = nodesPtr;
    _right = nodesPtr + 1;
    std::construct_at(_left);
    std::construct_at(_right);
}

template<class T, class BinaryOp, class Allocator>
void Node<T, BinaryOp, Allocator>::apply(const T& toApply) {
    const auto op = BinaryOp();
    _value = op(_value, toApply);
}

template<class T, class BinaryOp, class Allocator>
void Node<T, BinaryOp, Allocator>::siftDown() {
    if (isList()) {
        initChildren();
    }
    _left->apply(_value);
    _right->apply(_value);
    _value = T{};
}

template<class T, class BinaryOp, class Allocator>
Node<T, BinaryOp, Allocator>::~Node() {
    if (!isList()) {
        _left->~Node<T, BinaryOp, Allocator>();
        _right->~Node<T, BinaryOp, Allocator>();
        _allocator.deallocate(_left, 2);
    }
}

}

#endif // NODE_HPP
