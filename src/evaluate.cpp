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
	return search(pos, depth, root, 0, depth + 2);
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
	
	if (depthFromRoot > maxDepth) {
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	if (depth == 0) {
		
		if (pos.getCaptured() || pos.isCheck()) {
			
			return search(pos, 1, root, depthFromRoot, maxDepth);
			
		} else {
			
			root.score = evaluate(pos, depthFromRoot);
			return root;
			
		}
		
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
		
		MoveNode node(m);
		moves.push_back(search(next, depth - 1, node, depthFromRoot + 1, maxDepth));
		
		
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
		
		MoveNode node(m);
		moves.push_back(depthFirst(next, depth - 1, node, depthFromRoot + 1));
		
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
	material += pos.getPieceCount(c, BISHOP) * 310;
	material += pos.getPieceCount(c, ROOK) * 500;
	material += pos.getPieceCount(c, QUEEN) * 900;
	
	Bitboard bishops = pos.getPieces(c, BISHOP);
	if ((bishops & getLightSquares()) && (bishops & getDarkSquares())) { // Has bishop pair
		material += 50;
	}
	
	// END MATERIAL
	
	
	
	// PAWNS
	int pawns = 0;
	
	pawns += getPawnScores(pos, c);
	// ISOLATED
	// BACKWARDS
	// BLOCKED
	
	// END PAWNS
	
	// KNIGHTS
	int knights = getKnightScores(pos, c);
	// END KNIGHTS
	
	
	
	// KING SAFETY
	int kingSafety = 0;
	int enemyMaterial =
	pos.getPieceCount(~c, KNIGHT) * 300 +
	pos.getPieceCount(~c, BISHOP) * 300 + 
	pos.getPieceCount(~c, ROOK) * 500 +
	pos.getPieceCount(~c, QUEEN) * 900;
	if (enemyMaterial > 1200) {
		kingSafety += getKingSafety(pos, c);
	}
	
	// END KING SAFETY
	
	
	// MOBILITY
	int mobility = getLegalMoves(pos, c).size() * 2;
	// END MOBILITY
	
	
	return material + pawns + knights + kingSafety + mobility;
}


/* 
 * 
 */
int getPawnScores(Position& pos, Colour c) {
	
	int pawnScore = 0;
	
	Square *pawns = pos.getPieceList(c, PAWN);
	
	if (c == WHITE) {
		
		for (int i = 0; i < pos.getPieceCount(c, PAWN); i++) {
			pawnScore += pawnScores[pawns[i]];
		}
		
	} else {
		
		for (int i = 0; i < pos.getPieceCount(c, PAWN); i++) {
			pawnScore += pawnScores[63 - pawns[i]];
		}
		
	}
	return pawnScore;
	
}

/* 
 * 
 */
int getKnightScores(Position& pos, Colour c) {
	
	int knightScore = 0;
	
	Square *knights = pos.getPieceList(c, KNIGHT);
	
	for (int i = 0; i < pos.getPieceCount(c, KNIGHT); i++) {
		knightScore += knightScores[knights[i]]; // Symmetrical so doesn't make a difference.
	}
		
	return knightScore;
	
}

/* 
 * 
 */
int getKingSafety(Position& pos, Colour c) {
	
	int knightScore = 0;
	
	Square king = pos.getKingSquare(c);
	
		
	return kingSafety[king];
	
}



