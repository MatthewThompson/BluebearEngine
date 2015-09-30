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
int evaluate(Position& pos, int depth);
MoveNode search(Position& pos, int depth);
MoveNode search(Position& pos, int depth, MoveNode root);
int evaluate(Position& pos);
int evaluate(Position& pos, Colour c);
int getPawnScores(Position& pos, Colour c);



struct MoveNode {
	
	MoveNode() : score(0), move(0) {}
	MoveNode(Move m) : score(0), move(m) {}
	MoveNode(Position& pos) : score(evaluate(pos)), move(0) {}
	MoveNode(Position& pos, Move m) : score(evaluate(pos)), move(m) {}
	
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
	8 , 10, 12, 14, 14, 12, 10, 8 ,
	13, 14, 16, 19, 19, 16, 14, 13,
	16, 18, 20, 24, 24, 20, 18, 16,
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

#endif // #ifndef EVALUATE_H_INCLUDED
