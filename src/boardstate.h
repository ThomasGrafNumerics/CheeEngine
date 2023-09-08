#pragma once

#include "bitboard.h"
#include "magics.h"
#include "slow_attacks.h"
#include "types.h"

class Boardstate
{
public:
  Boardstate (void);
  Boardstate (Boardstate &&) = default;
  Boardstate (const Boardstate &) = default;
  Boardstate &operator= (Boardstate &&) = default;
  Boardstate &operator= (const Boardstate &) = default;
  ~Boardstate (void);
  Bitboard bitboards[12];
  Bitboard occupancies[3];
  bool side_to_move;
  unsigned int castle;
  unsigned int enpassant_square;
  unsigned int fifty_moves;
  uint64_t position_key;
  void clearBoardstate (void);

private:
};
