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

/* 
 * 
 */
bool ascending(MoveNode a, MoveNode b) {
	return a < b;
}

/* 
 * 
 */
bool descending(MoveNode a, MoveNode b) {
	return a > b;
}

vector<MoveNode> merge(vector<MoveNode> moves1, vector<MoveNode> moves2, bool (*f)(MoveNode, MoveNode)) {
	vector<MoveNode> merged;
	vector<MoveNode>::iterator it1 = moves1.begin();
	vector<MoveNode>::iterator it2 = moves2.begin();
	
	while (it1 != moves1.end() && it2 != moves2.end()) {
		if (f(*it1,*it2)) {
			merged.push_back(*it1);
			it1++;
		} else {
			merged.push_back(*it2);
			it2++;
		}
	}
	
	if (it1 != moves1.end()) {
		merged.insert(merged.end(), it1, moves1.end());
		return merged;
	} else {
		merged.insert(merged.end(), it2, moves2.end());
		return merged;
	}
	
}

/* 
 * 
 */
MoveNode search(Position& pos, int depth, int maxdepth) {
	MoveNode root;
	for (int i = 1; i <= depth; i++) {
		root = search(pos, i, root, 0, maxdepth, false);
		if (abs(root.score) > 9000) {
			return root; // If we found a forced mate, stop searching.
		}
	}
	return root;
}

/* 
 * 
 */
MoveNode breadthFirst(Position& pos, int depth) {
	MoveNode root;
	for (int i = 1; i <= depth; i++) {
		root = depthFirst(pos, i, root, 0, false);
		if (abs(root.score) > 9000) {
			return root; // If we found a forced mate, stop searching.
		}
	}
	return root;
}

/* 
 * 
 */
MoveNode depthFirst(Position& pos, int depth) {
	MoveNode root;
	return depthFirst(pos, depth, root, 0, false);
}

/* 
 * 
 */
MoveNode search(Position& pos, int depth, MoveNode root, int depthFromRoot, int maxDepth, bool endgame) {
	
	if (root.pruned || depthFromRoot > maxDepth) {
		if (root.pruned) {
			printf("we got pruned\n");
		}
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	if (depth == 0) {
		
		if (pos.getCaptured() || pos.isCheck()) {
			
			return search(pos, 1, root, depthFromRoot + 1, maxDepth, endgame);
			
		} else {
			
			root.score = evaluate(pos, depthFromRoot);
			return root;
			
		}
		
	}
	
	if (pos.isDraw() || pos.isCheckMate()) { // Checkmate or stalemate or 50move rule.
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	vector<MoveNode> moves;
	Position next;
	
	if (root.children.size() == 0) { // Not yet searched (if no moves due to check or stalemate this is already caught).
		
		Move m;
		vector<Move> quietMoveList = getLegalMoves(pos, QUIET);
		vector<Move> captureMoveList = getLegalMoves(pos, CAPTURE);
		vector<MoveNode> quietMoves;
		vector<MoveNode> captureMoves;
		
		for(vector<Move>::iterator it = quietMoveList.begin(); it != quietMoveList.end(); it++) {
			
			m = *it;
			next = Position(pos);
			next.doMove(m);
			
			MoveNode node(m);
			quietMoves.push_back(search(next, depth - 1, node, depthFromRoot + 1, maxDepth, endgame));
			
		}
		
		for(vector<Move>::iterator it = captureMoveList.begin(); it != captureMoveList.end(); it++) {
			
			m = *it;
			next = Position(pos);
			next.doMove(m);
			
			MoveNode node(m);
			captureMoves.push_back(search(next, depth - 1, node, depthFromRoot + 1, maxDepth, endgame));
			
		}
		
		Colour toMove = pos.getToMove();
		if (toMove == WHITE) {
			
			stable_sort(quietMoves.begin(), quietMoves.end(), descending);
			stable_sort(captureMoves.begin(), captureMoves.end(), descending);
			
		} else {
			
			stable_sort(quietMoves.begin(), quietMoves.end(), ascending);
			stable_sort(captureMoves.begin(), captureMoves.end(), ascending);
			
		}
		
		int i = 0;
		for (vector<MoveNode>::iterator it = quietMoves.begin(); it != quietMoves.end(); it++) {
			if (i >= 5) { // Only look at the best 5 quiets each time.
				(*it).pruned = true;
			}
			i++;
		}
	
		moves = toMove == WHITE ? merge(quietMoves, captureMoves, descending):
								merge(quietMoves, captureMoves, ascending);
		
	} else {
		printf("hi\n");
		vector<MoveNode> searchedMoves = root.children;
		for (vector<MoveNode>::iterator it = searchedMoves.begin(); it != searchedMoves.end(); it++) {
			printf("hiloop\n");
			next = Position(pos);
			next.doMove(it->move);
			moves.push_back(search(next, depth - 1, *it, depthFromRoot + 1, maxDepth, endgame));
		}
		
		pos.getToMove() == WHITE ?  stable_sort(moves.begin(), moves.end(), descending):
									stable_sort(moves.begin(), moves.end(), ascending);
		
	}
	
	root.children = moves;
	root.score = moves.front().score;
	
	return root;
	
}

/* 
 * 
 */
MoveNode breadthFirst(Position& pos, int depth, MoveNode root, int depthFromRoot, bool endgame) {
	
	
	
}

/* 
 * 
 */
MoveNode depthFirst(Position& pos, int depth, MoveNode root, int depthFromRoot, bool endgame) {
	
	if (depth == 0 || pos.isDraw() || pos.isCheckMate()) { // Checkmate or stalemate or 50move rule.
		root.score = evaluate(pos, depthFromRoot);
		return root;
	}
	
	Position next;
	vector<Move> moveList = getLegalMoves(pos);
	vector<MoveNode> moves;
	Move m;
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		
		m = *it;
		next = Position(pos);
		next.doMove(m);
		
		MoveNode node(m);
		moves.push_back(depthFirst(next, depth - 1, node, depthFromRoot + 1, endgame));
		
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



