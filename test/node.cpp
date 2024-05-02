////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <array>
#include <dst/impl/node.hpp>

#include "tools/allocate_n_times_then_throw.hpp"
#include "tools/copy_n_times_then_throw.hpp"

// NOLINTBEGIN(cppcoreguidelines-*, cert-err58-cpp,
// readability-function-cognitive-complexity)

////////////////////////////////////////////////////////////////////////////////
TEST(Node, Construct) {
  auto node = dst::impl::Node<int, bool>{};
  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, SetValue) {
  auto node = dst::impl::Node<int, bool>{};
  std::construct_at(node.getValuePtr(), 34);

  EXPECT_EQ(node.getValue(), 34);
  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, UpdateBool) {
  using Node = dst::impl::Node<int, bool>;

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 23);

  constexpr auto op = [](int n) {
    return n + 1;
  };

  auto alloc = std::allocator<Node>();
  node.update(op, alloc);

  EXPECT_TRUE(node.isLeaf());
  EXPECT_EQ(node.getValue(), 23);
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, UpdateInt) {
  using Node = dst::impl::Node<int, std::optional<int>>;

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 2);

  constexpr auto op = [](int n, int toAdd) {
    return n + toAdd;
  };

  auto alloc = std::allocator<Node>();
  node.update(op, 3, alloc);

  EXPECT_TRUE(node.isLeaf());
  EXPECT_EQ(node.getValue(), 2);
}

template <class ValueT, class UpdateT>
class NodeValueAndUpdateParametrizedTests : public testing::Test {
 public:
  using Node = dst::impl::Node<ValueT, UpdateT>;
};

template <class UpdateT>
using IntNodeUpdateParametrizedTests =
    NodeValueAndUpdateParametrizedTests<int, UpdateT>;

using UpdateTypes = ::testing::Types<void, bool, std::optional<int>>;

