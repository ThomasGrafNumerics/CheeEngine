#include "bitboard.h"
#include "types.h"

Bitboard::Bitboard (void) : bitboard{ 0_uint64 } {}
Bitboard::Bitboard (const uint64_t a) : bitboard{ a } {}
Bitboard::Bitboard (const Bitboard &a) : bitboard{ a.bitboard } {}
Bitboard &
Bitboard::operator= (const Bitboard &a)
{
  if (this == &a)
    {
      return *this;
    }
  bitboard = a.bitboard;
  return *this;
}

Bitboard::operator uint64_t & (void) { return bitboard; }
Bitboard::operator uint64_t (void) const { return bitboard; }

bool
Bitboard::get_bit (const unsigned int square) const
{
  return (bitboard) & (1_uint64 << square);
}

void
Bitboard::print_bitboard (void) const
{
  std::cout << "\n\n";
  for (unsigned int rank = 0; rank < 8; ++rank)
    {
      std::cout << (8 - rank) << "|  ";
      for (unsigned int file = 0; file < 8; ++file)
        {
          unsigned int square = rank * 8 + file;
          std::cout << get_bit (square) << "  ";
        }
      std::cout << "\n";
    }
  std::cout << "    _  _  _  _  _  _  _  _"
            << "\n";
  std::cout << "    a  b  c  d  e  f  g  h"
            << "\n\n";
  std::cout << "binary: " << std::bitset<64> (bitboard) << "\n";
  std::cout << "decimal: " << bitboard << "\n\n\n";
}

uint64_t
Bitboard::get_bitboard_value (void) const
{
  return bitboard;
}

void
Bitboard::set_bit (const unsigned int square)
{
  bitboard |= (1_uint64 << square);
}

void
Bitboard::clear_bit (const unsigned int square)
{
  bitboard &= (~(1_uint64 << square));
}

void
Bitboard::invert_bit (const unsigned int square)
{
  bitboard ^= (1_uint64 << square);
}

void
Bitboard::clear_least_significant_set_bit (void)
{
  bitboard &= (bitboard - 1_uint64);
}

void
Bitboard::clear_bitboard (void)
{
  bitboard = 0_uint64;
}

void
Bitboard::invert_bitboard (void)
{
  bitboard = ~bitboard;
}

unsigned int
Bitboard::get_number_of_set_bits (void) const
{
  // using Brian Kernighan’s Algorithm:
  // https://www.geeksforgeeks.org/count-set-bits-in-an-integer/
  uint64_t temp{ bitboard };
  unsigned int count{ 0 };
  while (temp)
    {
      temp &= (temp - 1_uint64);
      ++count;
    }
  return count;
}

unsigned int
Bitboard::get_index_of_least_significant_set_bit (void) const
{
  assert (bitboard != 0);
  Bitboard temp{ (bitboard & -bitboard) - 1_uint64 };
  return temp.get_number_of_set_bits ();
}
