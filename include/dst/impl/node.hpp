////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copySubtree at https://www.boost.org/LICENSE_1_0.txt)

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
    : public BaseNode<Node<T, std::optional<UpdateT>, Allocator>> {
 private:
  using This_ = Node<T, std::optional<UpdateT>, Allocator>;
  using Base_ = BaseNode<This_>;
  using Alloc_ = std::allocator_traits<Allocator>::template rebind_alloc<This_>;
  using AllocatorTraits_ = std::allocator_traits<Alloc_>;

 public:
  Node() = default;
  Node(const Node&) = delete;
  Node(Node&&) noexcept = delete;

  Node& operator=(const Node& other) = delete;
  Node& operator=(Node&& other) noexcept = delete;

 public:
  template <class UpdateOp, class UpdateT1>
    requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
  void update(const UpdateOp& updateOp, UpdateT1&& update, Alloc_& allocator);

  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp, Alloc_& allocator);

  /**
   * @brief Create children sifting down the value. Has strong exception safety
   * guarantee. Update must be applied to a value before calling (this
   * pre-condition is checked in debug mode).
   *
   * @param allocator
   */
  void initChildrenSiftingValue(Alloc_& allocator);

 public:
  /**
   * @brief Copy subtree from `src` to `dest`, using `nodeAlloc` for newly added
   * nodes creation.
   *
   * @param src source subtree root
   * @param dest destination subtree root
   * @param alloc allocator for newly created nodes
   */
  static void copySubtree(const Node& src, Node* dest, Alloc_& alloc);

  /**
   * @brief Copy subtree from `src` to `dest`, using `nodeAlloc` for newly added
   * nodes creation.
   *
   * `dest` is a constructed `Node`.
   * `dest` is expected not to have children yet. So `dest` is leaf. This is
   * checked with `assert`.
   * `dest`'s value is expected to be not constructed. We are unable to check
   * this, so accuracy is needed not to construct value at `dest` twice.
   *
   * @param src source subtree root
   * @param dest destination subtree root
   * @param alloc allocator for newly created nodes
   */
  static void copyToNewlyCreated(const Node& src, Node* dest, Alloc_& alloc);

  ~Node() = default;

 private:
  std::optional<UpdateT> updateValue_;

 private:
  friend class BaseNode<This_>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::copySubtree(
    const Node& src, Node* dest, Alloc_& nodeAlloc) {
  Base_::copySubtree(src, dest, nodeAlloc);
  dest->updateValue_ = src.updateValue_;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::copyToNewlyCreated(
    const Node& src, Node* dest, Alloc_& alloc) {
  Base_::copyToNewlyCreated(src, dest, alloc);
  dest->updateValue_ = src.updateValue_;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp, class UpdateT1>
  requires std::is_same_v<std::remove_cvref_t<UpdateT1>, UpdateT>
void Node<T, std::optional<UpdateT>, Allocator>::update(
    const UpdateOp& updateOp, UpdateT1&& updateVal, Alloc_& allocator) {
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
    assert(!updateValue_.has_value());
    Base_::getValue() =
        updateOp(Base_::getValue(), std::forward<UpdateT1>(updateVal));
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
template <class UpdateOp>
void Node<T, std::optional<UpdateT>, Allocator>::siftOptUpdate(
    const UpdateOp& updateOp, Alloc_& allocator) {
  if (updateValue_.has_value()) {
    assert(!Base_::isLeaf() && "It must not be a leaf.");
    Base_::getLeft()->update(updateOp, *updateValue_, allocator);
    Base_::getRight()->update(updateOp, std::move(*updateValue_), allocator);
    updateValue_.reset();
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class UpdateT, class Allocator>
void Node<T, std::optional<UpdateT>, Allocator>::initChildrenSiftingValue(
    Alloc_& alloc) {
  assert(!updateValue_.has_value() && "Update must be applied.");
  Base_::initChildrenSiftingValue(alloc);
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, bool, Allocator> : public BaseNode<Node<T, bool, Allocator>> {
 private:
  using This_ = Node<T, bool, Allocator>;
  using Base_ = BaseNode<This_>;
  using Alloc_ = std::allocator_traits<Allocator>::template rebind_alloc<This_>;

 public:
  Node() = default;

  template <class UpdateOp>
  void update(const UpdateOp& updateOp, Alloc_& allocator);
  template <class UpdateOp>
  void siftOptUpdate(const UpdateOp& updateOp, Alloc_& allocator);

  ~Node() = default;

 public:
  static void copySubtree(const Node& src, Node* dest, Alloc_& nodeAllocator);

  static void copyToNewlyCreated(const Node& src, Node* dest,
                                 Alloc_& nodeAlloc);

 private:
  bool toUpdate_{false};

 private:
  friend class BaseNode<This_>;
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::update(const UpdateOp& updateOp, Alloc_& alloc) {
  if (!Base_::isLeaf()) {
    if (toUpdate_) {
      Base_::getLeft()->update(updateOp, alloc);
      Base_::getRight()->update(updateOp, alloc);
    }
    toUpdate_ = true;
  } else {  // isLeaf()
    assert(!toUpdate_);
    Base_::getValue() = updateOp(Base_::getValue());
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
template <class UpdateOp>
void Node<T, bool, Allocator>::siftOptUpdate(const UpdateOp& updateOp,
                                             Alloc_& allocator) {
  if (toUpdate_) {
    assert(!Base_::isLeaf() && "It must not be a leaf.");
    Base_::getLeft()->update(updateOp, allocator);
    Base_::getRight()->update(updateOp, allocator);
    toUpdate_ = false;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
void Node<T, bool, Allocator>::copySubtree(const Node& src, Node* dest,
                                           Alloc_& nodeAlloc) {
  Base_::copySubtree(src, dest, nodeAlloc);
  dest->toUpdate_ = src.toUpdate_;
}

template <class T, class Allocator>
void Node<T, bool, Allocator>::copyToNewlyCreated(const Node& src, Node* dest,
                                                  Alloc_& nodeAlloc) {
  Base_::copyToNewlyCreated(src, dest, nodeAlloc);
  dest->toUpdate_ = src.toUpdate_;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Allocator>
class Node<T, void, Allocator> : public BaseNode<Node<T, void, Allocator>> {
 private:
  using This_ = Node<T, void, Allocator>;
  using Base_ = BaseNode<This_>;
  using Alloc_ = std::allocator_traits<Allocator>::template rebind_alloc<This_>;

 public:
  Node() = default;

  static void copySubtree(const Node& src, Node* dest, Alloc_& nodeAllocator) {
    Base_::copySubtree(src, dest, nodeAllocator);
  }

  static void copyToNewlyCreated(const Node& src, Node* dest,
                                 Alloc_& nodeAlloc) {
    Base_::copyToNewlyCreated(src, dest, nodeAlloc);
  }

  ~Node() = default;

  /**
   * @brief Set the value to node. Clears children if they exist.
   *
   * @param value value to set.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue(ValueT&& value, Alloc_& allocator);

 private:
  friend class BaseNode<This_>;
};

}  // namespace dst::impl

#endif  // NODE_HPP
