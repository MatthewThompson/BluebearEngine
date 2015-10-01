/* 
 * MOVES.CPP
 * 
 * Contains move functions, checking if they are legal, and finding
 * all moves in the current position.
 * 
 * 
 */
#include <stdio.h> // Useful for debugging.
#include <stdlib.h> // for abs()

#include "moves.h"

#include "evaluate.h"
#include "bitboards.h"
#include "evaluate.h"
#include "types.h"
#include "position.h"

using namespace std;


/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves pawns can make to the list.
 */
vector<Move> getPawnMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	// Get all relevant bitboards and directions depending what side we are.
	Bitboard pawns        = us == WHITE ? pos.getPieces(WHITE, PAWN) : pos.getPieces(BLACK, PAWN);
	Bitboard rank7        = us == WHITE ? getRankBB(RANK_7) : getRankBB(RANK_2);
	Bitboard rank3        = us == WHITE ? getRankBB(RANK_3) : getRankBB(RANK_6);
	Bitboard enemies      = us == WHITE ? pos.getPieces(BLACK) : pos.getPieces(WHITE);
	Bitboard emptySquares = ~pos.getPieces();
	Square forward        = us == WHITE ? NORTH : SOUTH;
	Square forwardLeft    = us == WHITE ? NORTH_WEST : SOUTH_EAST;
	Square forwardRight   = us == WHITE ? NORTH_EAST : SOUTH_WEST;
	Bitboard pawnsBelow7  = pawns & ~rank7;
	Bitboard pawnsOn7     = pawns & rank7;
	
	// Shift all pawns below the 7th rank forward once and check it's on an empty square.
	Bitboard pawnSingleMoves = shiftBB(pawnsBelow7, forward) & emptySquares;
	// Get all the pawns that just moved that are now on the relative 3rd rank, try move them again.
	Bitboard pawnDoubleMoves = shiftBB(pawnSingleMoves & rank3, forward) & emptySquares;
	
	string movename;
	Square from, to;
	
	while (pawnSingleMoves) { // Keep removing each set bit from these moves until none left.
		to = pop_lsb(&pawnSingleMoves);
		from = to - forward;
		
		// Add each one to the move list.
		moveList.push_back(getMove(from, to, NORMAL));
		
	}
	
	// Add all the pawn double pushes.
	while (pawnDoubleMoves) {
		to = pop_lsb(&pawnDoubleMoves);
		from = to - forward - forward;
		
		moveList.push_back(getMove(from, to, NORMAL));
		
	}
	
	// Get both the captures (non ep) left and right separately by shifting forward left and right, and checking we are on an enemy.
	Bitboard pawnCapsLeft = shiftBB(pawnsBelow7, forwardLeft) & enemies;
	Bitboard pawnCapsRight = shiftBB(pawnsBelow7, forwardRight) & enemies;
	
	while(pawnCapsLeft) {
		to = pop_lsb(&pawnCapsLeft);
		from = to - forwardLeft;
		
		moveList.push_back(getMove(from, to, NORMAL));
		
	}
	
	while(pawnCapsRight) {
		to = pop_lsb(&pawnCapsRight);
		from = to - forwardRight;
		
		moveList.push_back(getMove(from, to, NORMAL));
		
	}
	
	//
	Square epTargetSquare = pos.getEPTarget();
	
	if (epTargetSquare != NO_SQUARE) {
		
		Bitboard epTarget = getBB(pos.getEPTarget());
		
		// Get both left and right ep captures, by shifting and checking we are on an ep target square.
		Bitboard pawnEPCapsLeft = shiftBB(pawnsBelow7, forwardLeft) & epTarget;
		Bitboard pawnEPCapsRight = shiftBB(pawnsBelow7, forwardRight) & epTarget;
		Square captured;
		
		while(pawnEPCapsLeft) {
			to = pop_lsb(&pawnEPCapsLeft);
			from = to - forwardLeft;
			
			moveList.push_back(getMove(from, to, ENPASSANT));
			
		}
		
		while(pawnEPCapsRight) {
			to = pop_lsb(&pawnEPCapsRight);
			from = to - forwardRight;
			
			moveList.push_back(getMove(from, to, ENPASSANT));
			
		}
		
	}
	
	// Shift all of the pawns on the relative 7th rank up and check they're on an empty square.
	Bitboard pawnPromos = shiftBB(pawnsOn7, forward) & emptySquares;
	
	while(pawnPromos) {
		to = pop_lsb(&pawnPromos);
		from = to - forward;
		
		// Add 4 moves, one of each promotion type.
		for (PieceType promotionType = KNIGHT; promotionType <= QUEEN; promotionType++) {
			
			moveList.push_back(getMove(from, to, PROMOTION, promotionType));
			
		}
	}
	
	
	// Do the same as captures before, but with pawns on the 7th.
	Bitboard pawnPromoCapsLeft = shiftBB(pawnsOn7, forwardLeft) & enemies;
	Bitboard pawnPromoCapsRight = shiftBB(pawnsOn7, forwardRight) & enemies;
	
	while (pawnPromoCapsLeft) {
		to = pop_lsb(&pawnPromoCapsLeft);
		from = to - forwardLeft;
		
		for (PieceType promotionType = KNIGHT; promotionType <= QUEEN; promotionType++) {
			
			moveList.push_back(getMove(from, to, PROMOTION, promotionType));
			
		}
	}
	
	while (pawnPromoCapsRight) {
		to = pop_lsb(&pawnPromoCapsRight);
		from = to - forwardRight;
		
		for (PieceType promotionType = KNIGHT; promotionType <= QUEEN; promotionType++) {
			
			moveList.push_back(getMove(from, to, PROMOTION, promotionType));
			
		}
	}
	
	return moveList;
	
}


