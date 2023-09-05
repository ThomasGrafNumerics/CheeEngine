#include "magics.h"
#include "slow_attacks.h"

Magics::Magics (const uint32_t xorshift32_seed)
    : current_xorshift32_sequence_element{ xorshift32_seed }
{
}

uint32_t
Magics::get_and_update_xorshift32_prn (void)
{
  current_xorshift32_sequence_element ^= current_xorshift32_sequence_element
                                         << 13;
  current_xorshift32_sequence_element
      ^= current_xorshift32_sequence_element >> 17;
  current_xorshift32_sequence_element ^= current_xorshift32_sequence_element
                                         << 5;

  return current_xorshift32_sequence_element;
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
Magics::operator() ()
{
  return get_and_update_special_64bit_prn ()
         & get_and_update_special_64bit_prn ()
         & get_and_update_special_64bit_prn ();
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
Magics::give_kth_blocker_configuration (const Bitboard potential_blockers,
                                        const unsigned int k) const
{
  Bitboard kth_blocker_configuration{ 0 };
  Bitboard temp{ potential_blockers };

  const unsigned int n_bits{ temp.get_number_of_set_bits () };
  for (unsigned int n = 0; n < n_bits; ++n)
    {
      const unsigned int square{
        temp.get_index_of_least_significant_set_bit ()
      };
      if ((1_uint64 << n) & k)
        {
          kth_blocker_configuration.set_bit (square);
        }
      temp.clear_least_significant_set_bit ();
    }

  return kth_blocker_configuration;
}

uint64_t
Magics::find_magic_number (const bool is_rook, const unsigned int square) const
{
  bool failed{ false };

  const unsigned int n_blocker_squares
      = (is_rook ? rook_potential_blockers_table[square]
                 : bishop_potential_blockers_table[square]);
  const unsigned int max_k = 1_uint64 << n_blocker_squares;

  Bitboard *blocker_configurations = new Bitboard[max_k];
  Bitboard *correct_attacks = new Bitboard[max_k];
  Bitboard *hashed_attacks = new Bitboard[max_k];

  Bitboard potential_blockers = (is_rook ? rook_potential_blockers (square)
                                         : bishop_potential_blockers (square));

  for (unsigned int k = 0; k < max_k; ++k)
    {
      blocker_configurations[k]
          = give_kth_blocker_configuration (potential_blockers, k);

      if (is_rook)
        {
          correct_attacks[k] = slow_attacks.slow_rook_attacks (
              blocker_configurations[k], square);
        }
      else
        {
          correct_attacks[k] = slow_attacks.slow_bishop_attacks (
              blocker_configurations[k], square);
        }
    }

  while (true)
    {
      uint64_t magic_number_candidate = this.();

      if (Bitboard ((reduced_attack_mask * magic_number_candidate)
                    & 0xff00000000000000)
              .get_number_of_set_bits ()
          < 6)
        {
          continue;
        }
      //
      //   for (unsigned int k = 0;
      //        k < max_number_of_possible_blocker_configurations; ++k)
      //     {
      //       unsigned int magic_index = static_cast<unsigned int> (
      //           (occupancies[k] * magic_number_candidate)
      //           >> (64 - number_of_relevant_squares));
      //       if (used_attacks[magic_index] == 0)
      //         {
      //           used_attacks[magic_index] = attacks[k];
      //         }
      //       else if (used_attacks[magic_index] != attacks[k])
      //         {
      //           failed = true;
      //           break;
      //         }
      //     }
      //   if (!failed)
      //     {
      //       delete[] occupancies;
      //       delete[] attacks;
      //       delete[] used_attacks;
      //       return magic_number_candidate;
      //     }
      // }
      //
      return 11111;
    }
