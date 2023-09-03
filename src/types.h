#pragma once

#include <bitset>
#include <cassert>
#include <iostream>
#include <string>

#include <cstdint>

// user defined literal for std::uint64_t type
constexpr std::uint64_t operator"" _uint64 (unsigned long long arg)
{
  return static_cast<std::uint64_t> (arg);
}

// some constants
constexpr uint64_t not_a_file_mask{ 18374403900871474942_uint64 };
constexpr uint64_t not_h_file_mask{ 9187201950435737471_uint64 };
constexpr uint64_t not_ab_file_mask{ 18229723555195321596_uint64 };
constexpr uint64_t not_gh_file_mask{ 4557430888798830399_uint64 };
