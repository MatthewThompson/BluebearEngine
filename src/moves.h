/* 
 * 
 * MOVES.H
 * 
 * Header file for moves, contains some simple functions for
 * returning the type and to and from squares of a move.
 * 
 */


#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>

#include "evaluate.h"
#include "position.h"
#include "types.h"


/* 
 * Funciton prototypes for moves.cpp
 */

vector<Move> getPawnMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getKnightMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getBishopMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getRookMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getQueenMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getKingMoves(Position& pos, Colour us, vector<Move> moveList, MoveGenType type);
vector<Move> getMoves(Position& pos, Colour us, MoveGenType type);
vector<Move> getLegalMoves(Position& pos);
vector<Move> getLegalMoves(Position& pos, MoveGenType type);
vector<Move> getLegalMoves(Position& pos, Colour toMove);
vector<Move> getLegalMoves(Position& pos, Colour toMove, MoveGenType type);

string getMoveStr(Position& pos, Move m, string flags = "");
void printMoves(Position& pos, vector<Move> moveList);
void printMovesWithScores(Position& pos, vector<Move> moveList);
//void printMovesFromNodes(Position& pos, vector<MoveNode> moveList);

/* 
 * Simple inline functions.
 */

// Given a from, to squre, move type and promotion piece (if applicable), returns a move.
inline Move getMove(Square from, Square to, MoveType mt, PieceType pt = KNIGHT) {
	return mt | (pt - KNIGHT) << 12 | from << 6 | to;
}

// Gets the direction the king moves when castling on a given side.
inline Square getDirection(CastlingSide side) {
	return side == KING_SIDE ? EAST : WEST;
}

// Returns a move for castling, given a colour and side.
inline Move getCastling(Colour c, CastlingSide side) {
	Square king = c == WHITE ? E1 : E8;
	return getMove(king, king + 2 * getDirection(side), CASTLING);
}

// Gets the from square of a move.
inline Square getFrom(Move m) {
	return Square((m >> 6) & 63);
}

// Gets the to square of a move.
inline Square getTo(Move m) {
	return Square(m & 63);
}

// Gets the type of promotion piece of a move.
inline PieceType getPromotionPiece(Move m) {
	return PieceType(((m >> 12) & 3) + 2);
}

// Gets the type of a move.
inline MoveType getMoveType(Move m) {
	return MoveType(m & (3 << 14));
}

// Returns the forward direction.
inline Square forward(Colour c) {
	return c == WHITE ? NORTH : SOUTH;
}








#endif // #ifndef MOVES_H_INCLUDED
