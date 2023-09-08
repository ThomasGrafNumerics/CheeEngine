
#include "board.h"
#include "types.h"

void
Board::print_board (void) const
{
  for (unsigned int rank = 0; rank < 8; ++rank)
    {
      std::cout << (8 - rank) << " ";
      for (unsigned int file = 0; file < 8; ++file)
        {
          int square = rank * 8 + file;
          int piece = -1;

          for (unsigned int bb_piece = Chess::P; bb_piece <= Chess::k; ++bb_piece)
            {
              if (boardstate.bitboards[bb_piece].get_bit (square))
                {
                  piece = bb_piece;
                }
            }
          (piece == -1) ? (std::cout << "."
                                     << "  ")
                        : (std::cout << unicode_Chess::piece_list_table[piece] << "  ");
        }
      std::cout << "\n";
    }
  std::cout << "  a  b  c  d  e  f  g  h"
            << "\n\n";

  (boardstate.side_to_move == Chess::white) ? (std::cout << "side to move: white\n")
                                            : (std::cout << "side to move: black\n");
  (boardstate.enpassant_square != Chess::forbidden_square)
      ? (std::cout << "enpassant square: " << square_to_string_table[boardstate.enpassant_square] << "\n")
      : (std::cout << "enpassant square: none\n");
  std::string wk, wq, bk, bq;
  wk = (boardstate.castle & Chess::white_king_side_castle) ? "k" : "-";
  wq = (boardstate.castle & Chess::white_queen_side_castle) ? "q" : "-";
  bk = (boardstate.castle & Chess::black_king_side_castle) ? "k" : "-";
  bq = (boardstate.castle & Chess::black_queen_side_castle) ? "q" : "-";
  std::cout << "castling: " << wk << " " << wq << " " << bk << " " << bq << "\n";
}

bool
Board::is_square_attacked (const unsigned int attack_side, const unsigned int square) const
{
  unsigned int pawn = Chess::piece_list_table[attack_side][Chess::pawn];
  if (attacks.pawn_attacks (attack_side ^ 1, square) & boardstate.bitboards[pawn])
    return true;

  unsigned int knight = Chess::piece_list_table[attack_side][Chess::knight];
  if (attacks.knight_attacks (square) & boardstate.bitboards[knight])
    return true;

  unsigned int rook = Chess::piece_list_table[attack_side][Chess::rook];
  if (attacks.rook_attacks (boardstate.occupancies[Chess::both_sides], square) & boardstate.bitboards[rook])
    return true;

  unsigned int bishop = Chess::piece_list_table[attack_side][Chess::bishop];
  if (attacks.bishop_attacks (boardstate.occupancies[Chess::both_sides], square)
      & boardstate.bitboards[bishop])
    return true;

  unsigned int queen = Chess::piece_list_table[attack_side][Chess::queen];
  if (attacks.queen_attacks (boardstate.occupancies[Chess::both_sides], square) & boardstate.bitboards[queen])
    return true;

  unsigned int king = Chess::piece_list_table[attack_side][Chess::king];
  if (attacks.king_attacks (square) & boardstate.bitboards[king])
    return true;

  return false;
}
//
// bool
// Board::is_king_attacked (void) const
// {
//   int king_side = boardstate.side_to_move;
//   int king = Chess::Chess::piece_list_table[king_side][KING];
//   int king_square = boardstate.bitboards[king].get_index_of_least_significant_set_bit ();
//   return is_square_attacked (king_side ^ 1, king_square);
// }
