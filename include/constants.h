#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

typedef uint64_t Bitboard;

const Bitboard FILE_A = 0xFEFEFEFEFEFEFEFE; //all except file A
const Bitboard FILE_B = 0xFDFDFDFDFDFDFDFD; //all except file B
const Bitboard FILE_G = 0xBFBFBFBFBFBFBFBF; //all except file G
const Bitboard FILE_H = 0x7F7F7F7F7F7F7F7F; //all except file H

const Bitboard FILE_AB = FILE_A & FILE_B; //all except files A and B
const Bitboard FILE_GH = FILE_G & FILE_H; //all except files G and H

#endif