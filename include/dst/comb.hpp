////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>

namespace dst::comb {

////////////////////////////////////////////////////////////////////////////////
/// \brief The Min<GetValueT> class. Default min combiner.
///
/// \tparam GetValueT rangeGet operation return type.
///
template <class GetValueT>
struct Min {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return std::min(left, right);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// \brief The Max<GetValueT> class. Default max combiner.
///
/// \tparam GetValueT rangeGet operation return type.
///
template <class GetValueT>
struct Max {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return std::max(left, right);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// \brief The Sum<GetValueT> class. Default sum combiner.
///
/// \tparam GetValueT rangeGet operation return type.
///
template <class GetValueT>
struct Sum {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return left + right;
  }
};

////////////////////////////////////////////////////////////////////////////////
/// \brief The Avg<GetValueT, KeyT> class. Default average counting combiner.
///
/// \tparam GetValueT rangeGet operation return type.
/// \tparam KeyT key type of a segment tree.
///
template <class GetValueT, class KeyT>
struct Avg {
  GetValueT operator()(const GetValueT& left, const GetValueT& right,
                       KeyT leftBegin, KeyT separation, KeyT rightEnd) const {
    return (left * (separation - leftBegin) + right * (rightEnd - separation)) /
           (rightEnd - leftBegin);
  }
};

}  // namespace dst::comb