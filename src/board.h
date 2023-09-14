#pragma once

#include "attacks.h"
#include "bitboard.h"
#include "boardstate.h"
#include "magics.h"
#include "move.h"
#include "slow_attacks.h"
#include "types.h"
#include <vector>

class Board
{
public:
  Board (void) = default;
  Board (Board &&) = default;
  Board (const Board &) = default;
  Board &operator= (Board &&) = default;
  Board &operator= (const Board &) = default;
  ~Board (void) = default;

  Attacks attacks;
  Boardstate boardstate;
  unsigned int visited_nodes;

  void print_board (void) const;
  void parseFEN (const std::string &);
  bool is_square_attacked (const bool, const unsigned int) const;
  bool is_king_attacked (void) const;
  std::vector<Move> get_pseudo_moves (void) const;

private:
};
