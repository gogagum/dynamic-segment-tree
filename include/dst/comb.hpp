////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>

namespace dst::comb {

template <class GetValueT>
struct Min {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return std::min(left, right);
  }
};

template <class GetValueT>
struct Max {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return std::max(left, right);
  }
};

template <class GetValueT>
struct Sum {
  GetValueT operator()(const GetValueT& left, const GetValueT& right) const {
    return left + right;
  }
};

template <class GetValueT, class KeyT>
struct Avg {
  GetValueT operator()(const GetValueT& left, const GetValueT& right,
                       KeyT leftBegin, KeyT separation, KeyT rightEnd) const {
    return (left * (separation - leftBegin) + right * (rightEnd - separation)) /
           (rightEnd - leftBegin);
  }
};

}  // namespace dst::comb