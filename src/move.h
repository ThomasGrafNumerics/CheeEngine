#pragma once

#include "types.h"

class Move
{

  // clang-format off
  	/*
	            binary move bits           hexidecimal constants
         
         0000 0000 0000 0000 0011 1111    source square               0x3f
         0000 0000 0000 1111 1100 0000    dest square         >> 6  & 0x3f
         0000 0000 1111 0000 0000 0000    moved piece         >> 12 & 0xf
         0000 1111 0000 0000 0000 0000    promoted piece      >> 16 & 0xf
         0001 0000 0000 0000 0000 0000    capture flag           0x100000
         0010 0000 0000 0000 0000 0000    enpassant flag         0x200000
         0100 0000 0000 0000 0000 0000    double push flag       0x400000
         1000 0000 0000 0000 0000 0000    castling flag          0x800000
	*/
  // clang-format on
public:
  Move (void);
  Move (Move &&) = default;
  Move (const Move &) = default;
  Move &operator= (Move &&) = default;
  Move &operator= (const Move &) = default;
  ~Move () = default;

  int get_source () const;
  int get_dest () const;
  int get_moved_piece () const;
  int get_promoted_piece_type () const;
  bool is_capture () const;
  bool is_enpassant () const;
  bool is_double_push () const;
  bool is_castling () const;
  bool is_quiet_move () const;
  bool is_move_empty () const;
  void clear ();
  void print_move () const;

  operator int () const;
  operator int & ();

  Move (int new_move);
  Move (int source_square, int dest_square, int piece, int promoted_piece_type = 0, int flag = 0);

  static constexpr int get_square_mask = 0x3f;
  static constexpr int get_piece = 0xf;
  static constexpr int capture_flag = 0x100000;
  static constexpr int enpassant_flag = 0x200000;
  static constexpr int double_push_flag = 0x400000;
  static constexpr int castling_flag = 0x800000;
  static constexpr int violent_move_flag = get_piece << 16 | capture_flag | enpassant_flag;

  // move ordering
  static constexpr int futility_move_priority = 0;
  static constexpr int killer_move = 99;
  static constexpr int enpassant_priority = 105;
  static constexpr int promotion_priority = 500;

  // clang-format off
    static constexpr int capture_priority_table[12][12] = 
    {
        {0, 0, 0, 0, 0, 0,  105, 205, 305, 405, 505, 0},
        {0, 0, 0, 0, 0, 0,  104, 204, 304, 404, 504, 0},
        {0, 0, 0, 0, 0, 0,  103, 203, 303, 403, 503, 0},
        {0, 0, 0, 0, 0, 0,  102, 202, 302, 402, 502, 0},
        {0, 0, 0, 0, 0, 0,  101, 201, 301, 401, 501, 0},
        {0, 0, 0, 0, 0, 0,  100, 200, 300, 400, 500, 0},
        {105, 205, 305, 405, 505, 0, 0, 0, 0, 0, 0, 0,},
        {104, 204, 304, 404, 504, 0, 0, 0, 0, 0, 0, 0,},
        {103, 203, 303, 403, 503, 0, 0, 0, 0, 0, 0, 0,},
        {102, 202, 302, 402, 502, 0, 0, 0, 0, 0, 0, 0,},
        {101, 201, 301, 401, 501, 0, 0, 0, 0, 0, 0, 0,},
        {100, 200, 300, 400, 500, 0, 0, 0, 0, 0, 0, 0,}
    };
  // clang-format on
  static constexpr int principal_variation_priority = 10000;
  static constexpr int transposition_priority = 20000;

private:
  int move;
};
