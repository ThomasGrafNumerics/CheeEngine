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
  b.parseFEN (start_position);
  b.print_board ();
  auto m = b.get_pseudo_moves ();

  for (auto &move : m)
    {
      if (b.make_pseudo_move (move))
        {
          b.print_board ();
          b.restore_state ();
        }
    }
  return 0;
}
