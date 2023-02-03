#ifndef MPIMPL_HPP
#define MPIMPL_HPP

#include <disable_operations.hpp>
#include <concepts.hpp>

namespace dst::impl {


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

template <class UpdateOp, class ValueT>
using DefaultUpdateArgT = typename DefaultUpdateArg<UpdateOp, ValueT>::Type;

}

#endif // MPIMPL_HPP
