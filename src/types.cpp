#include "types.h"

unsigned int
get_rank (const unsigned int square)
{
  return square >> 3;
}
unsigned int
get_file (const unsigned int square)
{
  return square & (8 - 1);
}
unsigned int
to_rank (const char c)
{
  return 8 - (c - '0');
}
unsigned int
to_file (const char c)
{
  return tolower (c) - 'a';
}