TYPED_TEST_SUITE(IntNodeUpdateParametrizedTests, UpdateTypes);

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, ClearChildren) {
  using Node = TestFixture::Node;
  using Alloc = std::allocator<Node>;
  using AllocTraits = std::allocator_traits<Alloc>;

  auto node = Node{};

  auto alloc = Alloc{};
  auto* const children = AllocTraits::allocate(alloc, 2);
  AllocTraits::construct(alloc, children);
  AllocTraits::construct(alloc, children + 1);

  node.ptr = children;

  node.clearChildren(alloc);

  EXPECT_TRUE(node.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, CopyLeafNoUpdate) {
  using Node = TestFixture::Node;

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 3);
  auto dest = Node{};
  std::construct_at(dest.getValuePtr(), 4);

  auto alloc = std::allocator<Node>{};
  Node::copySubtree(std::as_const(node), &dest, alloc);

  EXPECT_TRUE(dest.isLeaf());
  EXPECT_EQ(dest.getValue(), 3);
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, CopyLeafToNewlyCreatedNoUpdate) {
  using Node = TestFixture::Node;

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 4);
  auto dest = Node{};

  auto alloc = std::allocator<Node>{};
  Node::copyToNewlyCreated(std::as_const(node), &dest, alloc);

  EXPECT_TRUE(dest.isLeaf());
  EXPECT_EQ(dest.getValue(), 4);
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, CopySmallTreeToSingleNodeNoUpdate) {
  using Node = TestFixture::Node;

  //       root
  //       /  \
  // leaf(3)  leaf(4)

  auto root = Node{};
  auto children = std::array<Node, 2>{};
  root.ptr = children.data();

  std::construct_at(root.getLeft()->getValuePtr(), 3);
  std::construct_at(root.getRight()->getValuePtr(), 4);

  // dest(5)
  auto dest = Node{};
  std::construct_at(dest.getValuePtr(), 5);

  auto alloc = std::allocator<Node>{};
  Node::copyToNewlyCreated(std::as_const(root), &dest, alloc);

  EXPECT_TRUE(!dest.isLeaf());
  EXPECT_EQ(dest.getLeft()->getValue(), 3);
  EXPECT_EQ(dest.getRight()->getValue(), 4);

  root.ptr = nullptr;  // Unlink children from source and then they
                       // will be destroyed by vector.

  dest.clearChildren(alloc);  // Need to clear children passing alloc

  // In the end, here we don't need to destroy values as they are integers.
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, CopySmallTreeToSmallTree) {
  using Node = TestFixture::Node;
  using Alloc = std::allocator<Node>;
  using AllocTraits = std::allocator_traits<Alloc>;

  //       root
  //       /  \
  // leaf(3)  leaf(4)

  auto root = Node{};
  auto children = std::vector<Node>(2);
  root.ptr = children.data();

  std::construct_at(root.getLeft()->getValuePtr(), 3);
  std::construct_at(root.getRight()->getValuePtr(), 4);

  auto alloc = Alloc{};

  //       dest
  //       /  \
  // leaf(5)  leaf(6)

  auto dest = Node{};

  auto* const destChildren = AllocTraits::allocate(alloc, 2);
  AllocTraits::construct(alloc, destChildren);
  AllocTraits::construct(alloc, destChildren + 1);

  std::construct_at(destChildren[0].getValuePtr(), 5);
  std::construct_at(destChildren[1].getValuePtr(), 6);

  dest.ptr = destChildren;

  Node::copySubtree(std::as_const(root), &dest, alloc);

  EXPECT_TRUE(!dest.isLeaf());
  EXPECT_EQ(dest.getLeft()->getValue(), 3);
  EXPECT_EQ(dest.getRight()->getValue(), 4);

  root.ptr = nullptr;  // Unlink children from source and then they
                       // will be destroyed by vector.

  // In the end, here we don't need to destroy values as they are integers.

  // Children nodes are expected to be reused
  EXPECT_EQ(dest.ptr, destChildren);

  dest.clearChildren(alloc);  // Need to clear children passing alloc
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, CopySingleNodeToSmallTree) {
  using Node = TestFixture::Node;
  using Alloc = std::allocator<Node>;
  using AllocTraits = std::allocator_traits<Alloc>;

  // root(3)
  auto root = Node{};
  std::construct_at(root.getValuePtr(), 3);

  auto alloc = Alloc{};

  //       dest
  //       /  \
  // leaf(5)  leaf(6)

  auto dest = Node{};

  auto* const destChildren = AllocTraits::allocate(alloc, 2);
  AllocTraits::construct(alloc, destChildren);
  AllocTraits::construct(alloc, destChildren + 1);

  std::construct_at(destChildren[0].getValuePtr(), 5);
  std::construct_at(destChildren[1].getValuePtr(), 6);

  dest.ptr = destChildren;

  Node::copySubtree(std::as_const(root), &dest, alloc);

  EXPECT_TRUE(dest.isLeaf());
  EXPECT_EQ(dest.getValue(), 3);

  root.ptr = nullptr;  // Unlink children from source and then they
                       // will be destroyed by vector.

  // In the end, here we don't need to destroy values as they are integers.

  // Children nodes are expected to be reused
  EXPECT_EQ(dest.ptr, nullptr);

  // No need to clear children of dest as it is leaf and its children were
  // cleared.
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(IntNodeUpdateParametrizedTests, InitChildren) {
  using Node = TestFixture::Node;

  auto node = Node{};
  auto alloc = std::allocator<Node>();

  std::construct_at(node.getValuePtr(), 2);

  node.initChildrenSiftingValue(alloc);

  EXPECT_FALSE(node.isLeaf());
  EXPECT_EQ(node.getLeft()->getValue(), 2);
  EXPECT_EQ(node.getRight()->getValue(), 2);

  node.clearChildren(alloc);
}

template <class UpdateT>
using CopyNTimesThenThrowNodeUpdateParametrizedTests =
    NodeValueAndUpdateParametrizedTests<CopyNTimesThenThrow, UpdateT>;

TYPED_TEST_SUITE(CopyNTimesThenThrowNodeUpdateParametrizedTests, UpdateTypes);

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests,
           ThrowOnFirstCopyInChildrenInit) {
  using Node = TestFixture::Node;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 0);

  EXPECT_THROW(node.initChildrenSiftingValue(nodeAlloc),
               CopyNTimesThenThrow::Exception);
  EXPECT_TRUE(node.isLeaf());

  std::destroy_at(node.getValuePtr());
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests,
           ThrowOnSecondCopyInChildrenInit) {
  using Node = TestFixture::Node;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 1);

  EXPECT_THROW(node.initChildrenSiftingValue(nodeAlloc),
               CopyNTimesThenThrow::Exception);
  EXPECT_TRUE(node.isLeaf());

  std::destroy_at(node.getValuePtr());
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests,
           ThrowOnFirstInCopyToNewlyCreated) {
  using Node = TestFixture::Node;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  auto children = std::array<Node, 2>{};
  node.ptr = children.data();

  auto val = CopyNTimesThenThrow{2};

  std::construct_at(children[0].getValuePtr(), val);  // First
  std::construct_at(children[1].getValuePtr(), val);  // Second

  auto dest = Node{};

  // No copies left expect throw on first copy.
  EXPECT_THROW(Node::copyToNewlyCreated(node, &dest, nodeAlloc),
               CopyNTimesThenThrow::Exception);

  node.ptr = nullptr;

  std::destroy_at(children[0].getValuePtr());
  std::destroy_at(children[1].getValuePtr());

  EXPECT_TRUE(dest.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests,
           ThrowOnSecondInCopyToNewlyCreated) {
  using Node = TestFixture::Node;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  auto children = std::array<Node, 2>{};
  node.ptr = children.data();

  auto val = CopyNTimesThenThrow{3};

  std::construct_at(children[0].getValuePtr(), val);  // First
  std::construct_at(children[1].getValuePtr(), val);  // Second

  auto dest = Node{};

  // Only one of two copies will be finished successfully
  EXPECT_THROW(Node::copyToNewlyCreated(node, &dest, nodeAlloc),
               CopyNTimesThenThrow::Exception);

  node.ptr = nullptr;

  std::destroy_at(children[0].getValuePtr());
  std::destroy_at(children[1].getValuePtr());

  EXPECT_TRUE(dest.isLeaf());
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests, ThrowOnFirstInCopy) {
  using Node = TestFixture::Node;
  using Alloc = std::allocator<Node>;
  using AllocTraits = std::allocator_traits<Alloc>;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  auto children = std::array<Node, 2>{};
  node.ptr = children.data();

  auto val = CopyNTimesThenThrow{2};

  std::construct_at(children[0].getValuePtr(), val);  // First
  std::construct_at(children[1].getValuePtr(), val);  // Second

  auto dest = Node{};
  dest.ptr = AllocTraits::allocate(nodeAlloc, 2);

  auto destVal = CopyNTimesThenThrow{std::numeric_limits<size_t>::max()};

  AllocTraits::construct(nodeAlloc, dest.getLeft());
  AllocTraits::construct(nodeAlloc, dest.getRight());

  std::construct_at(dest.getLeft()->getValuePtr(), destVal);
  std::construct_at(dest.getRight()->getValuePtr(), destVal);

  // Only one of two copies will be finished successfully
  EXPECT_THROW(Node::copySubtree(node, &dest, nodeAlloc),
               CopyNTimesThenThrow::Exception);

  node.ptr = nullptr;

  std::destroy_at(children[0].getValuePtr());
  std::destroy_at(children[1].getValuePtr());

  AllocTraits::destroy(nodeAlloc, dest.getLeft());
  AllocTraits::destroy(nodeAlloc, dest.getRight());

  AllocTraits::deallocate(nodeAlloc, dest.ptr, 2);
  dest.ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(CopyNTimesThenThrowNodeUpdateParametrizedTests, ThrowOnSecondInCopy) {
  using Node = TestFixture::Node;
  using Alloc = std::allocator<Node>;
  using AllocTraits = std::allocator_traits<Alloc>;

  auto nodeAlloc = std::allocator<Node>();

  auto node = Node{};
  auto children = std::array<Node, 2>{};
  node.ptr = children.data();

  auto val = CopyNTimesThenThrow{3};

  std::construct_at(children[0].getValuePtr(), val);  // First
  std::construct_at(children[1].getValuePtr(), val);  // Second

  auto dest = Node{};
  dest.ptr = AllocTraits::allocate(nodeAlloc, 2);

  auto destVal = CopyNTimesThenThrow{std::numeric_limits<size_t>::max()};

  AllocTraits::construct(nodeAlloc, dest.getLeft());
  AllocTraits::construct(nodeAlloc, dest.getRight());

  std::construct_at(dest.getLeft()->getValuePtr(), destVal);
  std::construct_at(dest.getRight()->getValuePtr(), destVal);

  // Only one of two copies will be finished successfully
  EXPECT_THROW(Node::copySubtree(node, &dest, nodeAlloc),
               CopyNTimesThenThrow::Exception);

  node.ptr = nullptr;

  std::destroy_at(children[0].getValuePtr());
  std::destroy_at(children[1].getValuePtr());

  AllocTraits::destroy(nodeAlloc, dest.getLeft());
  AllocTraits::destroy(nodeAlloc, dest.getRight());

  AllocTraits::deallocate(nodeAlloc, dest.ptr, 2);
  dest.ptr = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
TEST(Node, ThrowOnAllocation) {
  using Node = dst::impl::Node<int, bool, AllocateNTimesThenThrow<int>>;

  auto nodeAlloc = AllocateNTimesThenThrow<Node>(0);

  auto node = Node{};
  std::construct_at(node.getValuePtr(), 1);

  EXPECT_THROW(node.initChildrenSiftingValue(nodeAlloc),
               AllocateNTimesThenThrow<Node>::Exception);
  EXPECT_TRUE(node.isLeaf());
}

// NOLINTEND(cppcoreguidelines-*, cert-err58-cpp,
// readability-function-cognitive-complexity)
