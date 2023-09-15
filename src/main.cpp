#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "magics.h"
#include "types.h"
#include <iostream>

int
main ()
{
  const std::string myfen{ "r7/8/8/1K6/8/8/8/8 w - - " };
  const std::string foolsmate{ "4R3/2Q5/6R1/1K1B1N1P/3B4/3N4/P6P/1k6 w KQkq - 0 3 " };

  Board b;
  b.parseFEN (foolsmate);
  b.print_board ();
  auto m = b.get_pseudo_moves ();

  for (const auto &element : m)
    {
      element.print_move ();
      std::cout << std::endl;
    }
  std::cout << m.size ();
  return 0;
}
