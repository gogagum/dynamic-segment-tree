////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copySubtree at https://www.boost.org/LICENSE_1_0.txt)

#ifndef NODE_BASE_HPP
#define NODE_BASE_HPP

#include <concepts>
#include <memory>
#include <optional>

namespace dst::impl {

template <class Derived>
class BaseNode;

////////////////////////////////////////////////////////////////////////////////
/// \brief The BaseNode class
///
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
class BaseNode<Derived<T, UpdateT, Allocator>> {  // NOLINT
 protected:
  using This_ = BaseNode<Derived<T, UpdateT, Allocator>>;
  using Derived_ = Derived<T, UpdateT, Allocator>;
  using AllocForDerived_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Derived_>;
  using AllocTraits_ = std::allocator_traits<AllocForDerived_>;

 public:
  BaseNode() noexcept = default;
  BaseNode(const BaseNode& other) = delete;
  BaseNode(BaseNode&& other) noexcept = delete;

  BaseNode& operator=(const BaseNode& other) = delete;
  BaseNode& operator=(BaseNode&& other) = delete;

  /**
   * @brief sets value for a leaf, or clear children and assign value.
   *
   * @param val - value to set
   */
  template <class T1>
  void setOrConstructValue(T1&& val, AllocForDerived_& alloc);

  /**
   * @brief Get value reference.
   * @return value reference.
   */
  T& getValue() {
    return *std::launder(reinterpret_cast<T*>(&buffer_));  // NOLINT
  }

  /**
   * @brief Get const value reference.
   * @return value reference.
   */
  [[nodiscard]] const T& getValue() const {
    return *std::launder(reinterpret_cast<T const*>(&buffer_));  // NOLINT
  }

  /**
   * @brief Get value pointer.
   * @return pointer to a buffer.
   */
  T* getValuePtr() {
    return std::addressof(getValue());
  }

  /**
   * @brief Chick if node is a leaf.
   */
  [[nodiscard]] bool isLeaf() const {
    return !ptr;
  }

  /**
   * @brief Initializes children with no values. Exceptions can come only from
   * allocation. If this happened, function gives no effect.
   *
   * @param alloc allocator to allocate nodes.
   */
  void initEmptyChildren(AllocForDerived_& alloc);

  /**
   * @brief clears children destroying nodes, deallocating memory and setting
   * children pointer to `nullptr`.
   *
   * @param alloc allocator do deallocate nodes.
   */
  void rollbackEmptyChildrenInit(AllocForDerived_& alloc);

  /**
   * @brief split leaf.
   * Pre: node is a leaf node with constructed value.
   * Post: one of two:
   * - Node has two children with values constructed as a copy of its own value.
   * - Exception on allocation or copy construction was thrown and there is no
   * effect on the node.
   *
   * @param alloc allocator to allocate newly created nodes.
   */
  void initChildrenSiftingValue(AllocForDerived_& alloc);

  /**
   * @brief clear children for a node, which is a root of a correctly
   * initialized subtree.
   *
   * @param alloc allocator, to call deallocate for node s in subtree.
   */
  void clearChildren(AllocForDerived_& alloc);

  [[nodiscard]] Derived_* getLeft() const {
    return ptr;
  }

  [[nodiscard]] Derived_* getRight() const {
    return ptr + 1;  // NOLINT
  }

 public:
  static void copyChildren(const Derived_& src, Derived_* dest,
                           AllocForDerived_& nodeAlloc);

  static void copySubtree(const Derived_& src, Derived_* dest,
                          AllocForDerived_& nodeAlloc);

  static void copyToNewlyCreated(const Derived_& src, Derived_* dest,
                                 AllocForDerived_& nodeAlloc);

  static void copyChildrenToNewlyCreated(const Derived_& src, Derived_* dest,
                                         AllocForDerived_& alloc);

 public:
  Derived_* ptr{nullptr};  // NOLINT

 protected:
  ~BaseNode();

 private:
  void destructChildrenRecursive_();

