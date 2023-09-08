#pragma once

#include "attacks.h"
#include "bitboard.h"
#include "boardstate.h"
#include "magics.h"
#include "slow_attacks.h"
#include "types.h"

class Board
{
public:
  Board (void);
  Board (Board &&) = default;
  Board (const Board &) = default;
  Board &operator= (Board &&) = default;
  Board &operator= (const Board &) = default;
  ~Board (void);

  Attacks attacks;
  Boardstate boardstate;

  void print_board (void) const;
  void parseFEN (const std::string &);
  bool is_square_attacked (const unsigned int, const unsigned int) const;
  bool is_king_attacked (void) const;

private:
};