/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves knights can make to the list.
 */
vector<Move> getKnightMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	Bitboard knights      = us == WHITE ? pos.getPieces(WHITE, KNIGHT) : pos.getPieces(BLACK, KNIGHT);
	Bitboard enemies      = us == WHITE ? pos.getPieces(BLACK) : pos.getPieces(WHITE);
	Bitboard emptySquares = ~pos.getPieces();
	
	Square to, from;
	string movename;
	
	// Get all the knight non capture moves and capture moves separately.
	Bitboard knightJumps, knightCaps, tempMove;
	for(int i = 0; i < 8; i++) { // For all 8 knight directions.
	
		// A bitboard of all knights moved in one of the directions.
		tempMove = shiftBB(shiftBB(knights, KNIGHT_DIRECTIONS[i][0]), KNIGHT_DIRECTIONS[i][1]);
		knightJumps = tempMove & emptySquares;
		knightCaps = tempMove & enemies;
		
		while (knightJumps) {
			to = pop_lsb(&knightJumps);
			from = to - (KNIGHT_DIRECTIONS[i][0] + KNIGHT_DIRECTIONS[i][1]);
			
			moveList.push_back(getMove(from, to, NORMAL));
			
		}
		
		while (knightCaps) {
			to = pop_lsb(&knightCaps);
			from = to - (KNIGHT_DIRECTIONS[i][0] + KNIGHT_DIRECTIONS[i][1]);
			
			moveList.push_back(getMove(from, to, NORMAL));
			
		}
	}
	
	
	return moveList;
	
}


/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves bishops can make to the list.
 */
