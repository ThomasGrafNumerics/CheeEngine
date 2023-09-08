#include "attacks.h"
#include "bitboard.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  Attacks A;
  Bitboard occupancy;
  occupancy.set_bit (0);
  occupancy.set_bit (7);
  occupancy.set_bit (8);
  occupancy.set_bit (9);
  occupancy.set_bit (10);
  occupancy.set_bit (11);

  occupancy.print_bitboard ();

  // for (unsigned int square = 0; square < 64; ++square)
  //   {
  //     // A.pawn_attacks (0, square).print_bitboard ();
  //     // A.pawn_attacks (1, square).print_bitboard ();
  //     // A.knight_attacks (square).print_bitboard ();
  //     // A.king_attacks (square).print_bitboard ();
  //     // A.rook_attacks (occupancy, square).print_bitboard ();
  //     // A.bishop_attacks (occupancy, square).print_bitboard ();
  //     A.queen_attacks (occupancy, square).print_bitboard ();
  //   }
  return 0;
}
