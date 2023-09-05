#include "attacks.h"
#include "bitboard.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  Attacks A;

  for (unsigned int square = 0; square < 64; ++square)
    {
      A.pawn_attacks (1, square).print_bitboard ();
    }
  return 0;
}
