#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <string>

#include <cstdint>

// user defined literal for std::uint64_t type
constexpr std::uint64_t operator"" _uint64 (unsigned long long arg)
{
  return static_cast<std::uint64_t> (arg);
}

// some constants
constexpr uint64_t not_a_file_mask{ 18374403900871474942_uint64 };
constexpr uint64_t not_h_file_mask{ 9187201950435737471_uint64 };
constexpr uint64_t not_ab_file_mask{ 18229723555195321596_uint64 };
constexpr uint64_t not_gh_file_mask{ 4557430888798830399_uint64 };

// some mnemonics
constexpr std::array<const char *, 12> unicode_pieces
    = { "♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔" };

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

  };
// clang-format on
