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

template <class Derived>
class BaseNode;

////////////////////////////////////////////////////////////////////////////////
/// \brief The BaseNode class
///
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
class BaseNode<Derived<T, UpdateT, Allocator>> {
 protected:
  using This_ = BaseNode<Derived<T, UpdateT, Allocator>>;
  using Derived_ = Derived<T, UpdateT, Allocator>;
  using AllocatorForDerived_ =
      std::allocator_traits<Allocator>::template rebind_alloc<Derived_>;
  using AllocatorTraits_ = std::allocator_traits<AllocatorForDerived_>;

 public:
  explicit BaseNode(const T& value) : value_(value) {
  }
  explicit BaseNode(T&& value) noexcept
    requires std::move_constructible<T>
      : value_(std::move(value)) {
  }

  BaseNode(const BaseNode&) = delete;

  BaseNode(const BaseNode& other, AllocatorForDerived_& allocator);

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
  BaseNode& assign(const BaseNode& other, AllocatorForDerived_& allocator);

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

  template <class AllocatorForDerived>
  void initChildren(AllocatorForDerived& allocator);

  template <class AllocatorForDerived>
  void clearChildren(AllocatorForDerived& allocator);

  [[nodiscard]] Derived_* getLeft() const {
    return ptr_;
  }

  [[nodiscard]] Derived_* getRight() const {
    return ptr_ + 1;  // NOLINT
  }

 protected:
  /**
   * @brief Set value to node making a copy.
   * @param value value reference.
   */
  template <class ValueT, class AllocatorForDerived>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue_(ValueT&&, AllocatorForDerived& allocator);

  ~BaseNode();

 private:
  void destructChildrenRecursive_();

 private:
  std::optional<T> value_;
  Derived_* ptr_{nullptr};
};

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
BaseNode<Derived<T, UpdateT, Allocator>>::BaseNode(
    const BaseNode& other, AllocatorForDerived_& allocator)
    : value_(other.value_),
      ptr_(other.isLeaf() ? nullptr
                          : AllocatorTraits_::allocate(allocator, 2)) {
  if (!isLeaf()) {
    std::construct_at(getLeft(), *other.getLeft(), allocator);
    std::construct_at(getRight(), *other.getRight(), allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
auto BaseNode<Derived<T, UpdateT, Allocator>>::assign(
    const BaseNode& other, AllocatorForDerived_& allocator) -> This_& {
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
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
auto BaseNode<Derived<T, UpdateT, Allocator>>::operator=(
    BaseNode&& other) noexcept -> This_& {
  assert(&other != this && "Assign operator must not be called on itself.");
  std::swap(value_, other.value_);
  std::swap(ptr_, other.ptr_);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
template <class ValueT, class AllocForDerived>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void BaseNode<Derived<T, UpdateT, Allocator>>::setValue_(
    ValueT&& value, AllocForDerived& allocator) {
  value_ = std::forward<ValueT>(value);
  if (!this->isLeaf()) {
    clearChildren(allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
template <class AllocForDerived>
void BaseNode<Derived<T, UpdateT, Allocator>>::initChildren(
    AllocForDerived& allocator) {
  assert(isLeaf() && "Can only init children for a leaf.");
  auto nodesPtr =
      std::allocator_traits<AllocForDerived>::allocate(allocator, 2);

  assert(value_.has_value() && "No value to set to children.");
  try {
    ptr_ = nodesPtr;
    std::construct_at(getLeft(), *value_);
  } catch (...) {
    std::allocator_traits<AllocForDerived>::deallocate(allocator, ptr_, 2);
    ptr_ = nullptr;
    throw;
  }
  try {
    std::construct_at(getRight(), std::move(*value_));
  } catch (...) {
    getLeft()->~Derived_();
    std::allocator_traits<AllocForDerived>::deallocate(allocator, ptr_, 2);
    ptr_ = nullptr;
    throw;
  }
  value_.reset();
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
template <class AllocForDerived>
inline void BaseNode<Derived<T, UpdateT, Allocator>>::clearChildren(
    AllocForDerived& allocator) {
  if (!getLeft()->isLeaf()) {
    getLeft()->clearChildren(allocator);
  }
  if (!getRight()->isLeaf()) {
    getRight()->clearChildren(allocator);
  }
  std::allocator_traits<AllocForDerived>::destroy(allocator, getRight());
  std::allocator_traits<AllocForDerived>::destroy(allocator, getLeft());
  std::allocator_traits<AllocForDerived>::deallocate(allocator, ptr_, 2);
  ptr_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
template <template <class...> class Derived, class T, class UpdateT,
          class Allocator>
BaseNode<Derived<T, UpdateT, Allocator>>::~BaseNode() {
  assert(isLeaf());
}

}  // namespace dst::impl

#endif  // NODE_BASE_HPP
