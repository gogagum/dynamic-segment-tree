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
  BaseNode(const BaseNode&) = default;
  BaseNode(BaseNode&&) noexcept = default;

 public:
  BaseNode& operator=(const BaseNode&) = default;
  BaseNode& operator=(BaseNode&&) noexcept = default;
  [[nodiscard]] const T& getValue() const {
    assert(value_.has_value());
    return *value_;
  }

  /**
   * @brief Set value to node making a copy.
   * @param value value reference.
   */
  template <class ValueT>
    requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
  void setValue(ValueT&&, Allocator_& allocator);

  [[nodiscard]] bool hasValue() const {
    return value_.has_value();
  }
  void setNullValue() {
    value_ = std::nullopt;
  }
  [[nodiscard]] bool isLeaf() const {
    return !left_ && !right_;
  };
  void initChildren(Allocator_& allocator);
  [[nodiscard]] Derived* getLeft() const {
    return left_;
  }
  [[nodiscard]] Derived* getRight() const {
    return right_;
  }
  ~BaseNode();

  void clearChildren(Allocator_& allocator);

 private:
  void destructChildrenRecursive_();

 private:
  std::optional<T> value_;
  Derived* left_{nullptr};
  Derived* right_{nullptr};
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
template <class ValueT>
  requires std::is_same_v<std::remove_cvref_t<ValueT>, T>
void BaseNode<T, Derived, Allocator>::setValue(ValueT&& value,
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
    left_ = nodesPtr;
    std::construct_at(left_, *value_);
  } catch (const std::exception& e) {
    AllocatorTraits_::deallocate(allocator, left_, 2);
    left_ = nullptr;
    throw;
  }
  try {
    right_ = nodesPtr + 1;  // NOLINT
    std::construct_at(right_, std::move(*value_));
  } catch (const std::exception& e) {
    getLeft()->~Derived();
    AllocatorTraits_::deallocate(allocator, left_, 2);
    left_ = nullptr;
    right_ = nullptr;
    throw;
  }
  value_ = std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::~BaseNode() {
  assert(isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
inline void BaseNode<T, Derived, Allocator>::clearChildren(
    Allocator_& allocator) {
  if (!getLeft()->isLeaf()) {
    left_->clearChildren(allocator);
  }
  if (!getRight()->isLeaf()) {
    right_->clearChildren(allocator);
  }
  AllocatorTraits_::deallocate(allocator, left_, 2);
  left_ = nullptr;
  right_ = nullptr;
}

}  // namespace dst::impl

#endif  // NODE_BASE_HPP
