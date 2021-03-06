/* 
 * BITBOARDS.CPP
 * 
 * Contains the initialisation function, for prestoring values. Also some printing functions
 * which are useful for debugging.
 * 
 * 
 */

#include <stdio.h>
#include <string>

#include "types.h"
#include "bitboards.h"

using namespace std;


// Arrays for keeping the rank, file and square bitboards.
Bitboard SquareBB[TOTAL_SQUARES];
Bitboard FileBB[TOTAL_FILES];
Bitboard RankBB[TOTAL_RANKS];

Bitboard Lines[TOTAL_SQUARES][TOTAL_SQUARES];

Bitboard RookXrays[TOTAL_SQUARES];
Bitboard BishopXrays[TOTAL_SQUARES];


/*
 * Initialise all the bitboards that are kept in memory, these could all be calculated when neeeded during runtime
 * but calculating as much as we can and storing it as the start helps speed up later evaluation.
 */
void initBitboards(void) {
	
	// Fill the file and rank bitboard arrays (this could also be done in a loop).
	FileBB[FILE_A] = FileABB;
	FileBB[FILE_B] = FileBBB;
	FileBB[FILE_C] = FileCBB;
	FileBB[FILE_D] = FileDBB;
	FileBB[FILE_E] = FileEBB;
	FileBB[FILE_F] = FileFBB;
	FileBB[FILE_G] = FileGBB;
	FileBB[FILE_H] = FileHBB;
	
	
	RankBB[RANK_1] = Rank1BB;
	RankBB[RANK_2] = Rank2BB;
	RankBB[RANK_3] = Rank3BB;
	RankBB[RANK_4] = Rank4BB;
	RankBB[RANK_5] = Rank5BB;
	RankBB[RANK_6] = Rank6BB;
	RankBB[RANK_7] = Rank7BB;
	RankBB[RANK_8] = Rank8BB;
	
	
	// Fill the square bitboard array.
	for (Square s = A1; s <= H8; s++) {
		
		SquareBB[s] = (one << s);
		
	}
	
	// Fill the array of xray attacks to each square.
	for (Square s = A1; s <= H8; s++) {
		
		BishopXrays[s] = (getUpDiagonal(s) | getDownDiagonal(s)) & ~getBB(s);
		RookXrays[s] = (getFileBB(getFile(s)) | getRankBB(getRank(s))) & ~getBB(s);
		
		// Fill the 2d array of the lines that connect each pair of squares.
		for (Square s2 = A1; s2 <= H8; s2++) {
			
			if (s == s2) {
				
				Lines[s][s2] = 0;
				
			} else if (getRank(s) == getRank(s2)) {
				
				Lines[s][s2] = getRankBB(getRank(s));
				
			} else if (getFile(s) == getFile(s2)) {
				
				Lines[s][s2] = getFileBB(getFile(s));
				
			} else if (getUpDiagonal(s) == getUpDiagonal(s2)) {
				
				Lines[s][s2] = getUpDiagonal(s);
				
			} else if (getDownDiagonal(s) == getDownDiagonal(s2)) {
				
				Lines[s][s2] = getDownDiagonal(s);
				
			} else {
				
				Lines[s][s2] = 0;
				
			}
			
		}
	}
	
	
}


/*
 * Prints out a bitboard, with X to indicate a set bit.
 */
void printBitboard(Bitboard b) {
	
	string line = "+---+---+---+---+---+---+---+---+";
	
	printf("%s\n", line.c_str());
	
	Square s;
	for (Rank r = RANK_8; r >= RANK_1; r--) {
		
		printf("|");
		
		for (File f = FILE_A; f <= FILE_H; f++) {
			
			s = getSquare(f, r);
			
			printf(" %c |", isBitSet(s, b) ? 'X' : ' '); // If a bit is set print an X, otherwise a space.
			
		}
		
		printf("\n");
		printf("%s\n", line.c_str());
		
		
	}
	
}







