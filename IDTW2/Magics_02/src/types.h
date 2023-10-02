#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// user defined literal for std::uint64_t type
constexpr std::uint64_t
operator"" _uint64 (unsigned long long arg)
{
  return static_cast<std::uint64_t> (arg);
}

unsigned int get_rank (const unsigned int);
unsigned int get_file (const unsigned int);
unsigned int to_rank (const char);
unsigned int to_file (const char);

// some constants
constexpr uint64_t not_a_file_mask{ 18374403900871474942_uint64 };
constexpr uint64_t not_h_file_mask{ 9187201950435737471_uint64 };
constexpr uint64_t not_ab_file_mask{ 18229723555195321596_uint64 };
constexpr uint64_t not_gh_file_mask{ 4557430888798830399_uint64 };
constexpr uint64_t rank_7_mask{ 65280 };
constexpr uint64_t rank_6_mask{ 16711680 };
constexpr uint64_t rank_3_mask{ 280375465082880 };
constexpr uint64_t rank_2_mask{ 71776119061217280 };

// some mnemonics
// uncomment for light-mode terminal background
// constexpr std::array<const char *, 12> unicode_symbols = { "♙", "♘", "♗", "♖", "♕", "♔", "♟︎", "♞", "♝", "♜", "♛", "♚" };
// uncomment for dark-mode terminal background
constexpr std::array<const char *, 12> unicode_symbols = { "♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔" };

// clang-format off
constexpr std::array<const char*, 64> square_to_string_table =
{
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// FEN dedug positions
	const std::string empty_board { "8/8/8/8/8/8/8/8 b - - "};
	const std::string start_position {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "};
	const std::string tricky_position {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "};
	const std::string killer_position {"rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"};
	const std::string cmk_position {"r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "};
	const std::string repetitions {"2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 "};
	const std::string en_passant_position {"rnbqkbnr/1ppp1ppp/p7/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 4 "};

enum  ranks
{
	rank_8, rank_7, rank_6, rank_5, rank_4, rank_3, rank_2, rank_1
};

enum  files
{
	file_a, file_b, file_c, file_d, file_e, file_f, file_g, file_h
};

constexpr int castling_permission_filter_table[64] = {
	 7, 15, 15, 15,  3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
};

  namespace Chess
  {

	enum initial_letters {P, N, B, R, Q, K, p, n, b, r, q, k};

	enum squares
	{
		a8, b8, c8, d8, e8, f8, g8, h8,
		a7, b7, c7, d7, e7, f7, g7, h7,
		a6, b6, c6, d6, e6, f6, g6, h6,
		a5, b5, c5, d5, e5, f5, g5, h5,
		a4, b4, c4, d4, e4, f4, g4, h4,
		a3, b3, c3, d3, e3, f3, g3, h3,
		a2, b2, c2, d2, e2, f2, g2, h2,
		a1, b1, c1, d1, e1, f1, g1, h1,
		forbidden_square = 64
	};

  enum colors {white, black, both_sides};

	enum piece_types {pawn, knight, bishop, rook, queen, king};

	enum pieces
	{
		white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king,
		black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
	};

	constexpr unsigned int piece_list_table[2][6] =
	{
		{white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king},
		{black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king}
	};

	enum castling_rights
	{
		white_king_side_castle =  0b0001,
		white_queen_side_castle = 0b0010,
		black_king_side_castle =  0b0100,
		black_queen_side_castle = 0b1000 
	};

	constexpr unsigned int fen_to_piece(const char FEN)
	{
		switch (FEN)
		{
		case 'P': return white_pawn;
		case 'N': return white_knight;
		case 'B': return white_bishop;
		case 'R': return white_rook;
		case 'Q': return white_queen;
		case 'K': return white_king;
		case 'p': return black_pawn;
		case 'n': return black_knight;
		case 'b': return black_bishop;
		case 'r': return black_rook;
		case 'q': return black_queen;
		case 'k': return black_king;
		default:  return 420;
		}
	}

	constexpr unsigned int fen_to_piece_type(const char FEN)
	{
		switch (FEN)
		{
		case 'P':
		case 'p':
			return pawn;
		case 'N':
		case 'n':
			return knight;
		case 'B':
		case 'b':
			return bishop;
		case 'R':
		case 'r':
			return rook;
		case 'Q':
		case 'q':
			return queen;
		case 'K':
		case 'k':
			return king;
		default:
			return 0;
		}
	}

  constexpr char piece_to_fen(const unsigned int piece)
  {
    switch (piece)
    {
    case white_pawn:   return 'P';
    case white_knight: return 'N';
    case white_bishop: return 'B';
    case white_rook:   return 'R';
    case white_queen:  return 'Q';
    case white_king:   return 'K';
    case black_pawn:   return 'p';
    case black_knight: return 'n';
    case black_bishop: return 'b';
    case black_rook:   return 'r';
    case black_queen:  return 'q';
    case black_king:   return 'k';
    default:           return ' ';
    }
  }
  constexpr unsigned int piece_side_table[12] = { white, white, white, white, white, white, black, black, black, black, black, black };
  constexpr unsigned int castle_permission_requirement_table[2][2] = { {white_king_side_castle, white_queen_side_castle}, {black_king_side_castle, black_queen_side_castle} };
  constexpr uint64_t castle_get_occupancy_mask_table[2][2] = { {6917529027641081856, 1008806316530991104}, {96, 14} };

  enum king_queen_side
  {
    king_side, queen_side
  };

  };
// clang-format on
