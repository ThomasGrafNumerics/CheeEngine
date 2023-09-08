
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
          const unsigned int square = rank * 8 + file;
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
                        : (std::cout << unicode_symbols[piece] << "  ");
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
  wk = (boardstate.castle & Chess::white_king_side_castle) ? "K" : "-";
  wq = (boardstate.castle & Chess::white_queen_side_castle) ? "Q" : "-";
  bk = (boardstate.castle & Chess::black_king_side_castle) ? "k" : "-";
  bq = (boardstate.castle & Chess::black_queen_side_castle) ? "q" : "-";
  std::cout << "castling rights: " << wk << " " << wq << " " << bk << " " << bq << "\n";
}

// void
// Board::parseFEN (const std::string &FEN)
// {
//   boardstate.clear_boardstate ();
//   visited_nodes = 0;
//   // auto iter = FEN.begin ();
//   for (unsigned int square = 0; square < 64;)
//     {
// char letter = *(iter++);
// if (isdigit (letter))
//   {
//     square += letter - '0';
//   }
// else if (isalpha (letter))
//   {
//     boardstate.bitboards[FenToPiece (letter)].set_bit (square++);
//   }
// }
//
// 	for (int piece = WHITE_PAWN; piece <= WHITE_KING; ++piece)
// 	{
// 		boardstate.occupancies[WHITE] |= boardstate.bitboards[piece];
// 	}
// 	for (int piece = BLACK_PAWN; piece <= BLACK_KING; ++piece)
// 	{
// 		boardstate.occupancies[BLACK] |= boardstate.bitboards[piece];
// 	}
// 	boardstate.occupancies[BOTH_SIDES] = boardstate.occupancies[WHITE] | boardstate.occupancies[BLACK];
//
// 	boardstate.side_to_move = (*(++iter) == 'w' ? WHITE : BLACK);
// 	iter += 2;
//
// 	while (true)
// 	{
// 		if (*iter == ' ') break;
//
// 		switch (*iter)
// 		{
// 			case 'K': boardstate.castle |= WHITE_KING_SIDE_CASTLE; break;
// 			case 'Q': boardstate.castle |= WHITE_QUEEN_SIDE_CASTLE; break;
// 			case 'k': boardstate.castle |= BLACK_KING_SIDE_CASTLE; break;
// 			case 'q': boardstate.castle |= BLACK_QUEEN_SIDE_CASTLE; break;
// 		}
// 		++iter;
// 	}
//
// 	++iter;
//
// 	if (*iter != '-')
// 	{
// 		int file = ToFile(*iter);
// 		++iter;
// 		int rank = ToRank(*iter);
// 		boardstate.enpassant_square = rank * 8 + file;
// 	}
// 	iter += 2;
// }

bool
Board::is_square_attacked (const bool attack_side, const unsigned int square) const
{
  const unsigned int pawn = Chess::piece_list_table[attack_side][Chess::pawn];
  if (attacks.pawn_attacks (!attack_side, square) & boardstate.bitboards[pawn])
    return true;

  const unsigned int knight = Chess::piece_list_table[attack_side][Chess::knight];
  if (attacks.knight_attacks (square) & boardstate.bitboards[knight])
    return true;

  const unsigned int rook = Chess::piece_list_table[attack_side][Chess::rook];
  if (attacks.rook_attacks (boardstate.occupancies[Chess::both_sides], square) & boardstate.bitboards[rook])
    return true;

  const unsigned int bishop = Chess::piece_list_table[attack_side][Chess::bishop];
  if (attacks.bishop_attacks (boardstate.occupancies[Chess::both_sides], square)
      & boardstate.bitboards[bishop])
    return true;

  const unsigned int queen = Chess::piece_list_table[attack_side][Chess::queen];
  if (attacks.queen_attacks (boardstate.occupancies[Chess::both_sides], square) & boardstate.bitboards[queen])
    return true;

  const unsigned int king = Chess::piece_list_table[attack_side][Chess::king];
  if (attacks.king_attacks (square) & boardstate.bitboards[king])
    return true;

  return false;
}

bool
Board::is_king_attacked (void) const
{
  const bool king_side = boardstate.side_to_move;
  const unsigned int king = Chess::piece_list_table[king_side][Chess::king];
  int king_square = boardstate.bitboards[king].get_index_of_least_significant_set_bit ();
  return is_square_attacked (!king_side, king_square);
}
