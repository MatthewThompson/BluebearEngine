/* 
 * 
 * EVALUATE.CPP
 * 
 * 
 */



#include <algorithm> // stable_sort, min, max
#include <string>
#include <vector>

#include "evaluate.h"

#include "bitboards.h"
#include "moves.h"
#include "position.h"
#include "types.h"

/*
 * Initialise all precomputd values and arrays used to evaluate a position.
 */
void initEval() {
	
}

//
bool ascending(MoveNode a, MoveNode b) {
	return a < b;
}

//
bool descending(MoveNode a, MoveNode b) {
	return a > b;
}

int max(vector<int> scores) {
	int maxScore = -100001;
	
	for(vector<int>::iterator it = scores.begin(); it != scores.end(); it++) {
		if ((*it) > maxScore) {
			maxScore = (*it);
		}
	}
	
	return maxScore;
}

int min(vector<int> scores) {
	int minScore = 100001;
	
	for(vector<int>::iterator it = scores.begin(); it != scores.end(); it++) {
		if ((*it) < minScore) {
			minScore = (*it);
		}
	}
	
	return minScore;
}

/* 
 * 
 */
int evaluate(Position& pos, int depth) {
	
	if (depth == 0) {
		return evaluate(pos);
	}
	
	vector<int> scores;
	vector<Move> moveList = getLegalMoves(pos);
	
	Position next;
	Move m;
	int nextScore;
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		m = *it;
		next = Position(pos);
		next.doMove(m);
		nextScore = evaluate(next, depth - 1);
		scores.push_back(nextScore);
	}
	
	return pos.getToMove() == WHITE ? max(scores) : min(scores);
	
}

/* 
 * 
 */
MoveNode search(Position& pos, int depth) {
	return search(pos, depth, MoveNode(pos));
}

/* 
 * 
 */
MoveNode search(Position& pos, int depth, MoveNode root) {
	
	if (depth == 0) {
		
		return MoveNode(pos);
		
	}
	
	Position next;
	vector<Move> moveList = getLegalMoves(pos);
	if (moveList.size() == 0) { // Checkmate or stalemate.
		return MoveNode(pos);
	}
	
	vector<MoveNode> moves;
	Move m;
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		
		m = *it;
		next = Position(pos);
		next.doMove(m);
		
		if (depth == 1) {
			
			moves.push_back(MoveNode(next, m));
			
		} else { // Depth > 1
			
			MoveNode node(m);
			moves.push_back(search(next, depth - 1, node));
			
		}
		
	}
	
	
	pos.getToMove() == WHITE ?  stable_sort(moves.begin(), moves.end(), descending):
								stable_sort(moves.begin(), moves.end(), ascending);
	
	
	root.children = moves;
	
	vector<MoveNode> sortedMoves = root.children;
	root.score = moves.front().score;
	
	
	return root;
	
}

/*
 * Gives a score in centipawns (100 = 1.00).
 */
int evaluate(Position& pos) {
	
	if (pos.isDraw()) {
		return 0;
	}
	
	if (pos.isCheckMate()) {
		return pos.getToMove() == WHITE ? -100000 : 100000;
	}
	
	
	int whiteScore = evaluate(pos, WHITE);
	
	
	int blackScore = evaluate(pos, BLACK);
	
	
	
	return whiteScore - blackScore;
	
}

/* 
 * 
 */
int evaluate(Position& pos, Colour c) {
	
	//
	int material = 0;
	
	material += pos.getPieceCount(c, PAWN) * 100;
	material += pos.getPieceCount(c, KNIGHT) * 300;
	material += pos.getPieceCount(c, BISHOP) * 300;
	material += pos.getPieceCount(c, ROOK) * 500;
	material += pos.getPieceCount(c, QUEEN) * 900;
	
	// END MATERIAL
	
	
	//
	int pawns = 0;
	
	pawns += getPawnScores(pos, c);
	// ISOLATED
	// BACKWARDS
	// BLOCKED
	// POSITIONS (use array).
	
	
	// END PAWNS
	
	//
	int mobility = getLegalMoves(pos, c).size();
	
	return material + pawns + mobility;
}



int getPawnScores(Position& pos, Colour c) {
	
	int pawnScore = 0;
	
	Bitboard pawns = pos.getPieces(c, PAWN);
	if (c == WHITE) {
		
		while (pawns) {
			pawnScore += pawnScores[pop_lsb(&pawns)];
		}
		
	} else {
		
		while (pawns) {
			pawnScore += pawnScores[63 - pop_lsb(&pawns)];
		}
		
	}
	return pawnScore;
	
}



