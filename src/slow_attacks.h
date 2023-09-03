#pragma once

#include "bitboard.h"
#include "types.h"

class Slow_Attacks
{
public:
  Slow_Attacks (void) = default;
  Slow_Attacks (Slow_Attacks &&) = default;
  Slow_Attacks (const Slow_Attacks &) = default;
  Slow_Attacks &operator= (Slow_Attacks &&) = default;
  Slow_Attacks &operator= (const Slow_Attacks &) = default;
  ~Slow_Attacks (void) = default;

  Bitboard slow_pawn_attacks (const bool, const unsigned int) const;
  Bitboard slow_knight_attacks (const unsigned int) const;
  Bitboard slow_king_attacks (const unsigned int) const;
  Bitboard slow_rook_attacks (const Bitboard, const unsigned int) const;
  Bitboard slow_bishop_attacks (const Bitboard, const unsigned int) const;

private:
};
