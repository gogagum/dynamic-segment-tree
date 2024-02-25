////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef NODE_HPP
#define NODE_HPP

#include <cassert>
#include <dst/impl/node_base.hpp>
#include <memory>
#include <optional>
#include <type_traits>

namespace dst::impl {

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator = std::allocator<T>>
class Node;

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
class Node<T, std::optional<UpdateT>, Allocator>
    : public BaseNode<T, Node<T, std::optional<UpdateT>, Allocator>,
                      Allocator> {
 private:
  using Type_ = Node<T, std::optional<UpdateT>, Allocator>;
  using Base_ = BaseNode<T, Type_, Allocator>;

 public:
  explicit Node(const T& value) : Base_(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : Base_(std::move(value)) {
  }

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  void setValue(const T& value);

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  void setValue(T&& value);
  void setUpdateValue(const UpdateT& updateValue);
  template <class UpdateOp, class UpdateT1>
    requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
  void update(const UpdateOp& updateOp, UpdateT1&& update);
  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp);

 private:
  std::optional<UpdateT> updateValue_;

 private:
  friend class BaseNode<T, Type_, Allocator>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::setValue(const T& value) {
  Base_::setValue(value);
  updateValue_ = std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::setValue(T&& value) {
  Base_::setValue(std::move(value));
  updateValue_ = std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp, class UpdateT1>
  requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
void Node<T, std::optional<UpdateT>, Allocator>::update(
    const UpdateOp& updateOp, UpdateT1&& updateVal) {
  if (!this->isLeaf()) {
    if (this->updateValue_.has_value()) {
      // update left with old update
      this->getLeft()->update(updateOp, *this->updateValue_);
      // update right with old update
      this->getRight()->update(updateOp, std::move(*this->updateValue_));
    }
    // _value continues to have delayed update meaning.
    this->updateValue_ = updateVal;
  } else {  // isLeaf()
    assert(this->hasValue() && "Leaf must have a value.");
    this->setValue(
        updateOp(this->getValue(), std::forward<UpdateT1>(updateVal)));
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp>
void Node<T, std::optional<UpdateT>, Allocator>::siftOptUpdate(
    const UpdateOp& updateOp) {
  if (this->updateValue_.has_value()) {
    assert(!this->isLeaf() && "It must not be a leaf.");
    this->getLeft()->update(updateOp, *this->updateValue_);
    this->getRight()->update(updateOp, std::move(*this->updateValue_));
    this->updateValue_ = std::nullopt;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::setUpdateValue(
    const UpdateT& updateValue) {
  assert(!this->isLeaf() && "Can`t set update value for a leaf.");
  updateValue_ = updateValue;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, bool, Allocator>
    : public BaseNode<T, Node<T, bool, Allocator>, Allocator> {
 private:
  using Type_ = Node<T, bool, Allocator>;
  using Base_ = BaseNode<T, Type_, Allocator>;

 public:
  Node() = default;
  explicit Node(const T& value) : Base_(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : Base_(std::move(value)) {
  }
  void addUpdate();
  template <class UpdateOp>
  void update(const UpdateOp& updateOp);
  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp);

 private:
  bool toUpdate_{false};

 private:
  friend class BaseNode<T, Type_, Allocator>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::update(const UpdateOp& updateOp) {
  if (!this->isLeaf()) {
    if (toUpdate_) {
      this->getLeft()->update(updateOp);   // update left with old update
      this->getRight()->update(updateOp);  // update right with old update
    }
    toUpdate_ = true;
  } else {  // isLeaf()
    this->setValue(updateOp(this->getValue()));
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::siftOptUpdate(const UpdateOp& updateOp) {
  if (toUpdate_) {
    assert(!this->isLeaf() && "It must not be a leaf.");
    this->getLeft()->update(updateOp);
    this->getRight()->update(updateOp);
    toUpdate_ = false;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, void, Allocator>
    : public BaseNode<T, Node<T, void, Allocator>, Allocator> {
 private:
  using Type_ = Node<T, void, Allocator>;
  using Base_ = BaseNode<T, Type_, Allocator>;

 public:
  Node() = default;
  explicit Node(const T& value) : BaseNode<T, Type_, Allocator>(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : BaseNode<T, Type_, Allocator>(std::move(value)) {
  }

 private:
  friend class BaseNode<T, Node<T, void, Allocator>, Allocator>;
};

}  // namespace dst::impl

#endif  // NODE_HPP
