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

// Precalculated bitboards for ranks and files.
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


// Arrays for keeping the rank, file and square bitboards.
extern Bitboard SquareBB[TOTAL_SQUARES];
extern Bitboard FileBB[TOTAL_FILES];
extern Bitboard RankBB[TOTAL_RANKS];

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
 * Inline functions for simple operations on bitboards,
 * for example, getting rank and file bitboards, or adding and removing bits.
 */

// Returns a specified square bitboard.
inline Bitboard getBB(Square s) {
	return SquareBB[s];
}

// Returns a specified file bitboard.
inline Bitboard getFileBB(File f) {
	return FileBB[f];
}

// Returns a specified rank bitboard.
inline Bitboard getRankBB(Rank r) {
	return RankBB[r];
}

// Returns a bitboard with a specified square bit set.
inline Bitboard addToBB(Bitboard BB, Square s) {
	return BB | getBB(s);
}

// Returns a bitboard with a specified square bit unset.
inline Bitboard removeFromBB(Bitboard BB, Square s) {
	return BB & ~getBB(s);
}

// Returns false if a bit is not set, true otherwise.
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


#endif // #ifndef BITBOARDS_H_INCLUDED



