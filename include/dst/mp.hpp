////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MPIMPL_HPP
#define MPIMPL_HPP

#include <dst/concepts.hpp>
#include <dst/disable_operations.hpp>

namespace dst::mp {
template <class UpdateOp, class ValueT>
struct DefaultUpdateArg {
  using Type = ValueT;
};

template <class ValueT, dst::conc::OneArgUpdateOp<ValueT> UpdateOp>
struct DefaultUpdateArg<UpdateOp, ValueT> {
  using Type = void;
};

template <class ValueT>
struct DefaultUpdateArg<NoUpdateOp, ValueT> {
  using Type = void;
};

////////////////////////////////////////////////////////////////////////////////
// Default update operation argument.
template <class UpdateOp, class ValueT>
using DefaultUpdateArgT = typename mp::DefaultUpdateArg<UpdateOp, ValueT>::Type;

}  // namespace dst::mp

#endif  // MPIMPL_HPP
