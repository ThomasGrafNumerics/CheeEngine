#include "bitboard.h"
#include "types.h"
#include <iostream>

int
main ()
{
  Bitboard a{ not_a_file_mask };
  a.print_bitboard ();
  std::cout << a.get_number_of_set_bits () << std::endl;

  return 0;
}
