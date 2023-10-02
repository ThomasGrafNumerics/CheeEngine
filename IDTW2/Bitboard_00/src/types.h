#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>

// user defined literal for std::uint64_t type
constexpr std::uint64_t
operator"" _uint64 (unsigned long long arg)
{
  return static_cast<std::uint64_t> (arg);
}
