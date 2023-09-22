#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{

  Board b;

  for (unsigned int square = 0; square < 64; ++square)
    {
      b.attacks.knight_attacks (square).print_bitboard ();
    }

  return 0;
}
