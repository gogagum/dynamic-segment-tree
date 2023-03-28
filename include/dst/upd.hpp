////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace dst::upd {

////////////////////////////////////////////////////////////////////////////////
/// \brief The Negate<ValueT> class. Default negate update operation.
template <class ValueT>
struct Negate {
  ValueT operator()(const ValueT& val) const {
    return -val;
  };
};

}  // namespace dst::upd
