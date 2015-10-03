/* 
 * BITBOARDS.H
 * 
 * 
 * 
 * A bitboard is just a 64 bit unsigned int,
 * one bit for each square, format as follows :
 * 
 * 
       56 57 58 59 60 61 62 63 -> msb
       48 49 50 51 52 53 54 55
       40 41 42 43 44 45 46 47
       32 33 34 35 36 37 38 39
       24 25 26 27 28 29 30 31
       16 17 18 19 20 21 22 23
       8  9  10 11 12 13 14 15
lsb -> 0  1  2  3  4  5  6  7
 * 
 *  
 * 
 * Header file for bitboards, contains some simple functions for things like adding and removing
 * bits from a bitboard, as well as prestoring bitboards for each square, rank and file.
 * 
 */


#ifndef BITBOARDS_H_INCLUDED
#define BITBOARDS_H_INCLUDED

// A bitboard is just a uint64_t, typedef in types.h
#include "types.h"


// Declare function early because it is needed to define the diagonals.
Bitboard shiftBB(Bitboard b, Square direction);
Bitboard getBB(Square s);


// Precalculated bitboards for ranks and files, dark and light squares.
const Bitboard DarkSquares = 0xAA55AA55AA55AA55ULL;
const Bitboard LightSquares = ~DarkSquares;

const Bitboard FileABB = 0x0101010101010101ULL;
const Bitboard FileBBB = FileABB << 1;
const Bitboard FileCBB = FileABB << 2;
const Bitboard FileDBB = FileABB << 3;
const Bitboard FileEBB = FileABB << 4;
const Bitboard FileFBB = FileABB << 5;
const Bitboard FileGBB = FileABB << 6;
const Bitboard FileHBB = FileABB << 7;

const Bitboard Rank1BB = 0xFF;
const Bitboard Rank2BB = Rank1BB << (8 * 1);
const Bitboard Rank3BB = Rank1BB << (8 * 2);
const Bitboard Rank4BB = Rank1BB << (8 * 3);
const Bitboard Rank5BB = Rank1BB << (8 * 4);
const Bitboard Rank6BB = Rank1BB << (8 * 5);
const Bitboard Rank7BB = Rank1BB << (8 * 6);
const Bitboard Rank8BB = Rank1BB << (8 * 7);


/* 
 * Pre calculate all the diagonals
 */
// The up right diagonals.
const Bitboard DiagA1H8 = 0x8040201008040201ULL;

const Bitboard DiagA2G8 = shiftBB(DiagA1H8, NORTH);
const Bitboard DiagA3F8 = shiftBB(DiagA2G8, NORTH);
const Bitboard DiagA4E8 = shiftBB(DiagA3F8, NORTH);
const Bitboard DiagA5D8 = shiftBB(DiagA4E8, NORTH);
const Bitboard DiagA6C8 = shiftBB(DiagA5D8, NORTH);
const Bitboard DiagA7B8 = shiftBB(DiagA6C8, NORTH);

const Bitboard DiagB1H7 = shiftBB(DiagA1H8, EAST);
const Bitboard DiagC1H6 = shiftBB(DiagB1H7, EAST);
const Bitboard DiagD1H5 = shiftBB(DiagC1H6, EAST);
const Bitboard DiagE1H4 = shiftBB(DiagD1H5, EAST);
const Bitboard DiagF1H3 = shiftBB(DiagE1H4, EAST);
const Bitboard DiagG1H2 = shiftBB(DiagF1H3, EAST);

// The down right diagonals.
const Bitboard DiagA8H1 = 0x0102040810204080ULL;

const Bitboard DiagA7G1 = shiftBB(DiagA8H1, SOUTH);
const Bitboard DiagA6F1 = shiftBB(DiagA7G1, SOUTH);
const Bitboard DiagA5E1 = shiftBB(DiagA6F1, SOUTH);
const Bitboard DiagA4D1 = shiftBB(DiagA5E1, SOUTH);
const Bitboard DiagA3C1 = shiftBB(DiagA4D1, SOUTH);
const Bitboard DiagA2B1 = shiftBB(DiagA3C1, SOUTH);

