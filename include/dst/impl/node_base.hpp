////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef NODE_BASE_HPP
#define NODE_BASE_HPP

#include <concepts>
#include <memory>
#include <optional>

namespace dst::impl {

////////////////////////////////////////////////////////////////////////////////
/// \brief The BaseNode class
///
template <class T, class Derived, class Allocator>
class BaseNode {
 protected:
  using This_ = BaseNode<T, Derived, Allocator>;
  using Allocator_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Derived>;
  using AllocatorTraits_ = std::allocator_traits<Allocator_>;

 public:
  explicit BaseNode(const T& value) : value_(value) {
  }
  explicit BaseNode(T&& value) noexcept
    requires std::move_constructible<T>
      : value_(std::move(value)) {
  }

  BaseNode(const BaseNode&) = delete;

  BaseNode(const BaseNode& other, Allocator_& allocator);

  BaseNode(BaseNode&& other) noexcept
      : value_(std::move(other.value_)), ptr_{other.ptr_} {
    other.ptr_ = nullptr;
  }

 public:
  /**
   * @brief Deleted copy assign operator, as allocator is needed to allocate
   * subtree. (See `assign(const BaseNode& other, Allocator_& allocator)`).
   */
  BaseNode& operator=(const BaseNode&) = delete;

  /**
   * @brief Copy assign a Node, using a specified allocator, to allocate memory
   * for a subtree.
   *
   * @param other node to copy from.
   * @param allocator stl compatible allocator.
   * @return BaseNode& reference to itself.
   */
  BaseNode& assign(const BaseNode& other, Allocator_& allocator);

  /**
   * @brief Move assign operator.
   *
   * @param other node to move from.
   * @param allocator std compatible allocator.
   * @return BaseNode& reference to itself.
   */
  BaseNode& operator=(BaseNode&&) noexcept;

  /**
   * @brief Get const value reference.
   * @return value reference.
   */
  [[nodiscard]] const T& getValue() const {
    assert(hasValue());
    return *value_;
  }

  [[nodiscard]] bool hasValue() const {
    return value_.has_value();
  }

  [[nodiscard]] bool isLeaf() const {
    return !ptr_;
  }

  void initChildren(Allocator_& allocator);

  void clearChildren(Allocator_& allocator);

  [[nodiscard]] Derived* getLeft() const {
    return ptr_;
  }

  [[nodiscard]] Derived* getRight() const {
    return ptr_ + 1;  // NOLINT
  }

 protected:
  /**
   * @brief Set value to node making a copy.
   * @param value value reference.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue_(ValueT&&, Allocator_& allocator);

  ~BaseNode();

 private:
  void destructChildrenRecursive_();

 private:
  std::optional<T> value_;
  Derived* ptr_{nullptr};
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::BaseNode(const BaseNode& other,
                                          Allocator_& allocator)
    : value_(other.value_),
      ptr_(other.isLeaf() ? nullptr
                          : AllocatorTraits_::allocate(allocator, 2)) {
  if (!isLeaf()) {
    std::construct_at(getLeft(), *other.getLeft(), allocator);
    std::construct_at(getRight(), *other.getRight(), allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
auto BaseNode<T, Derived, Allocator>::assign(const BaseNode& other,
                                             Allocator_& allocator) -> This_& {
  assert(&other != this && "Assign operator must not be called on itself.");
  value_ = other.value_;
  if (nullptr != ptr_) {
    clearChildren(allocator);
  }
  if (other.isLeaf()) {
    ptr_ = nullptr;
  } else {
    ptr_ = AllocatorTraits_::allocate(allocator, 2);
    std::construct_at(getLeft(), *other.getLeft(), allocator);
    std::construct_at(getRight(), *other.getRight(), allocator);
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
auto BaseNode<T, Derived, Allocator>::operator=(BaseNode&& other) noexcept
    -> This_& {
  assert(&other != this && "Assign operator must not be called on itself.");
  std::swap(value_, other.value_);
  std::swap(ptr_, other.ptr_);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
template <class ValueT>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void BaseNode<T, Derived, Allocator>::setValue_(ValueT&& value,
                                                Allocator_& allocator) {
  value_ = std::forward<ValueT>(value);
  if (!this->isLeaf()) {
    clearChildren(allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::initChildren(Allocator_& allocator) {
  assert(isLeaf() && "Can only init children for a leaf.");
  auto nodesPtr = AllocatorTraits_::allocate(allocator, 2);

  assert(value_.has_value() && "No value to set to children.");
  try {
    ptr_ = nodesPtr;
    std::construct_at(getLeft(), *value_);
  } catch (...) {
    AllocatorTraits_::deallocate(allocator, ptr_, 2);
    ptr_ = nullptr;
    throw;
  }
  try {
    std::construct_at(getRight(), std::move(*value_));
  } catch (...) {
    getLeft()->~Derived();
    AllocatorTraits_::deallocate(allocator, ptr_, 2);
    ptr_ = nullptr;
    throw;
  }
  value_.reset();
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
inline void BaseNode<T, Derived, Allocator>::clearChildren(
    Allocator_& allocator) {
  if (!getLeft()->isLeaf()) {
    getLeft()->clearChildren(allocator);
  }
  if (!getRight()->isLeaf()) {
    getRight()->clearChildren(allocator);
  }
  AllocatorTraits_::destroy(allocator, getRight());
  AllocatorTraits_::destroy(allocator, getLeft());
  AllocatorTraits_::deallocate(allocator, ptr_, 2);
  ptr_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::~BaseNode() {
  assert(isLeaf());
}

}  // namespace dst::impl

#endif  // NODE_BASE_HPP
