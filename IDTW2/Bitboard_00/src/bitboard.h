#pragma once

#include "types.h"
#include <cstdint>

class Bitboard
{
public:
  // "ordinary constructor": create an object
  Bitboard (const uint64_t);

  // default constructor
  Bitboard (void);

  // copy constructor
  Bitboard (const Bitboard &);

  //  move constructor
  Bitboard (Bitboard &&) = default;

  // copy assignment
  Bitboard &operator= (const Bitboard &);

  // move assignment
  Bitboard &operator= (Bitboard &&) = default;

  // default destructor
  ~Bitboard (void) = default;

  // user-defined conversion functions
  operator uint64_t & (void);
  operator uint64_t (void) const;

  // *** bitboard operations IDTW2 *** //
  // get_bit

  // print_bitboard

  // get_bitboard_value

  // set_bit

  // clear_bit

  // invert_bit

  // clear_least_significant_set_bit

  // clear_bitboard

  // invert_bitboard

  // get_number_of_set_bits

  // get_index_of_least_significant_set_bit

private:
  uint64_t bitboard;
};
