#include "board.h"
#include "boardstate.h"
#include "types.h"
#include <algorithm>
#include <iterator>
#include <string>

Board::Board ()
    : boardstate (), boardstate_history (boardstate_stack_size), visited_nodes{ 0 }, pv_length{}, pv_table{}, killer_heuristic{},
      repeated_position{ new bool[repeated_position_size] }
{
  parseFEN ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

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
  boardstate_history.clear_stack ();
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
  const unsigned int king_square = boardstate.bitboards[king].get_index_of_least_significant_set_bit ();
  return is_square_attacked (!king_side, king_square);
}

std::vector<Move>
Board::get_pseudo_moves (void) const
{
  std::vector<Move> pseudo_moves;
  pseudo_moves.reserve (218);

  const bool side = boardstate.side_to_move;
  Bitboard not_occupancy = ~boardstate.occupancies[Chess::both_sides];

  const unsigned int pawn = Chess::piece_list_table[side][Chess::pawn];
  Bitboard pawn_push = (side == Chess::white ? boardstate.bitboards[pawn] >> 8 : boardstate.bitboards[pawn] << 8) & not_occupancy;

  for (Bitboard cpy_pawn_push = pawn_push; cpy_pawn_push; cpy_pawn_push.clear_least_significant_set_bit ())
    {
      const unsigned int dest_square = cpy_pawn_push.get_index_of_least_significant_set_bit ();
      const unsigned int source_square = (side == Chess::white ? dest_square + 8 : dest_square - 8);

      if (get_rank (dest_square) == rank_8 || get_rank (dest_square) == rank_1)
        {
          for (unsigned int promoted_piece_type = Chess::knight; promoted_piece_type <= Chess::queen; ++promoted_piece_type)
            {
              pseudo_moves.push_back (Move (source_square, dest_square, pawn, promoted_piece_type));
            }
        }
      else
        {
          pseudo_moves.push_back (Move (source_square, dest_square, pawn));
        }
    }

  Bitboard double_pawn_push = (side == Chess::white ? (pawn_push & rank_3_mask) >> 8 : (pawn_push & rank_6_mask) << 8) & not_occupancy;
  while (double_pawn_push)
    {
      const unsigned int dest_square = double_pawn_push.get_index_of_least_significant_set_bit ();
      const unsigned int source_square = (side == Chess::white ? dest_square + 16 : dest_square - 16);
      pseudo_moves.push_back (Move (source_square, dest_square, pawn, 0, Move::double_push_flag));
      double_pawn_push.clear_least_significant_set_bit ();
    }

  for (Bitboard pawn_bitboard = boardstate.bitboards[pawn]; pawn_bitboard; pawn_bitboard.clear_least_significant_set_bit ())
    {
      const unsigned int source_square = pawn_bitboard.get_index_of_least_significant_set_bit ();

      for (Bitboard pawn_attack = attacks.precomputed_pawn_attacks_table[side][source_square] & boardstate.occupancies[!side]; pawn_attack;
           pawn_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = pawn_attack.get_index_of_least_significant_set_bit ();

          if (get_rank (dest_square) == rank_8 || get_rank (dest_square) == rank_1)
            {
              for (unsigned int promoted_piece_type = Chess::knight; promoted_piece_type <= Chess::queen; ++promoted_piece_type)
                {
                  pseudo_moves.push_back (Move (source_square, dest_square, pawn, promoted_piece_type, Move::capture_flag));
                }
            }
          else
            {
              pseudo_moves.push_back (Move (source_square, dest_square, pawn, 0, Move::capture_flag));
            }
        }
    }

  if (boardstate.enpassant_square != Chess::forbidden_square)
    {
      const unsigned int dest_square = boardstate.enpassant_square;
      for (Bitboard pawn_attack = attacks.precomputed_pawn_attacks_table[!side][dest_square] & boardstate.bitboards[pawn]; pawn_attack;
           pawn_attack.clear_least_significant_set_bit ())
        {
          const unsigned int source_square = pawn_attack.get_index_of_least_significant_set_bit ();
          pseudo_moves.push_back (Move (source_square, dest_square, pawn, 0, Move::enpassant_flag));
        }
    }

  const unsigned int knight = Chess::piece_list_table[side][Chess::knight];
  for (Bitboard knight_bitboard = boardstate.bitboards[knight]; knight_bitboard; knight_bitboard.clear_least_significant_set_bit ())
    {
      const unsigned int source_square = knight_bitboard.get_index_of_least_significant_set_bit ();
      for (Bitboard knight_attack = attacks.knight_attacks (source_square) & ~boardstate.occupancies[side]; knight_attack; knight_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = knight_attack.get_index_of_least_significant_set_bit ();
          if (boardstate.occupancies[!side].get_bit (dest_square))
            pseudo_moves.push_back (Move (source_square, dest_square, knight, 0, Move::capture_flag));
          else
            pseudo_moves.push_back (Move (source_square, dest_square, knight));
        }
    }

  const unsigned int king = Chess::piece_list_table[side][Chess::king];
  if (boardstate.bitboards[king])
    {
      const unsigned int king_square = boardstate.bitboards[king].get_index_of_least_significant_set_bit ();
      for (Bitboard king_attack = attacks.king_attacks (king_square) & ~boardstate.occupancies[side]; king_attack; king_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = king_attack.get_index_of_least_significant_set_bit ();

          if (boardstate.occupancies[!side].get_bit (dest_square))
            pseudo_moves.push_back (Move (king_square, dest_square, king, 0, Move::capture_flag));
          else
            pseudo_moves.push_back (Move (king_square, dest_square, king));
        }

      for (unsigned int castle_type = Chess::king_side; castle_type <= Chess::queen_side; ++castle_type)
        {
          if (boardstate.castle & Chess::castle_permission_requirement_table[side][castle_type])
            {
              // check the occupancy
              if ((boardstate.occupancies[Chess::both_sides] & Chess::castle_get_occupancy_mask_table[side][castle_type]) == 0)
                {
                  // king and the adjacent squares cannot be in check
                  const unsigned int adj_square = (castle_type == Chess::king_side ? king_square + 1 : king_square - 1);
                  const unsigned int dest_square = (castle_type == Chess::king_side ? king_square + 2 : king_square - 2);

                  // do i need to check dest_square?
                  if (!is_square_attacked (!side, king_square) && !is_square_attacked (!side, adj_square) && !is_square_attacked (!side, dest_square))
                    {
                      pseudo_moves.push_back (Move (king_square, dest_square, king, 0, Move::castling_flag));
                    }
                }
            }
        }
    }

  const unsigned int bishop = Chess::piece_list_table[side][Chess::bishop];
  for (Bitboard bishop_bitboard = boardstate.bitboards[bishop]; bishop_bitboard; bishop_bitboard.clear_least_significant_set_bit ())
    {
      const unsigned int source_square = bishop_bitboard.get_index_of_least_significant_set_bit ();

      for (Bitboard bishop_attack = attacks.bishop_attacks (boardstate.occupancies[Chess::both_sides], source_square) & ~boardstate.occupancies[side]; bishop_attack;
           bishop_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = bishop_attack.get_index_of_least_significant_set_bit ();

          if (boardstate.occupancies[!side].get_bit (dest_square))
            pseudo_moves.push_back (Move (source_square, dest_square, bishop, 0, Move::capture_flag));
          else
            pseudo_moves.push_back (Move (source_square, dest_square, bishop));
        }
    }

  const unsigned int rook = Chess::piece_list_table[side][Chess::rook];
  for (Bitboard rook_bitboard = boardstate.bitboards[rook]; rook_bitboard; rook_bitboard.clear_least_significant_set_bit ())
    {
      const unsigned int source_square = rook_bitboard.get_index_of_least_significant_set_bit ();

      // knight attacks empty or enemy occupied squares
      for (Bitboard rook_attack = attacks.rook_attacks (boardstate.occupancies[Chess::both_sides], source_square) & ~boardstate.occupancies[side]; rook_attack;
           rook_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = rook_attack.get_index_of_least_significant_set_bit ();

          if (boardstate.occupancies[!side].get_bit (dest_square))
            pseudo_moves.push_back (Move (source_square, dest_square, rook, 0, Move::capture_flag));
          else
            pseudo_moves.push_back (Move (source_square, dest_square, rook));
        }
    }

  const unsigned int queen = Chess::piece_list_table[side][Chess::queen];
  for (Bitboard queen_bitboard = boardstate.bitboards[queen]; queen_bitboard; queen_bitboard.clear_least_significant_set_bit ())
    {
      const unsigned int source_square = queen_bitboard.get_index_of_least_significant_set_bit ();

      // knight attacks empty or enemy occupied squares
      for (Bitboard queen_attack = attacks.queen_attacks (boardstate.occupancies[Chess::both_sides], source_square) & ~boardstate.occupancies[side]; queen_attack;
           queen_attack.clear_least_significant_set_bit ())
        {
          const unsigned int dest_square = queen_attack.get_index_of_least_significant_set_bit ();

          if (boardstate.occupancies[!side].get_bit (dest_square))
            pseudo_moves.push_back (Move (source_square, dest_square, queen, 0, Move::capture_flag));
          else
            pseudo_moves.push_back (Move (source_square, dest_square, queen));
        }
    }

  return pseudo_moves;
}

