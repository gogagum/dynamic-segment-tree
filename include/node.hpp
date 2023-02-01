#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <optional>
#include <cassert>

namespace {

template <class T, class Derived, class Allocator>
class BaseNode {
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Derived>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

public:
    BaseNode() = default;
    BaseNode(const T& value) : _value(value) {}
    BaseNode(T&& value) : _value(std::move(value)) {}
    const T& getValue() const     { return _value.value(); }
    void setValue(const T&);
    bool hasValue() const         { return _value.has_value(); }
    void setNullValue()           { _value = std::nullopt; }
    bool isLeaf() const;
    void initChildren();
    Derived* getLeft() const      { return _left; }
    Derived* getRight() const     { return _right; }
    ~BaseNode();
protected:
    _Allocator _allocator;
    std::optional<T> _value;
    Derived* _left {nullptr};
    Derived* _right {nullptr};
};

template<class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::setValue(const T& value) {
    this->_value = value;
    if (!this->isLeaf()) {
        this->getLeft()->~Derived();
        this->getRight()->~Derived();
        _allocator.deallocate(this->_left, 2);
        this->_left = nullptr;
        this->_right = nullptr;
    }
}

template<class T, class Derived, class Allocator>
bool BaseNode<T, Derived, Allocator>::isLeaf() const {
    return _left == nullptr && _right == nullptr;
}

template<class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::initChildren() {
    assert(this->isLeaf() && "Can only init children for a leaf.");
    auto nodesPtr = Derived::_AllocatorTraits::allocate(this->_allocator, 2);
    this->_left = nodesPtr;
    this->_right = nodesPtr + 1;
    std::construct_at(this->_left);
    std::construct_at(this->_right);
    this->_left->setValue(this->_value.value());
    this->_right->setValue(this->_value.value());
    this->_value = std::nullopt;
}


template<class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::~BaseNode() {
    if (!this->isLeaf()) {
        _left->_destruct();
        _right->_destruct();
        _allocator.deallocate(_left, 2);
        _left = nullptr;
        _right = nullptr;
    }
}

template<class T,
         class UpdateT,
         class Allocator = std::allocator<T>>
class Node : public BaseNode<T, Node<T, UpdateT, Allocator>, Allocator> {
private:
    using _Type = Node<T, UpdateT, Allocator>;
    using _Base = BaseNode<T, _Type, Allocator>;
public:
    Node() = default;
    Node(const T& value) : _Base(value) {}
    Node(T&& value) : _Base(std::move(value)) {}
    void setUpdateValue(const UpdateT& updateValue);
    bool hasUpdateValue() const           { return _updateValue.has_value(); }
    const UpdateT& getUpdateValue() const { return _updateValue.value(); }
    void setNullUpdateValue()             { _updateValue = std::nullopt; }
    ~Node() { _destruct(); }
private:
    void _destruct()  { reinterpret_cast<_Base*>(this)->~_Base(); }
private:
    std::optional<UpdateT> _updateValue;
private:
    friend class BaseNode<T, Node<T, UpdateT, Allocator>, Allocator>;
};

template<class T, class UpdateT, class Allocator>
void Node<T, UpdateT, Allocator>::setUpdateValue(const UpdateT& updateValue) {
    assert(!this->isLeaf() && "Can`t set update value for a leaf.");
    _updateValue = updateValue;
}

template<class T, class Allocator>
class Node<T, void, Allocator> : public BaseNode<T, Node<T, void, Allocator>, Allocator> {
private:
    using _Type = Node<T, void, Allocator>;
    using _Base = BaseNode<T, _Type, Allocator>;
public:
    Node() = default;
    Node(const T& value) : BaseNode<T, _Type, Allocator>(value) {}
    Node(T&& value) : BaseNode<T, _Type, Allocator>(std::move(value)) {}
    ~Node() { _destruct(); }
private:
    void _destruct()  { reinterpret_cast<_Base*>(this)->~_Base(); }
private:
    friend class BaseNode<T, Node<T, void, Allocator>, Allocator>;
};




}

#endif // NODE_HPP
