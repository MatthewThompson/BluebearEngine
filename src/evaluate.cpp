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

using namespace std;

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


/* 
 * 
 */
MoveNode search(Position& pos, int depth) {
	MoveNode root;
	return search(pos, depth, root, 0, depth + 3);
}

/* 
 * 
 */
MoveNode depthFirst(Position& pos, int depth) {
	MoveNode root;
	return depthFirst(pos, depth, root, 0);
}

/* 
 * 
 */
MoveNode search(Position& pos, int depth, MoveNode root, int depthFromRoot, int maxDepth) {
	
	if (depth == 0 || depthFromRoot > maxDepth) {
		root.score = evaluate(pos, depthFromRoot);
		return root;
		
	}
	
	Position next;
	vector<Move> moveList = getLegalMoves(pos);
	if (moveList.size() == 0) { // Checkmate or stalemate.
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	vector<MoveNode> moves;
	Move m;
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		
		m = *it;
		next = Position(pos);
		next.doMove(m);
		
		if (depth == 1) {
			
			if (pos.isCapture(m) || pos.isMoveCheck(m)) {
				MoveNode node(m);
				moves.push_back(search(next, depth, node, depthFromRoot + 1, maxDepth));
			} else {
				moves.push_back(MoveNode(next, m, depthFromRoot + 1));
			}
			
		} else { // Depth > 1
			
			MoveNode node(m);
			moves.push_back(search(next, depth - 1, node, depthFromRoot + 1, maxDepth));
			
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
 * 
 */
MoveNode depthFirst(Position& pos, int depth, MoveNode root, int depthFromRoot) {
	
	if (depth == 0) {
		
		return MoveNode(pos, depthFromRoot);
		
	}
	
	Position next;
	vector<Move> moveList = getLegalMoves(pos);
	if (moveList.size() == 0) { // Checkmate or stalemate.
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	vector<MoveNode> moves;
	Move m;
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		
		m = *it;
		next = Position(pos);
		next.doMove(m);
		
		if (depth == 1) {
			
			moves.push_back(MoveNode(next, m, depthFromRoot + 1));
			
		} else { // Depth > 1
			
			MoveNode node(m);
			moves.push_back(depthFirst(next, depth - 1, node, depthFromRoot + 1));
			
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
int evaluate(Position& pos, int depthFromRoot = 0) {
	
	if (pos.isDraw()) {
		return 0;
	}
	
	if (pos.isCheckMate()) {
		int mateInN = depthFromRoot;
		if (depthFromRoot % 2 != 0) {
			mateInN++;
		}
		mateInN = mateInN / 2;
		return pos.getToMove() == WHITE ? -100000 + mateInN : 100000 - mateInN;
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