// insane bottle neck
void
Board::save_board_state (void)
{
  repeated_position[boardstate.position_key & (repeated_position_size - 1)] = true;
  boardstate_history.push_back_stack (boardstate);
  // boardstate_copy = boardstate;
  // std::copy (std::begin (boardstate.bitboards), std::end (boardstate.bitboards), std::begin (copy_bitboards));
}

// insane bottle neck
void
Board::restore_state ()
{
  // boardstate = boardstate_copy;
  boardstate = boardstate_history.back_stack ();
  boardstate_history.pop_back_stack ();
  repeated_position[boardstate.position_key & (repeated_position_size - 1)] = false;
}

int
Board::get_captured_piece (Move move)
{

  int moved_piece = move.get_moved_piece ();
  int side = Chess::piece_side_table[moved_piece];
  int dest_square = move.get_dest ();

  for (unsigned int captured_piece = Chess::piece_list_table[!side][Chess::pawn]; captured_piece <= Chess::piece_list_table[!side][Chess::queen]; ++captured_piece)
    {
      if (boardstate.bitboards[captured_piece].get_bit (dest_square))
        {

          return captured_piece;
        }
    }

  std::cout << "WARNING: " << std::endl;
  return -1;
}

bool
Board::make_pseudo_move (Move move)
{
  save_board_state ();

  int side = boardstate.side_to_move;

  int source_square = move.get_source ();
  int dest_square = move.get_dest ();
  int moving_piece = move.get_moved_piece ();

  boardstate.bitboards[moving_piece].invert_bit (source_square);
  boardstate.bitboards[moving_piece].invert_bit (dest_square);
  boardstate.occupancies[side].invert_bit (source_square);
  boardstate.occupancies[side].invert_bit (dest_square);
  boardstate.hash_piece (moving_piece, source_square);
  boardstate.hash_piece (moving_piece, dest_square);

  if (move.is_capture ())
    {

      int captured_piece = get_captured_piece (move);
      boardstate.bitboards[captured_piece].invert_bit (dest_square);
      boardstate.occupancies[!side].invert_bit (dest_square);
      boardstate.hash_piece (captured_piece, dest_square);
    }

  int promoted_piece_type = move.get_promoted_piece_type ();
  if (promoted_piece_type)
    {
      int promoted_piece = Chess::piece_list_table[side][promoted_piece_type];
      boardstate.bitboards[moving_piece].invert_bit (dest_square);
      boardstate.bitboards[promoted_piece].invert_bit (dest_square);
      boardstate.hash_piece (moving_piece, dest_square);
      boardstate.hash_piece (promoted_piece, dest_square);
    }

  if (move.is_enpassant ())
    {
      int captured_pawn = Chess::piece_list_table[!side][Chess::pawn];
      int captured_pawn_square = (side == Chess::white ? dest_square + 8 : dest_square - 8);

      boardstate.bitboards[captured_pawn].invert_bit (captured_pawn_square);
      boardstate.occupancies[!side].invert_bit (captured_pawn_square);
      boardstate.hash_piece (captured_pawn, captured_pawn_square);
    }

  // unhash the old enpassant square
  if (boardstate.enpassant_square != Chess::forbidden_square)
    {
      boardstate.hash_enpassant (boardstate.enpassant_square);
    }

  if (move.is_double_push ())
    {
      boardstate.enpassant_square = (side == Chess::white ? source_square - 8 : source_square + 8);
      boardstate.hash_enpassant (boardstate.enpassant_square);
    }
  else
    {
      boardstate.enpassant_square = Chess::forbidden_square;
    }

  if (move.is_castling ())
    {
      int rook = Chess::piece_list_table[side][Chess::rook];
      int rook_source = 0, rook_dest = 0;

      switch (dest_square)
        {
        case Chess::g1:
          rook_source = Chess::h1;
          rook_dest = Chess::f1;
          break;
        case Chess::c1:
          rook_source = Chess::a1;
          rook_dest = Chess::d1;
          break;
        case Chess::g8:
          rook_source = Chess::h8;
          rook_dest = Chess::f8;
          break;
        case Chess::c8:
          rook_source = Chess::a8;
          rook_dest = Chess::d8;
          break;
        }

      // flip bit is cheaper
      boardstate.bitboards[rook].invert_bit (rook_source);
      boardstate.bitboards[rook].invert_bit (rook_dest);
      boardstate.occupancies[side].invert_bit (rook_source);
      boardstate.occupancies[side].invert_bit (rook_dest);
      boardstate.hash_piece (rook, rook_source);
      boardstate.hash_piece (rook, rook_dest);
    }

  boardstate.hash_castle (boardstate.castle);
  boardstate.castle &= castling_permission_filter_table[source_square];
  boardstate.castle &= castling_permission_filter_table[dest_square];
  boardstate.hash_castle (boardstate.castle);

  // update the occupancies
  boardstate.occupancies[Chess::both_sides] = boardstate.occupancies[Chess::white] | boardstate.occupancies[Chess::black];

  // update fifty moves rule
  if (moving_piece == Chess::white_pawn || moving_piece == Chess::black_pawn || move.is_capture () || move.is_enpassant ())
    boardstate.fifty_moves = 0;
  else
    ++boardstate.fifty_moves;

  // check for check
  bool in_check = is_king_attacked ();

  if (in_check)
    {
      restore_state ();
      return false;
    }
  else
    {
      boardstate.side_to_move = boardstate.side_to_move ^ 1;
      boardstate.hash_side_to_move ();
      return true;
    }
}

