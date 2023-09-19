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
  ~Boardstate (void) = default;
  Bitboard bitboards[12];
  Bitboard occupancies[3];
  bool side_to_move;
  unsigned int castle;
  unsigned int enpassant_square;
  unsigned int fifty_moves;
  uint64_t position_key;
  void clear_boardstate (void);
  void hash_piece (const unsigned int piece, const unsigned int square);
  void hash_enpassant (const unsigned int square);
  void hash_castle (const unsigned int castle);
  void hash_side_to_move (void);
  void generate_new_key (void);
  static void init_zobrist_keys (void);

private:
  static uint64_t piece_keys[12][64];
  static uint64_t enpassant_keys[64];
  static uint64_t castle_keys[16];
  static uint64_t side_to_move_key;
};
