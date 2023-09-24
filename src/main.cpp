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
  const std::string postion{ "rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1" };
  b.parseFEN (postion);

  // for (unsigned int depth = 1; depth <= 7; ++depth)
  //   {
  b.visited_nodes = 0;

  b.perf_test (6);

  std::cout << b.visited_nodes << std::endl;
  // }

  return 0;
}
