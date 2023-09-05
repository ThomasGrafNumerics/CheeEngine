#include "magics.h"
#include "bitboard.h"
#include "slow_attacks.h"

Magics::Magics (const uint32_t xorshift32_seed) : xorshift32_sequence_element{ xorshift32_seed } {}

uint32_t
Magics::get_and_update_xorshift32_prn (void)
{
  xorshift32_sequence_element ^= xorshift32_sequence_element << 13;
  xorshift32_sequence_element ^= xorshift32_sequence_element >> 17;
  xorshift32_sequence_element ^= xorshift32_sequence_element << 5;

  return xorshift32_sequence_element;
}

uint64_t
Magics::get_and_update_special_64bit_prn (void)
{
  uint64_t n0, n1, n2, n3;
  n0 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n1 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n2 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;
  n3 = static_cast<uint64_t> (get_and_update_xorshift32_prn ()) & 0xFFFF;

  return n0 | (n1 << 16) | (n2 << 32) | (n3 << 48);
}

uint64_t
Magics::next_magic_candidate (void)
{
  return get_and_update_special_64bit_prn () & get_and_update_special_64bit_prn () & get_and_update_special_64bit_prn ();
}

Bitboard
Magics::rook_potential_blockers (const unsigned int square) const
{
  Bitboard potential_blockers{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  for (int r = rank + 1, f = file; r <= 6; ++r)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank - 1, f = file; r >= 1; --r)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank, f = file + 1; f <= 6; ++f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank, f = file - 1; f >= 1; --f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }

  return potential_blockers;
}

Bitboard
Magics::bishop_potential_blockers (const unsigned int square) const
{
  Bitboard potential_blockers{ 0 };

  const int rank = square / 8;
  const int file = square % 8;

  for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; ++r, ++f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; ++r, --f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; --r, ++f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }
  for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; --r, --f)
    {
      potential_blockers.set_bit (r * 8 + f);
    }

  return potential_blockers;
}

Bitboard
Magics::give_kth_blocker_configuration (const Bitboard potential_blockers, const unsigned int k) const
{
  Bitboard kth_blocker_configuration{ 0 };
  Bitboard temp{ potential_blockers };

  const unsigned int n_bits{ temp.get_number_of_set_bits () };
  for (unsigned int n = 0; n < n_bits; ++n)
    {
      const unsigned int square = temp.get_index_of_least_significant_set_bit ();
      if ((1_uint64 << n) & k)
        {
          kth_blocker_configuration.set_bit (square);
        }
      temp.clear_least_significant_set_bit ();
    }

  return kth_blocker_configuration;
}

uint64_t
Magics::find_magic_number (const bool is_rook, const unsigned int square)
{
  const unsigned int n_blocker_squares = (is_rook ? rook_blocker_count_table[square] : bishop_blocker_count_table[square]);
  const unsigned int max_k = 1_uint64 << n_blocker_squares;

  Bitboard *blocker_configurations = new Bitboard[max_k];
  Bitboard *correct_attacks = new Bitboard[max_k];
  Bitboard *unique_attacks = new Bitboard[max_k];

  Bitboard potential_blockers = (is_rook ? rook_potential_blockers (square) : bishop_potential_blockers (square));

  for (unsigned int k = 0; k < max_k; ++k)
    {
      blocker_configurations[k] = give_kth_blocker_configuration (potential_blockers, k);

      if (is_rook)
        {
          correct_attacks[k] = slow_attacks.slow_rook_attacks (blocker_configurations[k], square);
        }
      else
        {
          correct_attacks[k] = slow_attacks.slow_bishop_attacks (blocker_configurations[k], square);
        }
    }

  while (true)
    {
      uint64_t magic_number_candidate = next_magic_candidate ();
      if (static_cast<Bitboard> ((potential_blockers * magic_number_candidate) & 0xff00000000000000).get_number_of_set_bits () < 6)
        {
          continue;
        }

      bool failed = false;
      std::fill (unique_attacks, unique_attacks + max_k, 0);

      for (unsigned int k = 0; k < max_k; ++k)
        {
          const unsigned int magic_index
              = static_cast<unsigned int> ((blocker_configurations[k] * magic_number_candidate) >> (64 - n_blocker_squares));
          if (unique_attacks[magic_index] == 0)
            {
              unique_attacks[magic_index] = correct_attacks[k];
            }
          else if (unique_attacks[magic_index] != correct_attacks[k])
            {
              failed = true;
              break;
            }
        }
      if (!failed)
        {
          delete[] blocker_configurations;
          delete[] correct_attacks;
          delete[] unique_attacks;
          return magic_number_candidate;
        }
    }
}

void
Magics::init_rook_magic_numbers (void)
{
  for (unsigned int square = 0; square < 64; ++square)
    {
      rook_magic_numbers_table[square] = find_magic_number (true, square);
    }
}

void
Magics::init_bishop_magic_numbers (void)
{
  for (unsigned int square = 0; square < 64; ++square)
    {
      bishop_magic_numbers_table[square] = find_magic_number (false, square);
    }
}

void
Magics::print_magic_numbers (void) const
{
  std::cout << "rook magic numbers:\n";
  for (unsigned int square = 0; square < 64; ++square)
    {
      std::cout << "0x" << std::hex << rook_magic_numbers_table[square] << "_uint64,\n";
    }

  std::cout << std::endl << "bishop magic numbers:\n";

  for (unsigned int square = 0; square < 64; ++square)
    {
      std::cout << "0x" << std::hex << bishop_magic_numbers_table[square] << "_uint64,\n";
    }
}

void
Magics::precompute_slider_pieces_attacks (void)
{
  for (unsigned int square = 0; square < 64; ++square)
    {
      rook_potential_blockers_table[square] = rook_potential_blockers (square);
      unsigned int max_k = 1 << rook_blocker_count_table[square];
      for (unsigned int k = 0; k < max_k; ++k)
        {
          Bitboard kth_blocker = give_kth_blocker_configuration (rook_potential_blockers_table[square], k);
          unsigned int magic_index
              = static_cast<unsigned int> ((kth_blocker * precomputed_rook_magics[square]) >> (64 - rook_blocker_count_table[square]));
          precomputed_rook_attacks_hash_table[square][magic_index] = slow_attacks.slow_rook_attacks (kth_blocker, square);
        }

      bishop_potential_blockers_table[square] = bishop_potential_blockers (square);
      max_k = 1 << bishop_blocker_count_table[square];
      for (unsigned int k = 0; k < max_k; ++k)
        {
          Bitboard kth_blocker = give_kth_blocker_configuration (bishop_potential_blockers_table[square], k);
          unsigned int magic_index
              = static_cast<unsigned int> ((kth_blocker * precomputed_bishop_magics[square]) >> (64 - bishop_blocker_count_table[square]));
          precomputed_bishop_attacks_hash_table[square][magic_index] = slow_attacks.slow_bishop_attacks (kth_blocker, square);
        }
    }
}
