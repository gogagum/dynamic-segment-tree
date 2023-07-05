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

////////////////////////////////////////////////////////////////////////////////
/// \brief The DefaultUpdateArg<UpdateOp, ValueT> class.
///
/// \tparam UpdateOp update operation
/// \tparam ValueT value type in segment tree.
///
/// If UpdateOp is one argument update operation or NoUpdateOp then
/// DefaultUpdateArg::Type is void. In other case, if UpdateOp if one argument
/// update operation, then DefaultUpdateArg::Type is ValueT.
///
template <class UpdateOp, class ValueT>
struct DefaultUpdateArg {
  using Type = ValueT;
};

template <class UpdateOp, class ValueT>
  requires dst::conc::OneArgUpdateOp<UpdateOp, ValueT>
struct DefaultUpdateArg<UpdateOp, ValueT> {
  using Type = void;
};

template <class ValueT>
struct DefaultUpdateArg<NoUpdateOp, ValueT> {
  using Type = void;
};

////////////////////////////////////////////////////////////////////////////////
/// \brief Default update operation argument.
template <class UpdateOp, class ValueT>
using DefaultUpdateArgT = typename mp::DefaultUpdateArg<UpdateOp, ValueT>::Type;

}  // namespace dst::mp

#endif  // MPIMPL_HPP
