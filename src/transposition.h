#pragma once
#include "bitboard.h"
#include "move.h"
#include <cstdint>

class Transposition
{
public:
  uint64_t position_key;
  int depth_searched_beyond;
  int score;
  int hash_flag;

  void Clear ();

  Transposition ();
  Transposition (uint64_t new_position_key, int new_depth_searched_beyond, int new_score, int new_flag);

  static constexpr int HASH_FLAG_EMPTY = 0;
  static constexpr int HASH_FLAG_EXACT = 1;
  static constexpr int HASH_FLAG_ALPHA = 2;
  static constexpr int HASH_FLAG_BETA = 3;
  static constexpr int SCORE_EMPTY = 1234567;

  // just ignore the collision lol
  static int ProbeHashScore (const Transposition *tt, size_t tt_size, uint64_t position_key, int depth_searched_beyond, int alpha, int beta, int side_to_move);
  static void RecordHash (Transposition *tt, size_t tt_size, const Transposition &t, int side_to_move);
};
