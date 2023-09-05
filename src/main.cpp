#include "bitboard.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  Bitboard a;
  a.print_bitboard ();
  Magics m{ 0 };
  Bitboard b = m.bishop_potential_blockers (0);
  b.print_bitboard ();
  Bitboard c = m.give_kth_blocker_configuration (b, 11);
  c.print_bitboard ();

  return 0;
}
