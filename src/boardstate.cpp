#include "boardstate.h"
#include "types.h"
#include <cstdint>
#include <random>

Boardstate::Boardstate () : bitboards{}, occupancies{}, side_to_move{ Chess::white }, castle{ 0 }, enpassant_square{ Chess::forbidden_square }, fifty_moves{ 0 }, position_key{ 0 }
{
}

void
Boardstate::clear_boardstate (void)
{
  std::fill (std::begin (bitboards), std::end (bitboards), 0);
  std::fill (std::begin (occupancies), std::end (occupancies), 0);
  side_to_move = Chess::white;
  castle = 0;
  enpassant_square = Chess::forbidden_square;
  fifty_moves = 0;
  position_key = 0;
}

// static members
uint64_t Boardstate::piece_keys[12][64] = {};
uint64_t Boardstate::enpassant_keys[64] = {};
uint64_t Boardstate::castle_keys[16] = {};
uint64_t Boardstate::side_to_move_key = {};

void
Boardstate::hash_piece (const unsigned int piece, const unsigned int square)
{
  position_key ^= piece_keys[piece][square];
}
void
Boardstate::hash_enpassant (const unsigned int square)
{
  position_key ^= enpassant_keys[square];
}
void
Boardstate::hash_castle (const unsigned int castle)
{
  position_key ^= castle_keys[castle];
}
void
Boardstate::hash_side_to_move (void)
{
  position_key ^= side_to_move_key;
}

void
Boardstate::generate_new_key (void)
{
  position_key = 0;

  for (unsigned int piece = Chess::white_pawn; piece <= Chess::black_king; ++piece)
    {
      for (Bitboard cpy_bitboard = bitboards[piece]; cpy_bitboard; cpy_bitboard.clear_least_significant_set_bit ())
        {
          unsigned int square = cpy_bitboard.get_index_of_least_significant_set_bit ();
          position_key ^= piece_keys[piece][square];
        }
    }

  if (enpassant_square != Chess::forbidden_square)
    position_key ^= enpassant_keys[enpassant_square];

  position_key ^= castle_keys[castle];

  if (side_to_move == Chess::black)
    position_key ^= side_to_move_key;
}

void
Boardstate::init_zobrist_keys ()
{
  std::random_device dev;
  std::mt19937_64 rng (dev ());
  std::uniform_int_distribution<uint64_t> distribution;

  for (unsigned int piece = Chess::white_pawn; piece <= Chess::black_king; ++piece)
    {
      for (unsigned int square = 0; square < 64; ++square)
        {
          Boardstate::piece_keys[piece][square] = distribution (rng);
        }
    }

  for (unsigned int square = 0; square < 64; ++square)
    {
      Boardstate::enpassant_keys[square] = distribution (rng);
    }

  for (unsigned int i = 0; i < 16; ++i)
    {
      Boardstate::castle_keys[i] = distribution (rng);
    }

  Boardstate::side_to_move_key = distribution (rng);
}
