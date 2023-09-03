#include "bitboard.h"
#include "types.h"

Bitboard pawn_attacks (const bool, const unsigned int);
Bitboard knight_attacks (const unsigned int);
Bitboard king_attacks (const unsigned int);
Bitboard rook_attacks (const Bitboard, const unsigned int);
Bitboard bishop_attacks (const Bitboard, const unsigned int);
Bitboard queen_attacks (const Bitboard, const unsigned int);
