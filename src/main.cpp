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
  // Bitboard b{ not_a_file_mask };
  //
  // b.print_bitboard ();
  Board b;
  // const std::string postion{ "rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1" };
  b.parseFEN (start_position);

  for (unsigned int depth = 1; depth <= 5; ++depth)
    {
      b.visited_nodes = 0;

      b.perf_test (depth);

      std::cout << b.visited_nodes << std::endl;
    }

  return 0;
}
