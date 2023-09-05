#include "slow_attacks.h"

Bitboard
Slow_Attacks::slow_pawn_attacks (const bool side, const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  if (side == 0)
    {
      attacks |= ((temp >> 7) & not_a_file_mask);
      attacks |= ((temp >> 9) & not_h_file_mask);
    }
  else
    {
      attacks |= ((temp << 7) & not_h_file_mask);
      attacks |= ((temp << 9) & not_a_file_mask);
    }

  return attacks;
}

Bitboard
Slow_Attacks::slow_knight_attacks (const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  attacks |= ((temp >> 17) & not_h_file_mask);
  attacks |= ((temp >> 15) & not_a_file_mask);
  attacks |= ((temp >> 10) & not_gh_file_mask);
  attacks |= ((temp >> 6) & not_ab_file_mask);

  attacks |= ((temp << 17) & not_a_file_mask);
  attacks |= ((temp << 15) & not_h_file_mask);
  attacks |= ((temp << 10) & not_ab_file_mask);
  attacks |= ((temp << 6) & not_gh_file_mask);

  return attacks;
}

Bitboard
Slow_Attacks::slow_king_attacks (const unsigned int square) const
{
  Bitboard attacks{ 0 }, temp{ 0 };
  temp.set_bit (square);

  attacks |= ((temp >> 9) & not_h_file_mask);
  attacks |= (temp >> 8);
  attacks |= ((temp >> 7) & not_a_file_mask);
  attacks |= ((temp >> 1) & not_h_file_mask);

  attacks |= ((temp << 9) & not_a_file_mask);
  attacks |= (temp << 8);
  attacks |= ((temp << 7) & not_h_file_mask);
  attacks |= ((temp << 1) & not_a_file_mask);

  return attacks;
}

Bitboard
Slow_Attacks::slow_rook_attacks (const Bitboard blockers, const unsigned int square) const
{
  Bitboard attacks{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  for (int r = rank + 1, f = file; r <= 7; ++r)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank - 1, f = file; r >= 0; --r)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank, f = file + 1; f <= 7; ++f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank, f = file - 1; f >= 0; --f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  return attacks;
}

Bitboard
Slow_Attacks::slow_bishop_attacks (const Bitboard blockers, const unsigned int square) const
{
  Bitboard attacks{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; ++r, ++f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; ++r, --f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; --r, ++f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f)
    {
      Bitboard bitboard = 1_uint64 << (r * 8 + f);
      attacks |= bitboard;
      if (bitboard & blockers)
        break;
    }

  return attacks;
}
