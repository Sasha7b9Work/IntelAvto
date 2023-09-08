// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define WIN32_LEAN_AND_MEAN

// ����� �����������������, ����� ��� �������� ������ "1000�"
//#define USE_1000s

// ����� �����������������, ����� ��� �������� ������ "1000K"
//#define USE_1000K

typedef unsigned short         uint16;
typedef signed short           int16;
typedef unsigned char          uint8;
typedef unsigned char          uchar;
typedef signed char            int8;
typedef signed long long int   int64;
typedef unsigned long long int uint64;
typedef unsigned int           uint;
typedef const char * const     pString;
typedef const char *           pchar;

typedef void(*pFuncVII)(int, int);


#ifndef WIN32
#define nullptr 0 //-V1059
#endif

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U16(bits1, bits0) (BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define _SET_BIT_VALUE(value, numBit, bitValue) ((value) |= ((bitValue) << (numBit)))
#define _GET_BIT(value, numBit) (((value) >> (numBit)) & 1)
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))


#define STR_NUM_VERSION "Cherem V1.2"
#define MODEL_RU        "���������� ����������-������� �3-96/2"
#define MODEL_EN        "Electronic counting frequency meter Ch3-96/2"
#define NAME_MODEL      (LANG_IS_RU ? MODEL_RU : MODEL_EN)
//#define STR_CRC32       "A87CDBF5"