const Bitboard DiagB8H2 = shiftBB(DiagA8H1, EAST);
const Bitboard DiagC8H3 = shiftBB(DiagB8H2, EAST);
const Bitboard DiagD8H4 = shiftBB(DiagC8H3, EAST);
const Bitboard DiagE8H5 = shiftBB(DiagD8H4, EAST);
const Bitboard DiagF8H6 = shiftBB(DiagE8H5, EAST);
const Bitboard DiagG8H7 = shiftBB(DiagF8H6, EAST);


// Arrays for keeping the rank, file and square bitboards.
extern Bitboard SquareBB[TOTAL_SQUARES];
extern Bitboard FileBB[TOTAL_FILES];
extern Bitboard RankBB[TOTAL_RANKS];

extern Bitboard Lines[TOTAL_SQUARES][TOTAL_SQUARES];

extern Bitboard RookXrays[TOTAL_SQUARES];
extern Bitboard BishopXrays[TOTAL_SQUARES];



// Constant arrays for the directions each piece (excluding pawns) can move.
const Square KNIGHT_DIRECTIONS[8][2] = {
		{NORTH_WEST, WEST},
		{NORTH_WEST, NORTH},
		{NORTH_EAST, NORTH},
		{NORTH_EAST, EAST},
		{SOUTH_WEST, WEST},
		{SOUTH_WEST, SOUTH},
		{SOUTH_EAST, SOUTH},
		{SOUTH_EAST, EAST}
	};
const Square BISHOP_DIRECTIONS[4] = {
		NORTH_WEST,
		NORTH_EAST,
		SOUTH_EAST,
		SOUTH_WEST
	};
const Square ROOK_DIRECTIONS[4] = {
		NORTH,
		EAST,
		SOUTH,
		WEST
	};
const Square QUEEN_DIRECTIONS[8] = {
		NORTH_WEST,
		NORTH,
		NORTH_EAST,
		EAST,
		SOUTH_EAST,
		SOUTH,
		SOUTH_WEST,
		WEST
	};
const Square KING_DIRECTIONS[8] = {
		NORTH_WEST,
		NORTH,
		NORTH_EAST,
		EAST,
		SOUTH_EAST,
		SOUTH,
		SOUTH_WEST,
		WEST
	};

/*
 * Function declarations for bitboards.cpp
 */
void initBitboards(void);
void printBitboard(Bitboard b);



/*
 * Getting and popping the least significant bit from a bitboard,
 * this is used for getting all of the moves a type of piece can make in a position,
 * and retrieving them one by one.
 */

// Finds the nth smallest set bit and returns n (probably inneficient?).
inline Square lsb(Bitboard b) {
	if (b) {
		for (Square s = A1; s <= H8; s++) {
			if (getBB(s) & b) {
				return s;
			}
		}
	}
	return NO_SQUARE;
}

// Returns the lsb and then unsets it.
inline Square pop_lsb(Bitboard *b) {
  Square s = lsb(*b);
  (*b) &= (*b) - 1;
  return s;
}


/* 
 * Inline functions for simple operations on bitboards,
 * for example, getting rank and file bitboards, or adding and removing bits.
 */


/* 
 * Returns the bitboard for all the light squares.
 */
inline Bitboard getLightSquares() {
	return LightSquares;
}

/* 
 * Returns the bitboard for all the dark squares.
 */
inline Bitboard getDarkSquares() {
	return DarkSquares;
}

/* 
 * Returns a specified square bitboard.
 */
inline Bitboard getBB(Square s) {
	return SquareBB[s];
}

/* 
 * Returns a specified file bitboard.
 */
inline Bitboard getFileBB(File f) {
	return FileBB[f];
}

/* 
 * Returns a specified rank bitboard.
 */
inline Bitboard getRankBB(Rank r) {
	return RankBB[r];
}

/* 
 * Returns a bitboard of the line that connects 2 squares, 0 if not alligned.
 */
inline Bitboard getLine(Square s1, Square s2) {
	return Lines[s1][s2];
}

