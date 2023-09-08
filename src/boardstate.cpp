#include "boardstate.h"
#include "types.h"

Boardstate::Boardstate ()
    : bitboards{}, occupancies{}, side_to_move{ Chess::white }, castle{ 0 },
      enpassant_square{ Chess::forbidden_square }, fifty_moves{ 0 }, position_key{ 0 }
{
}

void
Boardstate::clear_boardstate (void)
{
  std::fill (std::begin (bitboards), std::end (bitboards), 0);
  std::fill (std::begin (occupancies), std::end (occupancies), 0);
  side_to_move = Chess::white;
  castle = 0;
  enpassant_square = Chess::forbidden_square;
  fifty_moves = 0;
  position_key = 0;
}
