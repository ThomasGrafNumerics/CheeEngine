
#include "board.h"
#include "boardstate.h"
#include "types.h"

// Board::Board () : boardstate () {}

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

  (boardstate.side_to_move == Chess::white) ? (std::cout << "side to move: white\n") : (std::cout << "side to move: black\n");
  (boardstate.enpassant_square != Chess::forbidden_square) ? (std::cout << "enpassant square: " << square_to_string_table[boardstate.enpassant_square] << "\n")
                                                           : (std::cout << "enpassant square: none\n");
  std::string wk, wq, bk, bq;
  wk = (boardstate.castle & Chess::white_king_side_castle) ? "K" : "-";
  wq = (boardstate.castle & Chess::white_queen_side_castle) ? "Q" : "-";
  bk = (boardstate.castle & Chess::black_king_side_castle) ? "k" : "-";
  bq = (boardstate.castle & Chess::black_queen_side_castle) ? "q" : "-";
  std::cout << "castling rights: " << wk << " " << wq << " " << bk << " " << bq << "\n";
}

void
Board::parseFEN (const std::string &fen)
{
  boardstate.clear_boardstate ();
  visited_nodes = 0;
  auto iter = fen.begin ();
  for (unsigned int square = 0; square < 64;)
    {
      const char letter = *(iter++);
      if (isdigit (letter))
        {
          square += letter - '0';
        }
      else if (isalpha (letter))
        {
          boardstate.bitboards[Chess::fen_to_piece (letter)].set_bit (square++);
        }
    }

  for (unsigned int piece = Chess::white_pawn; piece <= Chess::white_king; ++piece)
    {
      boardstate.occupancies[Chess::white] |= boardstate.bitboards[piece];
    }
  for (unsigned int piece = Chess::black_pawn; piece <= Chess::black_king; ++piece)
    {
      boardstate.occupancies[Chess::black] |= boardstate.bitboards[piece];
    }
  boardstate.occupancies[Chess::both_sides] = boardstate.occupancies[Chess::white] | boardstate.occupancies[Chess::black];

  boardstate.side_to_move = (*(++iter) == 'w' ? Chess::white : Chess::black);
  iter += 2;

  while (true)
    {
      if (*iter == ' ')
        break;

      switch (*iter)
        {
        case 'K':
          boardstate.castle |= Chess::white_king_side_castle;
          break;
        case 'Q':
          boardstate.castle |= Chess::white_queen_side_castle;
          break;
        case 'k':
          boardstate.castle |= Chess::black_king_side_castle;
          break;
        case 'q':
          boardstate.castle |= Chess::black_queen_side_castle;
          break;
        }
      ++iter;
    }

  ++iter;

  if (*iter != '-')
    {
      unsigned int file = to_file (*iter);
      ++iter;
      unsigned int rank = to_rank (*iter);
      boardstate.enpassant_square = rank * 8 + file;
    }
  iter += 2;
}

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
  if (attacks.bishop_attacks (boardstate.occupancies[Chess::both_sides], square) & boardstate.bitboards[bishop])
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

