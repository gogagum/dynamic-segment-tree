////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <dst/impl/node.hpp>

#include "tools/copy_n_times_then_throw.hpp"

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-cpp,
// readability-function-cognitive-complexity)

////////////////////////////////////////////////////////////////////////////////
TEST(Node, Construct) {
  auto node = dst::impl::Node<int, bool>{2};

  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, UpdateBool) {
  auto node = dst::impl::Node<int, bool>{2};

  using UpdateOp = decltype([](int n) {
    return n + 1;
  });
  auto alloc = std::allocator<dst::impl::Node<int, bool>>();
  node.update(UpdateOp{}, alloc);

  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, UpdateInt) {
  auto node = dst::impl::Node<int, std::optional<int>>{2};

  using UpdateOp = decltype([](int n, int toAdd) {
    return n + toAdd;
  });
  auto alloc = std::allocator<dst::impl::Node<int, std::optional<int>>>();
  node.update(UpdateOp{}, 3, alloc);

  EXPECT_TRUE(node.isLeaf());
  EXPECT_EQ(node.getValue(), 5);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, InitChildren) {
  auto node = dst::impl::Node<int, bool>{2};

  auto alloc = std::allocator<dst::impl::Node<int, bool>>();
  node.initChildren(alloc);

  EXPECT_FALSE(node.isLeaf());
  //EXPECT_EQ(node.getLeft()->getValue(), 2);
  //EXPECT_EQ(node.getRight()->getValue(), 2);

  node.clearChildren(alloc);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, ThrowOnFirstCopy) {
  auto alloc = std::allocator<CopyNTimesThenThrow>();
  auto nodeAlloc = std::allocator<dst::impl::Node<CopyNTimesThenThrow, bool>>();

  auto node =
      dst::impl::Node<CopyNTimesThenThrow, bool>{CopyNTimesThenThrow{1}};

  EXPECT_THROW(node.initChildren(nodeAlloc), CopyNTimesThenThrow::Exception);
  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, ThrowOnSecondCopy) {
  auto alloc = std::allocator<CopyNTimesThenThrow>();
  auto nodeAlloc = std::allocator<dst::impl::Node<CopyNTimesThenThrow, bool>>();

  auto node =
      dst::impl::Node<CopyNTimesThenThrow, bool>{CopyNTimesThenThrow{2}};

  EXPECT_THROW(node.initChildren(nodeAlloc), CopyNTimesThenThrow::Exception);
  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, ThrowOnAllocation) {
  auto alloc = std::allocator<CopyNTimesThenThrow>();
  auto nodeAlloc = std::allocator<dst::impl::Node<CopyNTimesThenThrow, bool>>();

  auto node =
      dst::impl::Node<CopyNTimesThenThrow, bool>{CopyNTimesThenThrow{1}};

  EXPECT_THROW(node.initChildren(nodeAlloc), CopyNTimesThenThrow::Exception);
  EXPECT_TRUE(node.isLeaf());
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-cpp,
// readability-function-cognitive-complexity)
