#include "move.h"
#include "board.h"
#include "types.h"
#include <iostream>

int
Move::get_source () const
{
  return move & get_square_mask;
}
int
Move::get_dest () const
{
  return (move >> 6) & get_square_mask;
}
int
Move::get_moved_piece () const
{
  return (move >> 12) & get_piece;
}
int
Move::get_promoted_piece_type () const
{
  return (move >> 16) & get_piece;
}
bool
Move::is_capture () const
{
  return move & capture_flag;
}
bool
Move::is_enpassant () const
{
  return move & enpassant_flag;
}
bool
Move::is_double_push () const
{
  return move & double_push_flag;
}
bool
Move::is_castling () const
{
  return move & castling_flag;
}
bool
Move::is_quiet_move () const
{
  return !(move & violent_move_flag);
}
bool
Move::is_move_empty () const
{
  return move == 0;
}
void
Move::clear ()
{
  move = 0;
}
void
Move::print_move () const
{
  std::cout << square_to_string_table[get_source ()] << square_to_string_table[get_dest ()];
  int promoted_piece = get_promoted_piece_type ();
  if (promoted_piece)
    std::cout << Chess::piece_to_fen (promoted_piece);
}

Move::operator int () const { return move; }
Move::operator int & () { return move; }

Move::Move () : move (0) {}
Move::Move (int new_move) : move{ new_move } {}
Move::Move (int source_square, int dest_square, int piece, int promoted_piece_type, int flag)
    : move{ source_square | (dest_square << 6) | (piece << 12) | (promoted_piece_type << 16) | flag }
{
}
