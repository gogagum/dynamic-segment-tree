#ifndef MPIMPL_HPP
#define MPIMPL_HPP

#include <concepts.hpp>

namespace dst::impl {

template <class ValueT, class UpdateOp>
struct DefaultUpdateArg {
    using Type = ValueT;
};

template <class ValueT, dst::conc::OneArgUpdateOp<ValueT> UpdateOp>
struct DefaultUpdateArg<ValueT, UpdateOp> {
    using Type = void;
};

template <class ValueT>
struct DefaultUpdateArg<ValueT, void> {
    using Type = void;
};

template <class ValueT, class UpdateOp>
using DefaultUpdateArgT = typename DefaultUpdateArg<ValueT, UpdateOp>::Type;

}

#endif // MPIMPL_HPP