vector<Move> getBishopMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	Bitboard bishops      = us == WHITE ? pos.getPieces(WHITE, BISHOP) : pos.getPieces(BLACK, BISHOP);
	Bitboard enemies      = us == WHITE ? pos.getPieces(BLACK) : pos.getPieces(WHITE);
	Bitboard emptySquares = ~pos.getPieces();
	
	Square direction;
	
	Bitboard bishopMoves = bishops;
	Bitboard bishopCaps;
	Bitboard temp;
	
	Square to, from;
	string movename;
	int moves;
	
	for (int i = 0; i < 4; i++) {
		
		direction = BISHOP_DIRECTIONS[i];
		moves = 0;
		bishopMoves = bishops;
		
		while (bishopMoves) { // While we still have bishops that haven't met another piece.
			
			// Move all the bishops in this direction.
			bishopMoves = shiftBB(bishopMoves, direction);
			bishopCaps = bishopMoves & enemies;
			bishopMoves = bishopMoves & emptySquares; // Bishops that hit a piece of any colour are lost.
			moves++; // Keep track of how many times we've moved.
			
			temp = bishopMoves;
			while (temp) { // Use a temporary variable so we don't lose where the bishops are.
				to = pop_lsb(&temp);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
			
			while (bishopCaps) {
				to = pop_lsb(&bishopCaps);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
		}
	
	}
	
	return moveList;
	
}


/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves rooks can make to the list.
 */
vector<Move> getRookMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	Bitboard rooks        = us == WHITE ? pos.getPieces(WHITE, ROOK) : pos.getPieces(BLACK, ROOK);
	Bitboard enemies      = us == WHITE ? pos.getPieces(BLACK) : pos.getPieces(WHITE);
	Bitboard emptySquares = ~pos.getPieces();
	
	Square direction;
	
	Bitboard rookMoves = rooks;
	Bitboard rookCaps;
	Bitboard temp;
	
	Square to, from;
	int moves;
	
	for (int i = 0; i < 4; i++) {
		
		direction = ROOK_DIRECTIONS[i];
		moves = 0;
		rookMoves = rooks;
		
		
		while (rookMoves) { // While we still have rooks that haven't met another piece.
		
			// Move all the rooks in this direction.
			rookMoves = shiftBB(rookMoves, direction);
			rookCaps = rookMoves & enemies;
			rookMoves = rookMoves & emptySquares; // Rooks that hit a piece of any colour are lost.
			moves++; // Keep track of how many times we've moved.
			
			temp = rookMoves;
			while (temp) { // Use a temporary variable so we don't lose where the rooks are.
				to = pop_lsb(&temp);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
			
			while (rookCaps) {
				to = pop_lsb(&rookCaps);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
		}
	
	}
	
	return moveList;
	
}


/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves queens can make to the list.
 */
vector<Move> getQueenMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	Bitboard queens       = us == WHITE ? pos.getPieces(WHITE, QUEEN) : pos.getPieces(BLACK, QUEEN);
	Bitboard enemies      = us == WHITE ? pos.getPieces(BLACK) : pos.getPieces(WHITE);
	Bitboard emptySquares = ~pos.getPieces();
	
	Square direction;
	
	Bitboard queenMoves;
	Bitboard queenCaps;
	Bitboard temp;
	
	Square to, from;
	string movename;
	int moves;
	
	for (int i = 0; i < 8; i++) {
		
		direction = QUEEN_DIRECTIONS[i];
		moves = 0;
		queenMoves = queens;
		
		while (queenMoves) { // While we still have queens that haven't met another piece.
			
			// Move all the queens in this direction.
			queenMoves = shiftBB(queenMoves, direction);
			queenCaps = queenMoves & enemies;
			queenMoves = queenMoves & emptySquares; // Queens that hit a piece of any colour are lost.
			moves++; // Keep track of how many times we've moved.
			
			temp = queenMoves;
			while (temp) { // Use a temporary variable so we don't lose where the queens are.
				to = pop_lsb(&temp);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
			
			while (queenCaps) {
				to = pop_lsb(&queenCaps);
				from = to - (direction * moves);
				
				moveList.push_back(getMove(from, to, NORMAL));
				
			}
		}
	
	}
	
	return moveList;
	
}

/* 
 * Takes a list of moves, and a colour, and adds all
 * the moves kings can make (including castling) to the list.
 */
vector<Move> getKingMoves(Position& pos, Colour us, vector<Move> moveList) {
	
	Bitboard king = pos.getPieces(us, KING);
	Bitboard kingMove;
	Square from = pos.getKingSquare(us);
	
	for (int i = 0; i < 8; i++) {
		
		kingMove = shiftBB(king, KING_DIRECTIONS[i]);
		
		if (kingMove && !(kingMove & pos.getPieces(us))) { // If the king is still on the board and didn't land on one of his own pieces.
			
			moveList.push_back(getMove(from, pop_lsb(&kingMove), NORMAL));
			
		}
		
	}
	
	// Check if we can castle each way (has the right do to so, path not blocked and king path not attacked).
	if (pos.canCastle(us, KING_SIDE)) {
		moveList.push_back(getCastling(us, KING_SIDE));
	}
	
	if (pos.canCastle(us, QUEEN_SIDE)) {
		moveList.push_back(getCastling(us, QUEEN_SIDE));
	}
	
	return moveList;
	
}

/* 
 * Gets all of the pseudo legal moves for one colour and returns them in a vector.
 */
vector<Move> getMoves(Position& pos, Colour us) {
	
	vector<Move> moveList;
	
	moveList = getPawnMoves(pos, us, moveList);
	moveList = getKnightMoves(pos, us, moveList);
	moveList = getBishopMoves(pos, us, moveList);
	moveList = getRookMoves(pos, us, moveList);
	moveList = getQueenMoves(pos, us, moveList);
	moveList = getKingMoves(pos, us, moveList);
	
	return moveList;
}

/* 
 * Returns all of the legal moves in a position, for the colour whose turn it is to play.
 */
vector<Move> getLegalMoves(Position& pos) {
	vector<Move> moveList = getMoves(pos, pos.getToMove());
	
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		if (!pos.isLegal(*it)) {
			it = moveList.erase(it) - 1;
		}
	}
	return moveList;
}

/* 
 * Returns all of the legal moves in a position, for a given colour.
 */
vector<Move> getLegalMoves(Position& pos, Colour c) {
	Square temp = pos.getEPTarget();
	if (c != pos.getToMove()) { // If we're testing moves for the colour whose turn it isn't, don't let them take on the ep square.
		pos.setEPTarget(NO_SQUARE);
	}
	
	vector<Move> moveList = getMoves(pos, c);
	
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		if (!pos.isLegal(*it, c)) {
			it = moveList.erase(it) - 1;
		}
	}
	
	pos.setEPTarget(temp); // Reset ep square
	
	return moveList;
}

