#ifndef NODE_HPP
#define NODE_HPP

#include <memory>

namespace {

template<class T, class BinaryOp>
class Node{
using Type = Node<T, BinaryOp>;

public:
    Node() = default;
    Node(const T& value) : _value(value) {}
    Node(T&& value) : _value(std::move(value)) {}
    bool isList() const          { return _left == nullptr && _right == nullptr; }
    const T& getValue() const    { return _value; }
    T& getValue()                { return _value; }
    void initChildren()          { _left = std::make_unique<Type>();
                                  _right = std::make_unique<Type>(); }
    void apply(const T& toApply) { const auto op = BinaryOp();
                                   _value = op(_value, toApply); }
    void siftDown()              { if (isList()) { initChildren(); }
                                   _left->apply(_value);
                                   _right->apply(_value);
                                   _value = T{}; }
    Type* getLeft() const        { return _left.get(); }
    Type* getRight() const       { return _right.get(); }
private:
    T _value;
    std::unique_ptr<Node<T, BinaryOp>> _left;
    std::unique_ptr<Node<T, BinaryOp>> _right;
};

}

#endif // NODE_HPP