void
Board::perf_test (int depth)
{
  if (depth == 0)
    {
      ++visited_nodes;
      return;
    }

  std::vector<Move> pseudo_moves = get_pseudo_moves ();

  for (Move move : pseudo_moves)
    {
      // save_board_state ();
      // std::cout << "BEFORE" << std::endl;
      // this->print_board ();
      if (make_pseudo_move (move))
        {
          // move.print_move ();
          // std::cout << std::endl;
          // this->print_board ();
          perf_test (depth - 1);
          restore_state ();
        }
    }

  return;
}

void
Board::parse_perf_test (const std::string &perf_str)
{
  size_t index = perf_str.find ("depth") + 6;
  if (index < std::string::npos)
    {
      visited_nodes = 0;
      int max_depth = stoi (perf_str.substr (index));

      std::chrono::high_resolution_clock::time_point timer = std::chrono::high_resolution_clock::now ();
      perf_test (max_depth);
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now () - timer);

      std::cout << "Node: " << visited_nodes << '\n';
      std::cout << "Time: " << duration.count () << " ms\n";
      std::cout << "Speed: " << visited_nodes / duration.count () << " knode/s" << std::endl;
    }
}

bool
Board::parse_move (const std::string &move_str)
{
  if (move_str.size () < 4)
    return false;

  int source_file = to_file (move_str[0]);
  int source_rank = to_rank (move_str[1]);
  int dest_file = to_file (move_str[2]);
  int dest_rank = to_rank (move_str[3]);
  int promoted_piece_type = 0;
  if (move_str.size () >= 5)
    promoted_piece_type = Chess::fen_to_piece_type (move_str[4]);

  int source_square = source_rank * 8 + source_file;
  int dest_square = dest_rank * 8 + dest_file;

  std::vector<Move> pseudo_moves = get_pseudo_moves ();
  for (Move pseudo_move : pseudo_moves)
    {
      if (pseudo_move.get_source () == source_square && pseudo_move.get_dest () == dest_square && pseudo_move.get_promoted_piece_type () == promoted_piece_type)
        {
          if (make_pseudo_move (pseudo_move))
            return true;
          else
            return false;
        }
    }

  return false;
}