/* 
 * Returns a string representation of a move (a4, Nb2, Bxc3, Nbd2, O-O-O, exf3e.p., g8=Q).
 * 
 * Flags : ?, ??, ?!, !?, !, !!, +/-, +/=, =, =/+, -/+, TN, N
 */
string getMoveStr(Position& pos, Move m, string flags = "") {
	
	string moveStr = "";
	
	// Get the type, to and from squares of the move.
	MoveType type = getMoveType(m);
	
	Square from = getFrom(m);
	Square to = getTo(m);
	
	// Get the type and colour of the moved piece.
	PieceType pieceType = getPieceType(pos.pieceAt(from));
	Colour us = getPieceColour(pos.pieceAt(from));
	
	bool capture = pos.isCapture(m);
	
	string toStr = pos.getSquareStr(to);
	
	// If it is castling we can return the string right away.
	if (type == CASTLING) {
		
		if(to > from) {
			moveStr.append("O-O");
		} else {
			moveStr.append("O-O-O");
		}
		
	} else { // Non castle.
	
		// If we moved a pawn we don't need to add the piece type char.
		if (pieceType == PAWN) {
			
			if (capture) {
				// Add the file we captured from, if a capture, and an x, e.g. fxe4, axb6.
				string fileFrom = pos.getFileStr(getFile(from));
				moveStr.append(fileFrom).append("x");
				
			}
			
			// Add the square we're captureing to.
			moveStr.append(toStr);
			
			if (type == ENPASSANT) {
				// If we captured en passant, add "e.p."
				moveStr.append("e.p.");
				
			} else if (type == PROMOTION) {
				// Add the promotion piece type if we promoted.
				PieceType promotionPiece = getPromotionPiece(m);
				moveStr.append("=").append(pos.getStringFromPieceType(promotionPiece));
				
			}
			
		} else { // Non pawn move, non castle.
			
			string typeString = pos.getStringFromPieceType(pieceType);
			
			// If we moved something other than a pawn, we need to add the piece type char to the start.
			moveStr += (pos.getStringFromPieceType(pieceType));
			
			// Get all the other attackers of this type to the square we moved to.
			// Do this by getting all attackers of this type to the square "to", and removing
			// the moved piece, by unsetting the bit at the square "from".
			Bitboard otherAttackers = removeFromBB(pos.getAttackersTo(to, us, pieceType), from);
			
			// If there were other attackers to the square, we need to specify which piece we moved.
			if (otherAttackers) {
				
				if (otherAttackers & getFileBB(getFile(from))) {
					
					if (otherAttackers & getRankBB(getRank(from))) {
						// There were attackers on the same file and rank, we need to specify the exact square we attacked from.
						// e.g. Nb4c6
						moveStr.append(pos.getFileStr(getFile(from)));
					}
					// Otherwise just add the rank
					// e.g. R4e5
					moveStr.append(pos.getRankStr(getRank(from)));
					
				} else {
					// If no attackers on the same file, we just add what file we attacked from (most common case)
					// e.g. Nbd2, Rgg7.
					moveStr.append(pos.getFileStr(getFile(from)));
					
				}
				
				
			} 
			
			// Add an x for a capture (Nxc4)
			if (capture) {
				
				moveStr.append("x");
				
			}
			
			moveStr.append(toStr);
			
		}
	
	}
	
	// + for check, # for checkmate.
	if (pos.isMoveCheckmate(m)) {
		moveStr.append("#");
	} else if (pos.isMoveCheck(m)) {
		moveStr.append("+");
	}
	
	// Flags like ! for good move, ?? for blunder, etc.
	return moveStr.append(flags);
	
}


/* 
 * Prints all moves in a vector in algebraic notation.
 */
void printMoves(Position& pos, vector<Move> moveList) {
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		printf("%s\n", getMoveStr(pos, *it).c_str());
	}
}

/* 
 * Prints all moves in a vector in algebraic notation.
 */
void printMovesWithScores(Position& pos, vector<Move> moveList) {
	int score = 0;
	Position next;
	Move m;
	string negative = "";
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		m = *it;
		next = Position(pos);
		next.doMove(m);
		score = evaluate(next);
		negative = score < 0 ? "-" : "";
		printf("%s : %s%d.%02d\n", getMoveStr(pos, m).c_str(), negative.c_str(), abs(score/100), abs(score % 100));
	}
}

/* 
 * Prints all moves in a vector in algebraic notation.
 */
void printMovesWithScores(Position& pos, vector<MoveNode> moveList) {
	int score = 0;
	MoveNode node;
	Move m;
	string negative = "";
	for(vector<MoveNode>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		node = *it;
		m = node.move;
		score = node.score;
		negative = score < 0 ? "-" : "";
		printf("%s : %s%d.%02d\n", getMoveStr(pos, m).c_str(), negative.c_str(), abs(score/100), abs(score % 100));
	}
}




