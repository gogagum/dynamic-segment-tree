#ifndef NODE_BASE_HPP
#define NODE_BASE_HPP

#include <memory>
#include <optional>

namespace dst::impl {

////////////////////////////////////////////////////////////////////////////////
/// \brief The BaseNode class
///
template <class T, class Derived, class Allocator>
class BaseNode {
    using _Allocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Derived>;
    using _AllocatorTraits = std::allocator_traits<_Allocator>;

public:
    BaseNode() :               _left{ nullptr }, _right{ nullptr } {};
    BaseNode(const T& value) : _value(value), _left{ nullptr }, _right{ nullptr } {}
    BaseNode(T&& value) :      _value(std::move(value)) {}
public:
    const T& getValue() const  { return _value.value(); }
    void setValue(const T&);
    bool hasValue() const      { return _value.has_value(); }
    void setNullValue()        { _value = std::nullopt; }
    bool isLeaf() const        { return !_left && !_right; };
    void initChildren();
    Derived* getLeft() const   { return _left; }
    Derived* getRight() const  { return _right; }
    ~BaseNode();
protected:
    _Allocator _allocator;
    std::optional<T> _value;
    Derived* _left {nullptr};
    Derived* _right {nullptr};
};

////////////////////////////////////////////////////////////////////////////////
template<class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::setValue(const T& value) {
    this->_value = value;
    if (!this->isLeaf()) {
        this->getLeft()->~Derived();
        this->getRight()->~Derived();
        _AllocatorTraits::deallocate(this->_allocator, _left, 2);
        this->_left = nullptr;
        this->_right = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
template<class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::initChildren() {
    assert(this->isLeaf() && "Can only init children for a leaf.");
    auto nodesPtr = _AllocatorTraits::allocate(this->_allocator, 2);
    this->_left = nodesPtr;
    this->_right = nodesPtr + 1;
    std::construct_at(this->_left);
    std::construct_at(this->_right);
    this->_left->setValue(this->_value.value());
    this->_right->setValue(this->_value.value());
    this->_value = std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
template<class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::~BaseNode() {
    if (!this->isLeaf()) {
        this->getLeft()->~Derived();
        this->getRight()->~Derived();
        _AllocatorTraits::deallocate(this->_allocator, _left, 2);
        _left = nullptr;
        _right = nullptr;
    }
}

}

#endif // NODE_BASE_HPP
