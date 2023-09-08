#pragma once

#include "bitboard.h"
#include "magics.h"
#include "slow_attacks.h"
#include "types.h"

class Attacks
{
public:
  Attacks (void);
  Attacks (Attacks &&) = default;
  Attacks (const Attacks &) = default;
  Attacks &operator= (Attacks &&) = default;
  Attacks &operator= (const Attacks &) = default;
  ~Attacks () = default;

  void precompute_leaper_pieces_attacks (void);
  Bitboard pawn_attacks (const bool, const unsigned int) const;
  Bitboard knight_attacks (const unsigned int) const;
  Bitboard king_attacks (const unsigned int) const;
  Bitboard rook_attacks (const Bitboard, const unsigned int) const;
  Bitboard bishop_attacks (const Bitboard, const unsigned int) const;
  Bitboard queen_attacks (const Bitboard, const unsigned int) const;

  Slow_Attacks slow_attacks;
  Magics magics;
  Bitboard precomputed_pawn_attacks_table[2][64];
  Bitboard precomputed_knight_attacks_table[64];
  Bitboard precomputed_king_attacks_table[64];

private:
};
