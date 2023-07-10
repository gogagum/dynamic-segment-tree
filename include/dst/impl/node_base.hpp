////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

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
  using Allocator_ =
    std::allocator_traits<Allocator>::template rebind_alloc<Derived>;
  using AllocatorTraits_ = std::allocator_traits<Allocator_>;

 public:
  BaseNode() = default;
  explicit BaseNode(const T& value) : value_(value) {
  }
  explicit BaseNode(T&& value) : value_(std::move(value)) {
  }
  BaseNode(const BaseNode&) = default;
  BaseNode(BaseNode&&) noexcept = default;

 public:
  BaseNode& operator=(const BaseNode&) = default;
  BaseNode& operator=(BaseNode&&) noexcept = default;
  [[nodiscard]] const T& getValue() const {
    assert(value_.has_value());
    return value_.value();
  }
  
  /**
   * @brief Set value to node making a copy.
   * @param value value reference.
   */
  void setValue(const T&);
  
  /**
   * @brief Set the value to node moving it.
   * @param value moved value.
   */
  void setValue(T&&);
  
  [[nodiscard]] bool hasValue() const {
    return value_.has_value();
  }
  void setNullValue() {
    value_ = std::nullopt;
  }
  [[nodiscard]] bool isLeaf() const {
    return !left_ && !right_;
  };
  void initChildren();
  [[nodiscard]] Derived* getLeft() const {
    return left_;
  }
  [[nodiscard]] Derived* getRight() const {
    return right_;
  }
  ~BaseNode();

 private:
  void clearChildren_();

 private:
  Allocator_ allocator_;
  std::optional<T> value_;
  Derived* left_{nullptr};
  Derived* right_{nullptr};
};

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::setValue(const T& value) {
  value_ = value;
  if (!this->isLeaf()) {
    clearChildren_();
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::setValue(T&& value) {
  value_ = std::move(value);
  if (!this->isLeaf()) {
    clearChildren_();
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
void BaseNode<T, Derived, Allocator>::initChildren() {
  assert(isLeaf() && "Can only init children for a leaf.");
  auto nodesPtr = AllocatorTraits_::allocate(allocator_, 2);
  left_ = nodesPtr;
  right_ = nodesPtr + 1;  //NOLINT
  std::construct_at(left_);
  std::construct_at(right_);
  assert(value_.has_value() && "No value to set to children.");
  left_->setValue(value_.value());
  if constexpr (std::movable<T>) {
    right_->setValue(std::move(value_.value()));
  } else {
    right_->setValue(value_.value());
  }
  value_ = std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
BaseNode<T, Derived, Allocator>::~BaseNode() {
  if (!isLeaf()) {
    clearChildren_();
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T, class Derived, class Allocator>
inline void BaseNode<T, Derived, Allocator>::clearChildren_() {
  getLeft()->~Derived();
  getRight()->~Derived();
  AllocatorTraits_::deallocate(allocator_, left_, 2);
  left_ = nullptr;
  right_ = nullptr;
}

}  // namespace dst::impl

#endif  // NODE_BASE_HPP
