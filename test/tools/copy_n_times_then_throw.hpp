////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef COPY_N_TIMES_THEN_THROW
#define COPY_N_TIMES_THEN_THROW

#include <memory>
#include <stdexcept>

class CopyNTimesThenThrow {
 public:
  class Exception : public std::runtime_error {
   public:
    Exception() : std::runtime_error("Done all copies.") {
    }
  };

 public:
  explicit CopyNTimesThenThrow(std::size_t n)
      : counter_{std::make_unique<std::size_t>(n)} {
  }
  CopyNTimesThenThrow(const CopyNTimesThenThrow& other);
  CopyNTimesThenThrow(CopyNTimesThenThrow&& other) noexcept = delete;

  CopyNTimesThenThrow& operator=(const CopyNTimesThenThrow&);
  CopyNTimesThenThrow& operator=(CopyNTimesThenThrow&&) noexcept = delete;

  ~CopyNTimesThenThrow() = default;
  
 private:
  std::shared_ptr<std::size_t> counter_;
};

#endif  // COPY_N_TIMES_THEN_THROW
