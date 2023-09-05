#pragma once

#include "bitboard.h"
#include "slow_attacks.h"
#include "types.h"

class Magics
{
public:
  Magics (void) = default;
  Magics (Magics &&) = default;
  Magics (const Magics &) = default;
  Magics &operator= (Magics &&) = default;
  Magics &operator= (const Magics &) = default;
  ~Magics (void) = default;

  Magics (const uint32_t);

  uint32_t get_and_update_xorshift32_prn (void);
  uint64_t get_and_update_special_64bit_prn (void);
  uint64_t operator() ();

  Bitboard rook_potential_blockers (const unsigned int) const;
  Bitboard bishop_potential_blockers (const unsigned int) const;
  Bitboard give_kth_blocker_configuration (const Bitboard,
                                           const unsigned int) const;
  uint64_t find_magic_number (const bool, const unsigned int) const;

private:
  uint32_t current_xorshift32_sequence_element{ 1804289383 };
  Slow_Attacks slow_attacks;

  static constexpr unsigned int rook_potential_blockers_table[64]
      = { 12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12 };

  static constexpr unsigned int bishop_potential_blockers_table[64]
      = { 6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
          5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
          7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6 };
};