std::vector<Move>
Board::get_pseudo_moves (void) const
{
  std::vector<Move> pseudo_moves;
  pseudo_moves.reserve (218);

  int side = boardstate.side_to_move;
  int pawn = Chess::piece_list_table[side][Chess::pawn];

  // reverse the occupancy, where bit 1 represents an empty space
  Bitboard not_occupancy = ~boardstate.occupancies[Chess::both_sides];
  //
  // 	Bitboard pawn_push = (side == WHITE ? boardstate.bitboards[pawn] >> 8 :
  // boardstate.bitboards[pawn] << 8) & not_occupancy; 	for (Bitboard cpy_pawn_push = pawn_push;
  // cpy_pawn_push; cpy_pawn_push.PopBit())
  // 	{
  // 		int dest_square = cpy_pawn_push.GetLeastSigBit();
  // 		int source_square = (side == WHITE ? dest_square + 8 : dest_square - 8);
  //
  // 		//no need to check for the side, because pawn can not move backward
  // 		if (GetRank(dest_square) == RANK_8 || GetRank(dest_square) == RANK_1)
  // 		{
  // 			for (int promoted_piece_type = KNIGHT; promoted_piece_type <= QUEEN;
  // ++promoted_piece_type)
  // 			{
  // 				pseudo_moves.push_back(Move(source_square, dest_square, pawn,
  // promoted_piece_type));
  // 			}
  // 		}
  // 		else
  // 		{
  // 			pseudo_moves.push_back(Move(source_square, dest_square, pawn));
  // 		}
  // 	}
  //
  //
  // 	Bitboard double_pawn_push = (side == WHITE ? (pawn_push & RANK_3_MASK) >> 8 : (pawn_push &
  // RANK_6_MASK) << 8) & not_occupancy; 	while (double_pawn_push)
  // 	{
  // 		int dest_square = double_pawn_push.GetLeastSigBit();
  // 		int source_square = (side == WHITE ? dest_square + 16 : dest_square - 16);
  // 		pseudo_moves.push_back(Move(source_square, dest_square, pawn, 0, Move::DOUBLE_PUSH_FLAG));
  // 		double_pawn_push.PopBit();
  // 	}
  //
  //
  // 	for (Bitboard pawn_bitboard = boardstate.bitboards[pawn]; pawn_bitboard; pawn_bitboard.PopBit())
  // 	{
  // 		int source_square = pawn_bitboard.GetLeastSigBit();
  //
  // 		for (Bitboard pawn_attack = PAWN_ATTACK_TABLE[side][source_square] &
  // boardstate.occupancies[!side]; pawn_attack; pawn_attack.PopBit())
  // 		{
  // 			int dest_square = pawn_attack.GetLeastSigBit();
  //
  // 			if (GetRank(dest_square) == RANK_8 || GetRank(dest_square) == RANK_1)
  // 			{
  // 				for (int promoted_piece_type = KNIGHT; promoted_piece_type <= QUEEN;
  // ++promoted_piece_type)
  // 				{
  // 					pseudo_moves.push_back(Move(source_square, dest_square, pawn,
  // promoted_piece_type, Move::CAPTURE_FLAG));
  // 				}
  // 			}
  // 			else
  // 			{
  // 				pseudo_moves.push_back(Move(source_square, dest_square, pawn, 0,
  // Move::CAPTURE_FLAG));
  // 			}
  // 		}
  // 	}
  //
  //
  // 	if (boardstate.enpassant_square != INVALID_SQUARE)
  // 	{
  // 		int dest_square = boardstate.enpassant_square;
  //
  // 		//simulate an attack from the being captured side, the source must be a pawn
  // 		for (Bitboard pawn_attack = PAWN_ATTACK_TABLE[!side][dest_square] &
  // boardstate.bitboards[pawn]; pawn_attack; pawn_attack.PopBit())
  // 		{
  // 			int source_square = pawn_attack.GetLeastSigBit();
  //
  // 			//do i need capture flag?
  // 			pseudo_moves.push_back(Move(source_square, dest_square, pawn, 0,
  // Move::ENPASSANT_FLAG));
  // 		}
  // 	}
  //
  //
  //
  //
  //
  //
  // 	int knight = PIECE_LIST_TABLE[side][KNIGHT];
  // 	for (Bitboard knight_bitboard = boardstate.bitboards[knight]; knight_bitboard;
  // knight_bitboard.PopBit())
  // 	{
  // 		int source_square = knight_bitboard.GetLeastSigBit();
  //
  // 		//knight attacks empty or enemy occupied squares
  // 		for (Bitboard knight_attack = GetKnightAttackExact(source_square) &
  // ~boardstate.occupancies[side]; knight_attack; knight_attack.PopBit())
  // 		{
  // 			int dest_square = knight_attack.GetLeastSigBit();
  // 			if(boardstate.occupancies[!side].GetBit(dest_square))
  // pseudo_moves.push_back(Move(source_square, dest_square, knight, 0, Move::CAPTURE_FLAG));
  // else pseudo_moves.push_back(Move(source_square, dest_square, knight));
  // 		}
  // 	}
  //
  //
  //
  //
  // 	int king = PIECE_LIST_TABLE[side][KING];
  // 	int king_square = boardstate.bitboards[king].GetLeastSigBit();
  //
  // 	for (Bitboard king_attack = GetKingAttackExact(king_square) & ~boardstate.occupancies[side];
  // king_attack; king_attack.PopBit())
  // 	{
  // 		int dest_square = king_attack.GetLeastSigBit();
  //
  // 		if (boardstate.occupancies[!side].GetBit(dest_square))
  // pseudo_moves.push_back(Move(king_square, dest_square, king, 0, Move::CAPTURE_FLAG)); 		else
  // pseudo_moves.push_back(Move(king_square, dest_square, king));
  // 	}
  //
  // 	for (int castle_type = KING_SIDE; castle_type <= QUEEN_SIDE; ++castle_type)
  // 	{
  // 		//check for the permission
  // 		if (boardstate.castle & CASTLE_PERMISSION_REQUIREMENT_TABLE[side][castle_type])
  // 		{
  // 			//check the occupancy
  // 			if ((boardstate.occupancies[BOTH] &
  // CASTLE_GET_OCCUPANCY_MASK_TABLE[side][castle_type]) == 0)
  // 			{
  // 				//king and the adjacent squares can not be in checked
  // 				int adj_square = (castle_type == KING_SIDE ? king_square + 1 : king_square -
  // 1); 				int dest_square = (castle_type == KING_SIDE ? king_square + 2 :
  // king_square
  // - 2);
  //
  // 				if (!IsSquareAttacked(king_square, side^1) && !IsSquareAttacked(adj_square,
  // side^1))
  // 				{
  // 					pseudo_moves.push_back(Move(king_square, dest_square, king, 0,
  // Move::CASTLING_FLAG));
  // 				}
  // 			}
  // 		}
  // 	}
  //
  //
  //
  //
  //
  //
  // 	int bishop = PIECE_LIST_TABLE[side][BISHOP];
  // 	for (Bitboard bishop_bitboard = boardstate.bitboards[bishop]; bishop_bitboard;
  // bishop_bitboard.PopBit())
  // 	{
  // 		int source_square = bishop_bitboard.GetLeastSigBit();
  //
  // 		for (Bitboard bishop_attack = GetBishopAttackExact(source_square,
  // boardstate.occupancies[BOTH]) & ~boardstate.occupancies[side]; bishop_attack;
  // bishop_attack.PopBit())
  // 		{
  // 			int dest_square = bishop_attack.GetLeastSigBit();
  //
  // 			if (boardstate.occupancies[!side].GetBit(dest_square))
  // pseudo_moves.push_back(Move(source_square, dest_square, bishop, 0, Move::CAPTURE_FLAG));
  // else pseudo_moves.push_back(Move(source_square, dest_square, bishop));
  // 		}
  // 	}
  //
  //
  //
  //
  //
  // 	int rook = PIECE_LIST_TABLE[side][ROOK];
  // 	for (Bitboard rook_bitboard = boardstate.bitboards[rook]; rook_bitboard; rook_bitboard.PopBit())
  // 	{
  // 		int source_square = rook_bitboard.GetLeastSigBit();
  //
  // 		//knight attacks empty or enemy occupied squares
  // 		for (Bitboard rook_attack = GetRookAttackExact(source_square,
  // boardstate.occupancies[BOTH]) & ~boardstate.occupancies[side]; rook_attack;
  // rook_attack.PopBit())
  // 		{
  // 			int dest_square = rook_attack.GetLeastSigBit();
  //
  // 			if (boardstate.occupancies[!side].GetBit(dest_square))
  // pseudo_moves.push_back(Move(source_square, dest_square, rook, 0, Move::CAPTURE_FLAG));
  // else pseudo_moves.push_back(Move(source_square, dest_square, rook));
  // 		}
  // 	}
  //
  //
  //
  //
  //
  // 	int queen = PIECE_LIST_TABLE[side][QUEEN];
  // 	for (Bitboard queen_bitboard = boardstate.bitboards[queen]; queen_bitboard;
  // queen_bitboard.PopBit())
  // 	{
  // 		int source_square = queen_bitboard.GetLeastSigBit();
  //
  // 		//knight attacks empty or enemy occupied squares
  // 		for (Bitboard queen_attack = GetQueenAttackExact(source_square,
  // boardstate.occupancies[BOTH]) & ~boardstate.occupancies[side]; queen_attack;
  // queen_attack.PopBit())
  // 		{
  // 			int dest_square = queen_attack.GetLeastSigBit();
  //
  // 			if (boardstate.occupancies[!side].GetBit(dest_square))
  // pseudo_moves.push_back(Move(source_square, dest_square, queen, 0, Move::CAPTURE_FLAG));
  // else pseudo_moves.push_back(Move(source_square, dest_square, queen));
  // 		}
  // 	}
  //
  return pseudo_moves;
}
