////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef INFO_STREAM_HPP
#define INFO_STREAM_HPP

#include <format>
#include <iostream>
#include <iterator>

std::ostream& infoStream();

template<class... Args>
void infoStreamPrint(std::format_string<Args...> fmtStr, Args&&... args) {
  std::format_to(std::ostream_iterator<char>(infoStream()), fmtStr,
                 std::forward<Args>(args)...);
}

#endif
