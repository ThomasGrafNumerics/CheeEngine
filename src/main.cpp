#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "magics.h"
#include "types.h"
#include <iostream>
#include <string>

int
main ()
{
  Board b;
  const std::string postion{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " };
  b.parseFEN (postion);

  for (unsigned int depth = 1; depth <= 7; ++depth)
    {
      b.visited_nodes = 0;
      b.perf_test (depth);
      std::cout << b.visited_nodes << std::endl;
    }

  return 0;
}
