#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  Bitboard b{ rank_2_mask };
  b.print_bitboard ();
  // const std::string custom{ "8/8/3B4/4r3/8/8/8/8 b - - " };
  // Board b;
  //
  // Bitboard test;
  // b.parseFEN (custom);
  //
  // for (unsigned int square = 0; square < 64; ++square)
  //   {
  //     if (b.is_square_attacked (0, square))
  //       {
  //         test.set_bit (square);
  //       }
  //   }
  // b.print_board ();
  // test.print_bitboard ();
  return 0;
}
