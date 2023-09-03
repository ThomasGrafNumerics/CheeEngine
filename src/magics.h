#pragma once

#include "bitboard.h"
#include "slow_attacks.h"
#include "types.h"

class Magics
{
public:
  Magics (void) = default;
  Magics (Magics &&) = default;
  Magics (const Magics &) = default;
  Magics &operator= (Magics &&) = default;
  Magics &operator= (const Magics &) = default;
  ~Magics ();

  Magics (const uint32_t);

  uint32_t get_and_update_xorshift32_prn (void);
  uint64_t get_and_update_special_64bit_prn (void);
  uint64_t operator() ();

private:
  uint32_t current_xorshift32_sequence_element;
};
