#include "attacks.h"
#include "bitboard.h"
#include "types.h"

Attacks::Attacks (void)
{
  this->precompute_leaper_pieces_attacks ();
  this->magics.precompute_slider_pieces_attacks ();
}

void
Attacks::precompute_leaper_pieces_attacks (void)
{

  for (unsigned int square = 0; square < 64; ++square)
    {
      precomputed_pawn_attacks_table[0][square] = slow_attacks.slow_pawn_attacks (Chess::white, square);
      precomputed_pawn_attacks_table[1][square] = slow_attacks.slow_pawn_attacks (Chess::black, square);

      precomputed_knight_attacks_table[square] = slow_attacks.slow_knight_attacks (square);
      precomputed_king_attacks_table[square] = slow_attacks.slow_king_attacks (square);
    }
}

Bitboard
Attacks::pawn_attacks (const bool side, const unsigned int square) const
{
  return precomputed_pawn_attacks_table[side][square];
}

Bitboard
Attacks::knight_attacks (const unsigned int square) const
{
  return precomputed_knight_attacks_table[square];
}

Bitboard
Attacks::king_attacks (const unsigned int square) const
{
  return precomputed_king_attacks_table[square];
}

Bitboard
Attacks::rook_attacks (const Bitboard occupancy, const unsigned int square) const
{
  unsigned int magic_index = static_cast<unsigned int> (
      ((occupancy & magics.rook_potential_blockers_table[square]) * magics.precomputed_rook_magics[square])
      >> (64 - magics.rook_blocker_count_table[square]));

  return magics.precomputed_rook_attacks_hash_table[square][magic_index];
}

Bitboard
Attacks::bishop_attacks (const Bitboard occupancy, const unsigned int square) const
{
  unsigned int magic_index
      = static_cast<unsigned int> (((occupancy & magics.bishop_potential_blockers_table[square])
                                    * magics.precomputed_bishop_magics[square])
                                   >> (64 - magics.bishop_blocker_count_table[square]));

  return magics.precomputed_bishop_attacks_hash_table[square][magic_index];
}

Bitboard
Attacks::queen_attacks (const Bitboard occupancy, const unsigned int square) const
{
  return rook_attacks (occupancy, square) | bishop_attacks (occupancy, square);
}
