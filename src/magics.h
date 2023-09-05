#pragma once

#include "bitboard.h"
#include "slow_attacks.h"
#include "types.h"
#include <cstdint>

class Magics
{
public:
  Magics (void) = default;
  Magics (Magics &&) = default;
  Magics (const Magics &) = default;
  Magics &operator= (Magics &&) = default;
  Magics &operator= (const Magics &) = default;
  ~Magics (void) = default;

  Magics (const uint32_t);

  uint32_t get_and_update_xorshift32_prn (void);
  uint64_t get_and_update_special_64bit_prn (void);
  uint64_t next_magic_candidate (void);

  Bitboard rook_potential_blockers (const unsigned int) const;
  Bitboard bishop_potential_blockers (const unsigned int) const;
  Bitboard give_kth_blocker_configuration (const Bitboard, const unsigned int) const;
  uint64_t find_magic_number (const bool, const unsigned int);

  void init_rook_magic_numbers (void);
  void init_bishop_magic_numbers (void);
  void print_magic_numbers (void) const;
  void precompute_slider_pieces_attacks (void);

  uint32_t xorshift32_sequence_element{ 1804289383 };
  Slow_Attacks slow_attacks;

  // clang-format off
  static constexpr unsigned int rook_blocker_count_table[64]
      = { 12, 11, 11, 11, 11, 11, 11, 12,
          11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11,
          11, 10, 10, 10, 10, 10, 10, 11,
          12, 11, 11, 11, 11, 11, 11, 12 };

  static constexpr unsigned int bishop_blocker_count_table[64]
      = { 6, 5, 5, 5, 5, 5, 5, 6,
          5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 7, 7, 7, 7, 5, 5,
          5, 5, 7, 9, 9, 7, 5, 5,
          5, 5, 7, 9, 9, 7, 5, 5,
          5, 5, 7, 7, 7, 7, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5,
          6, 5, 5, 5, 5, 5, 5, 6 };
  // clang-format on

  uint64_t rook_magic_numbers_table[64];
  uint64_t bishop_magic_numbers_table[64];

  Bitboard rook_potential_blockers_table[64];
  Bitboard bishop_potential_blockers_table[64];
  Bitboard precomputed_rook_attacks_hash_table[64][4096];
  Bitboard precomputed_bishop_attacks_hash_table[64][512];

  constexpr static uint64_t precomputed_rook_magics[64]
      = { 0x8a80104000800020_uint64, 0x140002000100040_uint64,  0x2801880a0017001_uint64,  0x100081001000420_uint64,  0x200020010080420_uint64,
          0x3001c0002010008_uint64,  0x8480008002000100_uint64, 0x2080088004402900_uint64, 0x800098204000_uint64,     0x2024401000200040_uint64,
          0x100802000801000_uint64,  0x120800800801000_uint64,  0x208808088000400_uint64,  0x2802200800400_uint64,    0x2200800100020080_uint64,
          0x801000060821100_uint64,  0x80044006422000_uint64,   0x100808020004000_uint64,  0x12108a0010204200_uint64, 0x140848010000802_uint64,
          0x481828014002800_uint64,  0x8094004002004100_uint64, 0x4010040010010802_uint64, 0x20008806104_uint64,      0x100400080208000_uint64,
          0x2040002120081000_uint64, 0x21200680100081_uint64,   0x20100080080080_uint64,   0x2000a00200410_uint64,    0x20080800400_uint64,
          0x80088400100102_uint64,   0x80004600042881_uint64,   0x4040008040800020_uint64, 0x440003000200801_uint64,  0x4200011004500_uint64,
          0x188020010100100_uint64,  0x14800401802800_uint64,   0x2080040080800200_uint64, 0x124080204001001_uint64,  0x200046502000484_uint64,
          0x480400080088020_uint64,  0x1000422010034000_uint64, 0x30200100110040_uint64,   0x100021010009_uint64,     0x2002080100110004_uint64,
          0x202008004008002_uint64,  0x20020004010100_uint64,   0x2048440040820001_uint64, 0x101002200408200_uint64,  0x40802000401080_uint64,
          0x4008142004410100_uint64, 0x2060820c0120200_uint64,  0x1001004080100_uint64,    0x20c020080040080_uint64,  0x2935610830022400_uint64,
          0x44440041009200_uint64,   0x280001040802101_uint64,  0x2100190040002085_uint64, 0x80c0084100102001_uint64, 0x4024081001000421_uint64,
          0x20030a0244872_uint64,    0x12001008414402_uint64,   0x2006104900a0804_uint64,  0x1004081002402_uint64 };

  constexpr static uint64_t precomputed_bishop_magics[64]
      = { 0x40040844404084_uint64,   0x2004208a004208_uint64,   0x10190041080202_uint64,   0x108060845042010_uint64,  0x581104180800210_uint64,
          0x2112080446200010_uint64, 0x1080820820060210_uint64, 0x3c0808410220200_uint64,  0x4050404440404_uint64,    0x21001420088_uint64,
          0x24d0080801082102_uint64, 0x1020a0a020400_uint64,    0x40308200402_uint64,      0x4011002100800_uint64,    0x401484104104005_uint64,
          0x801010402020200_uint64,  0x400210c3880100_uint64,   0x404022024108200_uint64,  0x810018200204102_uint64,  0x4002801a02003_uint64,
          0x85040820080400_uint64,   0x810102c808880400_uint64, 0xe900410884800_uint64,    0x8002020480840102_uint64, 0x220200865090201_uint64,
          0x2010100a02021202_uint64, 0x152048408022401_uint64,  0x20080002081110_uint64,   0x4001001021004000_uint64, 0x800040400a011002_uint64,
          0xe4004081011002_uint64,   0x1c004001012080_uint64,   0x8004200962a00220_uint64, 0x8422100208500202_uint64, 0x2000402200300c08_uint64,
          0x8646020080080080_uint64, 0x80020a0200100808_uint64, 0x2010004880111000_uint64, 0x623000a080011400_uint64, 0x42008c0340209202_uint64,
          0x209188240001000_uint64,  0x400408a884001800_uint64, 0x110400a6080400_uint64,   0x1840060a44020800_uint64, 0x90080104000041_uint64,
          0x201011000808101_uint64,  0x1a2208080504f080_uint64, 0x8012020600211212_uint64, 0x500861011240000_uint64,  0x180806108200800_uint64,
          0x4000020e01040044_uint64, 0x300000261044000a_uint64, 0x802241102020002_uint64,  0x20906061210001_uint64,   0x5a84841004010310_uint64,
          0x4010801011c04_uint64,    0xa010109502200_uint64,    0x4a02012000_uint64,       0x500201010098b028_uint64, 0x8040002811040900_uint64,
          0x28000010020204_uint64,   0x6000020202d0240_uint64,  0x8918844842082200_uint64, 0x4010011029020020_uint64 };

private:
};
