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
  using This_ = Node<T, std::optional<UpdateT>, Allocator>;
  using Base_ = BaseNode<T, This_, Allocator>;
  using Allocator_ = Base_::Allocator_;

 public:
  explicit Node(const T& value) : Base_(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : Base_(std::move(value)) {
  }

  Node(const Node&) = delete;

  Node(const Node& node, Allocator_& allocator);

  Node(Node&& other) noexcept = default;

  Node& operator=(const Node& other) = delete;

  Node& assign(const Node& other, Allocator_& allocator);

  Node& operator=(Node&& other) noexcept;

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue(ValueT&& value, Allocator_& allocator);

  template <class UpdateOp, class UpdateT1>
    requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
  void update(const UpdateOp& updateOp, UpdateT1&& update,
              Allocator_& allocator);
  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp, Allocator_& allocator);

  ~Node() = default;

 private:
  std::optional<UpdateT> updateValue_;

 private:
  friend class BaseNode<T, This_, Allocator>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
Node<T, std::optional<UpdateT>, Allocator>::Node(const Node& node,
                                                 Allocator_& allocator)
    : Base_(node, allocator), updateValue_(node.updateValue_) {
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
auto Node<T, std::optional<UpdateT>, Allocator>::assign(const This_& other,
                                                        Allocator_& allocator)
    -> This_& {
  assert(&other != this && "Node must not be assigned to itself.");
  updateValue_ = other.updateValue_;
  Base_::assign(other, allocator);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
auto Node<T, std::optional<UpdateT>, Allocator>::operator=(
    This_&& other) noexcept -> This_& {
  assert(&other != this && "Node must not be assigned to itself.");
  std::swap(updateValue_, other.updateValue_);
  Base_::operator=(std::move(other));
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class ValueT>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void Node<T, std::optional<UpdateT>, Allocator>::setValue(
    ValueT&& value, Allocator_& allocator) {
  Base_::setValue_(std::forward<ValueT>(value), allocator);
  updateValue_.reset();
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp, class UpdateT1>
  requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
void Node<T, std::optional<UpdateT>, Allocator>::update(
    const UpdateOp& updateOp, UpdateT1&& updateVal, Allocator_& allocator) {
  if (!Base_::isLeaf()) {
    if (updateValue_.has_value()) {
      // update left with old update
      Base_::getLeft()->update(updateOp, *updateValue_, allocator);
      // update right with old update
      Base_::getRight()->update(updateOp, std::move(*updateValue_), allocator);
    }
    // _value continues to have delayed update meaning.
    updateValue_ = std::forward<UpdateT1>(updateVal);
  } else {  // isLeaf()
    assert(Base_::hasValue() && "Leaf must have a value.");
    assert(!updateValue_.has_value());
    Base_::setValue_(
        updateOp(Base_::getValue(), std::forward<UpdateT1>(updateVal)),
        allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp>
void Node<T, std::optional<UpdateT>, Allocator>::siftOptUpdate(
    const UpdateOp& updateOp, Allocator_& allocator) {
  if (updateValue_.has_value()) {
    assert(!Base_::isLeaf() && "It must not be a leaf.");
    Base_::getLeft()->update(updateOp, *updateValue_, allocator);
    Base_::getRight()->update(updateOp, std::move(*updateValue_), allocator);
    updateValue_.reset();
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, bool, Allocator>
    : public BaseNode<T, Node<T, bool, Allocator>, Allocator> {
 private:
  using This_ = Node<T, bool, Allocator>;
  using Base_ = BaseNode<T, This_, Allocator>;
  using Allocator_ = Base_::Allocator_;

 public:
  Node() = default;
  explicit Node(const T& value) : Base_(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : Base_(std::move(value)) {
  }
  Node(const Node&) = delete;
  Node(const Node& node, Allocator_ allocator)
      : Base_(node, allocator), toUpdate_{node.toUpdate_} {
  }
  Node(Node&& other) noexcept = default;
  Node& operator=(const Node& other) = delete;
  Node& assign(const Node& other, Allocator_& allocator);
  Node& operator=(Node&& other) noexcept;

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue(ValueT&& value, Allocator_& allocator);

  template <class UpdateOp>
  void update(const UpdateOp& updateOp, Allocator_& allocator);
  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp, Allocator_& allocator);

  ~Node() = default;

 private:
  bool toUpdate_{false};

 private:
  friend class BaseNode<T, This_, Allocator>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
auto Node<T, bool, Allocator>::assign(const Node& other, Allocator_& allocator)
    -> This_& {
  assert(&other != this && "Node must not be assigned to itself.");
  toUpdate_ = other.toUpdate_;
  return static_cast<This_&>(Base_::assign(other, allocator));
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
auto Node<T, bool, Allocator>::operator=(Node&& other) noexcept -> This_& {
  assert(&other != this && "Node must not be assigned to itself.");
  std::swap(toUpdate_, other.toUpdate_);
  return static_cast<This_&>(Base_::operator=(std::move(other)));  // NOLINT
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class ValueT>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void Node<T, bool, Allocator>::setValue(ValueT&& value, Allocator_& allocator) {
  Base_::setValue_(std::forward<ValueT>(value), allocator);
  toUpdate_ = false;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::update(const UpdateOp& updateOp,
                                      Allocator_& allocator) {
  if (!Base_::isLeaf()) {
    if (toUpdate_) {
      Base_::getLeft()->update(updateOp,
                               allocator);  // update left with old update
      Base_::getRight()->update(updateOp,
                                allocator);  // update right with old update
    }
    toUpdate_ = true;
  } else {  // isLeaf()
    assert(Base_::hasValue());
    assert(!toUpdate_);
    Base_::setValue_(updateOp(Base_::getValue()), allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::siftOptUpdate(const UpdateOp& updateOp,
                                             Allocator_& allocator) {
  if (toUpdate_) {
    assert(!Base_::isLeaf() && "It must not be a leaf.");
    Base_::getLeft()->update(updateOp, allocator);
    Base_::getRight()->update(updateOp, allocator);
    toUpdate_ = false;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, void, Allocator>
    : public BaseNode<T, Node<T, void, Allocator>, Allocator> {
 private:
  using This_ = Node<T, void, Allocator>;
  using Base_ = BaseNode<T, This_, Allocator>;
  using Allocator_ = Base_::Allocator_;

 public:
  Node() = default;
  explicit Node(const T& value) : Base_(value) {
  }
  explicit Node(T&& value) noexcept
    requires std::move_constructible<T>
      : Base_(std::move(value)) {
  }

  Node(const Node&) = delete;

  Node(const Node& other, Allocator_& allocator) : Base_(other, allocator) {
  }

  Node(Node&& other) noexcept = default;

  Node& operator=(const Node& other) = delete;

  Node& assign(const Node& other, Allocator_& allocator);

  Node& operator=(Node&& other) noexcept;

  ~Node() = default;

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue(ValueT&& value, Allocator_& allocator);

 private:
  friend class BaseNode<T, Node<T, void, Allocator>, Allocator>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
auto Node<T, void, Allocator>::assign(const This_& other, Allocator_& allocator)
    -> This_& {
  assert(&other != this && "Node must not be assigned to itself.");
  return static_cast<This_&>(Base_::assign(other, allocator));
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
auto Node<T, void, Allocator>::operator=(This_&& other) noexcept -> This_& {
  return static_cast<This_&>(Base_::operator=(std::move(other)));  // NOLINT
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class ValueT>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void Node<T, void, Allocator>::setValue(ValueT&& value, Allocator_& allocator) {
  Base_::setValue_(std::forward<ValueT>(value), allocator);
}

}  // namespace dst::impl

#endif  // NODE_HPP
