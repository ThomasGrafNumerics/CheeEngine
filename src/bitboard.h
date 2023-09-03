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
  // Bitboard (Bitboard &&) = delete;

  // copy assignment
  Bitboard &operator= (const Bitboard &);

  // move assignment
  Bitboard &operator= (Bitboard &&) = default;

  // default destructor
  ~Bitboard (void) = default;

  // user-defined conversion functions
  operator uint64_t & (void);
  operator uint64_t (void) const;

  // Bitboard operations
  bool get_bit (const unsigned int) const;
  void print_bitboard (void) const;
  uint64_t get_bitboard_value (void) const;
  void set_bit (const unsigned int);
  void clear_bit (const unsigned int);
  void invert_bit (const unsigned int);
  void clear_least_significant_set_bit (void);
  void clear_bitboard (void);
  void invert_bitboard (void);
  unsigned int get_number_of_set_bits (void) const;
  unsigned int get_index_of_least_significant_set_bit (void) const;

private:
  uint64_t bitboard;
};
