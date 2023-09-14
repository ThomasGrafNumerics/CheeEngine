#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  // const std::string myfen{ "1R6/P7/8/8/8/8/8/8 w - - " };
  Board b;
  b.parseFEN (tricky_position);
  b.print_board ();
  auto m = b.get_pseudo_moves ();

  for (const auto &element : m)
    {
      element.print_move ();
      std::cout << std::endl;
    }

  return 0;
}
