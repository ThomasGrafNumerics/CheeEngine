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