 private:
  alignas(T) char buffer_[sizeof(T)];  // NOLINT
};

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
template <class T1>
void BaseNode<Derived<T, UpdateT, Allocator>>::setOrConstructValue(
    T1&& val, AllocForDerived_& alloc) {
  if (isLeaf()) {
    getValue() = std::forward<T1>(val);
  } else {
    std::construct_at(getValuePtr(), std::forward<T1>(val));
    clearChildren(alloc);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::initEmptyChildren(
    AllocForDerived_& alloc) {
  ptr = AllocTraits_::allocate(alloc, 2);
  AllocTraits_::construct(alloc, getLeft());
  AllocTraits_::construct(alloc, getRight());
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::rollbackEmptyChildrenInit(
    AllocForDerived_& alloc) {
  AllocTraits_::destroy(alloc, getRight());
  AllocTraits_::destroy(alloc, getLeft());
  AllocTraits_::deallocate(alloc, ptr, 2);
  ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::initChildrenSiftingValue(
    AllocForDerived_& alloc) {
  assert(isLeaf() && "Can only init children for a leaf.");
  auto* const nodesPtr = AllocTraits_::allocate(alloc, 2);  // Can throw
  ptr = nodesPtr;
  std::construct_at(getLeft());
  try {
    std::construct_at(getLeft()->getValuePtr(), getValue());
  } catch (...) {
    std::destroy_at(getLeft());
    AllocTraits_::deallocate(alloc, ptr, 2);
    ptr = nullptr;
    throw;
  }
  std::construct_at(getRight());
  try {
    if constexpr (std::move_constructible<T>) {
      std::construct_at(getRight()->getValuePtr(), std::move(getValue()));
    } else {
      std::construct_at(getRight()->getValuePtr(), getValue());
    }
  } catch (...) {
    std::destroy_at(getLeft()->getValuePtr());
    std::destroy_at(getLeft());
    std::destroy_at(getRight());
    AllocTraits_::deallocate(alloc, ptr, 2);
    ptr = nullptr;
    throw;
  }
  std::destroy_at(getValuePtr());
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
inline void BaseNode<Derived<T, UpdateT, Allocator>>::clearChildren(
    AllocForDerived_& allocator) {
  if (!getLeft()->isLeaf()) {
    getLeft()->clearChildren(allocator);
  } else {
    std::destroy_at(getLeft()->getValuePtr());
  }
  if (!getRight()->isLeaf()) {
    getRight()->clearChildren(allocator);
  } else {
    std::destroy_at(getRight()->getValuePtr());
  }
  AllocTraits_::destroy(allocator, getRight());
  AllocTraits_::destroy(allocator, getLeft());
  AllocTraits_::deallocate(allocator, ptr, 2);
  ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::copyChildren(
    const Derived_& src, Derived_* dest, AllocForDerived_& nodeAlloc) {
  Derived_::copySubtree(*src.getLeft(), dest->getLeft(), nodeAlloc);
  Derived_::copySubtree(*src.getRight(), dest->getRight(), nodeAlloc);
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::copySubtree(
    const Derived_& src, Derived_* dest, AllocForDerived_& nodeAlloc) {
  if (dest->isLeaf()) {
    if (src.isLeaf()) {
      dest->getValue() = src.getValue();  // That's all!
    } else {
      std::destroy_at(dest->getValuePtr());
      // dest is like newly created
      copyChildrenToNewlyCreated(src, dest, nodeAlloc);
    }
  } else {
    if (src.isLeaf()) {
      dest->clearChildren(nodeAlloc);
      // Dest must not have value here
      std::construct_at(dest->getValuePtr(), src.getValue());
    } else {
      // No values in both, both have children
      copyChildren(src, dest, nodeAlloc);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::copyToNewlyCreated(
    const Derived_& src, Derived_* dest, AllocForDerived_& nodeAlloc) {
  assert(dest->isLeaf());
  if (src.isLeaf()) {
    std::construct_at(dest->getValuePtr(), src.getValue());
  } else {
    copyChildrenToNewlyCreated(src, dest, nodeAlloc);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
void BaseNode<Derived<T, UpdateT, Allocator>>::copyChildrenToNewlyCreated(
    const Derived_& src, Derived_* dest, AllocForDerived_& nodeAlloc) {
  assert(dest->isLeaf());
  assert(!src.isLeaf());
  dest->initEmptyChildren(nodeAlloc);
  try {
    Derived_::copyToNewlyCreated(*src.getLeft(), dest->getLeft(), nodeAlloc);
  } catch (...) {
    dest->rollbackEmptyChildrenInit(nodeAlloc);
    throw;
  }

  try {
    Derived_::copyToNewlyCreated(*src.getRight(), dest->getRight(), nodeAlloc);
  } catch (...) {
    if (auto* left = dest->getLeft(); left->isLeaf()) {
      std::destroy_at(left->getValuePtr());
    } else {
      left->clearChildren(nodeAlloc);
    }
    dest->rollbackEmptyChildrenInit(nodeAlloc);
    throw;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
BaseNode<Derived<T, UpdateT, Allocator>>::~BaseNode() {
  assert(isLeaf());
}

}  // namespace dst::impl

#endif  // NODE_BASE_HPP