/* 
 * Returns true if 2 squares are alligned.
 */
inline bool areAlligned(Square s1, Square s2) {
	return getLine(s1, s2);
}

/* 
 * Returns true if 3 squares are alligned.
 */
inline bool areAlligned(Square s1, Square s2, Square s3) {
	return getLine(s1, s2) & getBB(s3);
}

/* 
 * Retuns the bitboard for every square a bishop could attack the given square.
 */
inline Bitboard getBishopXrays(Square s) {
	return BishopXrays[s];
}

/* 
 * Retuns the bitboard for every square a rook could attack the given square.
 */
inline Bitboard getRookXrays(Square s) {
	return RookXrays[s];
}

/* 
 * Gets the diagonal that a square is on, that goes from bottom left to top right.
 */
inline Bitboard getUpDiagonal(Square s) {
	switch (s) {
		case NO_SQUARE : return 0;
		case A7 : return DiagA7B8;
		case A6 : return DiagA6C8;
		case A5 : return DiagA5D8;
		case A4 : return DiagA4E8;
		case A3 : return DiagA3F8;
		case A2 : return DiagA2G8;
		case A1 : return DiagA1H8;
		case B1 : return DiagB1H7;
		case C1 : return DiagC1H6;
		case D1 : return DiagD1H5;
		case E1 : return DiagE1H4;
		case F1 : return DiagF1H3;
		case G1 : return DiagG1H2;
		default:
			Bitboard shifted = shiftBB(getBB(s), SOUTH_WEST);
			return getUpDiagonal( pop_lsb(&shifted) );
	}
}

/* 
 * Gets the diagonal that a square is on, that goes from top left to bottom right.
 */
inline Bitboard getDownDiagonal(Square s) {
	switch(s) {
		case NO_SQUARE : return 0;
		case A2 : return DiagA2B1;
		case A3 : return DiagA3C1;
		case A4 : return DiagA4D1;
		case A5 : return DiagA5E1;
		case A6 : return DiagA6F1;
		case A7 : return DiagA7G1;
		case A8 : return DiagA8H1;
		case B8 : return DiagB8H2;
		case C8 : return DiagC8H3;
		case D8 : return DiagD8H4;
		case E8 : return DiagE8H5;
		case F8 : return DiagF8H6;
		case G8 : return DiagG8H7;
		default:
			Bitboard shifted = shiftBB(getBB(s), NORTH_WEST);
			return getDownDiagonal( pop_lsb(&shifted) );
	}
}


/* 
 * Returns a bitboard with a specified square bit set.
 */
inline Bitboard addToBB(Bitboard BB, Square s) {
	return BB | getBB(s);
}

/* 
 * Returns a bitboard with a specified square bit unset.
 */
inline Bitboard removeFromBB(Bitboard BB, Square s) {
	return BB & ~getBB(s);
}

/* 
 * Returns false if a bit is not set, true otherwise.
 */
inline bool isBitSet(Square s, Bitboard b) {
	return getBB(s) & b;
}

/*
 * Shifts a bitboard in a direction and returns it, ensuring that 
 * bits don't go from one side of the board to another in our 8x8
 * representation. This is done by zeroing out one side before shifting
 * to the left or right. For just up or down (north or south), this is
 * not needed because bits are automatically lost if they go above
 * the 64th, or below the 1st bits.
 */
inline Bitboard shiftBB(Bitboard b, Square direction) {
	return
	direction == NORTH      ? b              << NORTH:
	direction == NORTH_EAST ? (b & ~FileHBB) << NORTH_EAST:
	direction == EAST       ? (b & ~FileHBB) << EAST:
	direction == SOUTH_EAST ? (b & ~FileHBB) >> -SOUTH_EAST:
	direction == SOUTH      ? b              >> -SOUTH:
	direction == SOUTH_WEST ? (b & ~FileABB) >> -SOUTH_WEST:
	direction == WEST       ? (b & ~FileABB) >> -WEST:
	direction == NORTH_WEST ? (b & ~FileABB) << NORTH_WEST:
	0;
}


#endif // #ifndef BITBOARDS_H_INCLUDED



