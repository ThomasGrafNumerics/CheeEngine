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
  // together!
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

// Bitboard
// Attacks::rook_attacks (const Bitboard occupancy, const unsigned int square) const
// {
//
// }

// Bitboard
// Attacks::bishop_attacks (const Bitboard occupancy, const unsigned int square) const
// {
//
// }

// Bitboard
// Attacks::queen_attacks (const Bitboard occupancy, const unsigned int square) const
// {
// }
