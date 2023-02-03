#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <optional>
#include <cassert>

#include <node_base.hpp>

namespace dst::impl {

template<class T,
         class UpdateT,
         class Allocator = std::allocator<T>>
class Node;

template<class T, class UpdateT, class Allocator>
class Node<T, std::optional<UpdateT>, Allocator>
        : public BaseNode<T, Node<T, std::optional<UpdateT>, Allocator>, Allocator> {
private:
    using _Type = Node<T, std::optional<UpdateT>, Allocator>;
    using _Base = BaseNode<T, _Type, Allocator>;
public:
    Node() = default;
    Node(const T& value) : _Base(value) {}
    Node(T&& value) : _Base(std::move(value)) {}
    void setUpdateValue(const UpdateT& updateValue);
    template <class UpdateOp>
    void update(const UpdateOp& updateOp, const UpdateT& update);
    template <class UpdateOp>
    void siftOptUpdate(const UpdateOp& updateOp);
    ~Node() { _destruct(); }
private:
    void _destruct()  { reinterpret_cast<_Base*>(this)->~_Base(); }
private:
    std::optional<UpdateT> _updateValue;
private:
    friend class BaseNode<T, _Type, Allocator>;
};

template<class T, class UpdateT, class Allocator>
template <class UpdateOp>
void
Node<T, std::optional<UpdateT>, Allocator>::update(const UpdateOp& updateOp,
                                                   const UpdateT& updateVal) {
    if (!this->isLeaf()) {
        if (this->_updateValue.has_value()) {
            // update left with old update
            this->getLeft()->update(updateOp, this->_updateValue.value());
            // update right with old update
            this->getRight()->update(updateOp, this->_updateValue.value());
        }
        // _value continues to have delayed update meaning.
        this->_updateValue = updateVal;
    } else { // isLeaf()
        assert(this->hasValue() && "Leaf must have a value.");
        this->setValue(updateOp(this->getValue(), updateVal));
    }
}

template<class T, class UpdateT, class Allocator>
template <class UpdateOp>
void Node<T, std::optional<UpdateT>, Allocator>::siftOptUpdate(
        const UpdateOp& updateOp) {
    if (this->_updateValue.has_value()) {
        assert(!this->isLeaf() && "It nust not be a leaf.");
        this->getLeft()->update(updateOp, this->_updateValue.value());
        this->getRight()->update(updateOp, this->_updateValue.value());
        this->_updateValue = std::nullopt;
    }
}

template<class T, class UpdateT, class Allocator>
void
Node<T, std::optional<UpdateT>, Allocator>::setUpdateValue(
        const UpdateT& updateValue) {
    assert(!this->isLeaf() && "Can`t set update value for a leaf.");
    _updateValue = updateValue;
}

template<class T, class Allocator>
class Node<T, bool, Allocator>
        : public BaseNode<T, Node<T, bool, Allocator>, Allocator> {
private:
    using _Type = Node<T, bool, Allocator>;
    using _Base = BaseNode<T, _Type, Allocator>;
public:
    Node() = default;
    Node(const T& value) : _Base(value) {}
    Node(T&& value) : _Base(std::move(value)) {}
    void addUpdate();
    template <class UpdateOp>
    void update(const UpdateOp& updateOp);
    template <class UpdateOp>
    void siftOptUpdate(const UpdateOp& updateOp);
    ~Node() { reinterpret_cast<_Base*>(this)->~_Base(); }
private:
    bool _toUpdate{false};
private:
    friend class BaseNode<T, _Type, Allocator>;
};

template<class T, class Allocator>
template <class UpdateOp>
void
Node<T, bool, Allocator>::update(const UpdateOp& updateOp) {
    if (!this->isLeaf()) {
        if (_toUpdate) {
            this->getLeft()->update(updateOp);  // update left with old update
            this->getRight()->update(updateOp); // update right with old update
        }
        _toUpdate = true;
    } else { // isLeaf()
        this->_value = updateOp(this->getValue());
    }
}

template<class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::siftOptUpdate(const UpdateOp& updateOp) {
    if (_toUpdate) {
        assert(!this->isLeaf() && "It nust not be a leaf.");
        this->getLeft()->update(updateOp);
        this->getRight()->update(updateOp);
        _toUpdate = false;
    }
}

template<class T, class Allocator>
class Node<T, void, Allocator>
        : public BaseNode<T, Node<T, void, Allocator>, Allocator> {
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
