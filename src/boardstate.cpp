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

  // WK | WQ | BK | BQ == 16
  for (unsigned int i = 0; i < 16; ++i)
    {
      Boardstate::castle_keys[i] = distribution (rng);
    }

  Boardstate::side_to_move_key = distribution (rng);
}
