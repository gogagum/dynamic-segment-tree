////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "info_stream.hpp"
#include <iostream>

std::ostream& infoStream() {
  return std::cerr << "[          ] INFO: ";
}
