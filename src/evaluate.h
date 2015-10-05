/* 
 * 
 * EVALUATE.H
 * 
 * 
 */
 
#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

#include <string>
#include <vector>

#include "position.h"
#include "moves.h"
#include "types.h"

struct MoveNode;

// Initialise all precomputd values and arrays used to evaluate a position.
void initEval();

bool ascending(MoveNode a, MoveNode b);
bool descending(MoveNode a, MoveNode b);

// Gives a score in centipawns (100 = 1.00).
MoveNode search(Position& pos, int depth, int maxdepth);
MoveNode breadthFirst(Position& pos, int depth);
MoveNode depthFirst(Position& pos, int depth);
MoveNode search(Position& pos, int depth, MoveNode root, int depthFromRoot, int maxDepth, bool endgame);
MoveNode breadthFirst(Position& pos, int depth, MoveNode root, int depthFromRoot, bool endgame);
MoveNode depthFirst(Position& pos, int depth, MoveNode root, int depthFromRoot, bool endgame);
int evaluate(Position& pos, int depthFromRoot = 0);
int evaluate(Position& pos, Colour c);
int getPawnScores(Position& pos, Colour c);
int getKnightScores(Position& pos, Colour c);
int getKingSafety(Position& pos, Colour c);

struct MoveNode {
	
	MoveNode() : score(0), move(0) {}
	MoveNode(Move m) : score(0), move(m) {}
	MoveNode(Position& pos, int depthFromRoot) : score(evaluate(pos, depthFromRoot)), move(0) {}
	MoveNode(Position& pos, Move m, int depthFromRoot) : score(evaluate(pos, depthFromRoot)), move(m) {}
	
	bool operator<(const MoveNode& m) const { return score < m.score; }
	bool operator>(const MoveNode& m) const { return score > m.score; }
	bool operator==(const MoveNode& m) const { return score == m.score; }
	
	int score;
	//MoveNode parent;
	Move move;
	vector<MoveNode> children;
	
};





const int pawnScores[64] = { // This is shown from black's perspective
//A1
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	4 , 8 , 4 , 5 , 5 , 4 , 8 , 4 ,
	4 , 7 , 8 , 10, 10, 8 , 7 , 4 ,
	12, 15, 18, 20, 20, 18, 15, 12,
	18, 20, 26, 30, 30, 26, 20, 18,
	22, 24, 28, 36, 36, 28, 24, 22,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  // H8
};
/* For white:

0    0    0    0    0    0    0    0
16   18   20   24   24   20   18   16
13   14   16   19   19   16   14   13
8    10   12   14   14   12   10   8
4    7    8    10   10   8    7    4
4    8    4    5    5    4    8    4
0    0    0    0    0    0    0    0
0    0    0    0    0    0    0    0

*/


const int knightScores[64] = { // 
//A1
	-35, -30, -20, -20, -20, -20, -30, -35,
	-30, -15, -10, -10, -10, -10, -15, -30,
	-20, -10,  0 ,  0 ,  0 ,  0 , -10, -20,
	-20, -10,  0 ,  0 ,  0 ,  0 , -10, -20,
	-20, -10,  0 ,  0 ,  0 ,  0 , -10, -20,
	-20, -10,  0 ,  0 ,  0 ,  0 , -10, -20,
	-30, -15, -10, -10, -10, -10, -15, -30,
	-35, -30, -20, -20, -20, -20, -30, -35  // H8
};


const int kingSafety[64] = { // 
//A1
	0  ,  0  ,  -10,  -20,  -20,  -10,  0  ,  0  ,
	0  ,  -5 ,  -12,  -25,  -25,  -12,  -5 ,  0  ,
	-8 ,  -18,  -30,  -40,  -40,  0  ,  -18,  -8 ,
	-10,  -30,  -40,  -60,  -60,  -40,  -30,  -10,
	-10,  -30,  -40,  -60,  -60,  -40,  -30,  -10,
	-8 ,  -18,  -30,  -40,  -40,  0  ,  -18,  -8 ,
	0  ,  -5 ,  -12,  -25,  -25,  -12,  -5 ,  0  ,
	0  ,  0  ,  -10,  -20,  -20,  -10,  0  ,  0  // H8
};











#endif // #ifndef EVALUATE_H_INCLUDED
