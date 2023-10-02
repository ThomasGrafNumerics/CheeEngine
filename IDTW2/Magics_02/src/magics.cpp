#include "magics.h"
#include "bitboard.h"
#include "slow_attacks.h"

Magics::Magics (const uint32_t xorshift32_seed) : xorshift32_sequence_element{ xorshift32_seed } {}

uint32_t
Magics::get_and_update_xorshift32_prn (void)
{
  xorshift32_sequence_element ^= xorshift32_sequence_element << 13;
  xorshift32_sequence_element ^= xorshift32_sequence_element >> 17;
  xorshift32_sequence_element ^= xorshift32_sequence_element << 5;

  return xorshift32_sequence_element;
}

uint64_t
Magics::get_and_update_special_64bit_prn (void)
{
  uint64_t n0, n1, n2, n3;
  n0 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n1 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n2 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n3 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;

  return n0 | (n1 << 16) | (n2 << 32) | (n3 << 48);
}

uint64_t
Magics::next_magic_candidate (void)
{
  return get_and_update_special_64bit_prn () & get_and_update_special_64bit_prn () & get_and_update_special_64bit_prn ();
}

Bitboard
Magics::rook_potential_blockers (const unsigned int square) const
{
  Bitboard potential_blockers{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  // ?????
  // ?????
  // ?????
  // ?????
  // ?????

  return potential_blockers;
}

Bitboard
Magics::bishop_potential_blockers (const unsigned int square) const
{
  Bitboard potential_blockers{ 0 };
  // together!

  return potential_blockers;
}

Bitboard
Magics::give_kth_blocker_configuration (const Bitboard potential_blockers, const unsigned int k) const
{
  Bitboard kth_blocker_configuration{ 0 };
  // together!
  return kth_blocker_configuration;
}

uint64_t
Magics::find_magic_number (const bool is_rook, const unsigned int square)
{
  // together!
  return 11111;
}

void
Magics::init_rook_magic_numbers (void)
{
  for (unsigned int square = 0; square < 64; ++square)
    {
      rook_magic_numbers_table[square] = find_magic_number (true, square);
    }
}

void
Magics::init_bishop_magic_numbers (void)
{
  for (unsigned int square = 0; square < 64; ++square)
    {
      bishop_magic_numbers_table[square] = find_magic_number (false, square);
    }
}

void
Magics::print_magic_numbers (void) const
{
  std::cout << "rook magic numbers:\n";
  for (unsigned int square = 0; square < 64; ++square)
    {
      std::cout << "0x" << std::hex << rook_magic_numbers_table[square] << "_uint64,\n";
    }

  std::cout << std::endl << "bishop magic numbers:\n";

  for (unsigned int square = 0; square < 64; ++square)
    {
      std::cout << "0x" << std::hex << bishop_magic_numbers_table[square] << "_uint64,\n";
    }
}

void
Magics::precompute_slider_pieces_attacks (void)
{
  // together!
}
