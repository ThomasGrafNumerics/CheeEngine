#pragma once

#include "attacks.h"
#include "bitboard.h"
#include "boardstate.h"
#include "boardstate_stack.h"
#include "magics.h"
#include "move.h"
#include "slow_attacks.h"
#include "timer.h"
#include "types.h"
#include <vector>

extern Bitboard copy_bitboards[12];
extern Bitboard copy_occupancies[3];
extern bool copy_side_to_move;
extern unsigned int copy_castle;
extern unsigned int copy_enpassant_square;
extern unsigned int copy_fifty_moves;
extern uint64_t copy_position_key;

class Board
{
private:
  static constexpr int max_searching_depth = 30;
  static constexpr size_t boardstate_stack_size = 256;
  static constexpr int null_move_depth_required = 1;
  static constexpr int null_move_piece_required = 2;
  static constexpr int null_move_depth_reduction = 3;
  static constexpr int late_move_searched_required = 4; // change this value if the engine failed to find good moves
  static constexpr int late_move_depth_reduction = 3;
  static constexpr size_t repeated_position_size = 262144;    // power of 2 for fast modulo
  static constexpr size_t transposition_table_size = 8388608; // power of 2 for fast modulo

public:
  Boardstate boardstate;
  Boardstate_Stack boardstate_history;
  // Boardstate boardstate_copy;
  unsigned long long visited_nodes;
  int pv_length[max_searching_depth];
  Move pv_table[max_searching_depth][max_searching_depth]; // searching depth, move index
  Move killer_heuristic[max_searching_depth][2];
  bool *repeated_position;

  Attacks attacks;

  // Transposition *transposition_table;
  Board (void);
  Board (Board &&) = default;
  Board (const Board &) = default;
  Board &operator= (Board &&) = default;
  Board &operator= (const Board &) = default;
  ~Board (void) = default;

  void print_board (void) const;
  void parseFEN (const std::string &);
  bool is_square_attacked (const bool, const unsigned int) const;
  bool is_king_attacked (void) const;
  std::vector<Move> get_pseudo_moves (void) const;
  void save_board_state (void);
  void restore_state (void);
  int get_captured_piece (Move);
  bool make_pseudo_move (Move);
  void perf_test (int);
  void parse_perf_test (const std::string &);
  bool parse_move (const std::string &);
};
