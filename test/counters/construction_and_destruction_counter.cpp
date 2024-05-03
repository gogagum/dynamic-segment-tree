////////////////////////////////////////////////////////////////////////////////
// Copyright Georgy Guminov 2023-2024.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "construction_and_destruction_counter.hpp"

////////////////////////////////////////////////////////////////////////////////
ConstructionAndDestructionCounter::ConstructionAndDestructionCounter(Init init)
    : init_(std::move(init)) {
  ++init_->constructionCnt;
}

////////////////////////////////////////////////////////////////////////////////
ConstructionAndDestructionCounter::ConstructionAndDestructionCounter(
    const ConstructionAndDestructionCounter& other)
    : init_{other.init_} {
  ++init_->constructionCnt;
}

////////////////////////////////////////////////////////////////////////////////
ConstructionAndDestructionCounter::ConstructionAndDestructionCounter(
    ConstructionAndDestructionCounter&& other) noexcept
    : init_{other.init_} {  // NOLINT
  ++init_->constructionCnt;
}

////////////////////////////////////////////////////////////////////////////////
ConstructionAndDestructionCounter::~ConstructionAndDestructionCounter() {
  ++init_->destructionCnt;
}
