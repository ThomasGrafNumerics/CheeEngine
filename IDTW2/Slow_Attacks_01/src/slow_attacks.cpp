#include "slow_attacks.h"

Bitboard
Slow_Attacks::slow_pawn_attacks (const bool side, const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  // together!

  return attacks;
}

Bitboard
Slow_Attacks::slow_knight_attacks (const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  // ?????
  // ?????
  // ?????
  // ?????

  return attacks;
}

Bitboard
Slow_Attacks::slow_king_attacks (const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  // ?????
  // ?????
  // ?????
  // ?????

  return attacks;
}

Bitboard
Slow_Attacks::slow_rook_attacks (const Bitboard blockers, const unsigned int square) const
{
  Bitboard attacks{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  // ?????
  // ?????
  // ?????
  // ?????

  return attacks;
}

Bitboard
Slow_Attacks::slow_bishop_attacks (const Bitboard blockers, const unsigned int square) const
{
  Bitboard attacks{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  // together!

  return attacks;
}