void
Board::parse_position (const std::string &position_str)
{
  size_t index = 9;

  if (position_str.substr (index, 8) == "startpos")
    {
      index += 9;
      parseFEN ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
  else if (position_str.substr (index, 8) == "kiwipete")
    {
      index += 9;
      parseFEN ("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    }
  else if (position_str.substr (index, 7) == "DTZ1033")
    {
      index += 8;
      parseFEN ("8/r6n/8/8/5k2/3K4/7N/3b3Q b - - 0 0");
    }
  else if (position_str.substr (index, 7) == "endgame")
    {
      index += 8;
      parseFEN ("8/P7/8/8/8/4k3/1K6/8 w - - 1 5");
    }
  else if (position_str.substr (index, 3) == "fen")
    {
      index += 4;
      parseFEN (position_str.substr (index));
    }

  if (index < position_str.size ())
    {
      // 4 bytes assembly level KMP
      // index = strstr(position_str.c_str(), "move") - position_str.c_str();
      index = position_str.find ("moves");
      if (index != std::string::npos)
        {
          index += 5;
          while (index != std::string::npos)
            {
              ++index;
              bool parsed_success = parse_move (position_str.substr (index));

              if (parsed_success)
                index = position_str.find (" ", index);
              else
                break;
            }
        }
    }
}

void
Board::UCI ()
{
  std::ios_base::sync_with_stdio (false);

  std::string command;
  while (true)
    {
      getline (std::cin, command);
      if (command == "uci")
        {
          std::cout << "id name LeeChess\n";
          std::cout << "uciok" << std::endl;
        }
      else if (command.find ("isready") != std::string::npos)
        std::cout << "readyok" << std::endl;
      else if (command.find ("position") != std::string::npos)
        parse_position (command);
      else if (command.find ("ucinewgame") != std::string::npos)
        parse_position ("position startpos");
      else if (command.find ("perft") != std::string::npos)
        parse_perf_test (command);
      // else if (command.find ("go") != std::string::npos)
      //   parse_go (command);
      else if (command.find ("takeback") != std::string::npos)
        restore_state ();
      // else if (command.find ("train") != std::string::npos)
      //   parse_train (command);
      else if (command.find ("quit") != std::string::npos)
        break;
      else
        std::cout << "Invaild command\a\n";

      print_board ();
    }
}
