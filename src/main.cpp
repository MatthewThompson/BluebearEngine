/* 
 * MAIN.CPP
 * 
 * Main, contains a function to initialise everything and calls it.
 * 
 * 
 */
 

#include <stdio.h>
#include <iostream.h>
#include <vector>
#include <string>
#include <time.h>

#include "evaluate.h"
#include "position.h"
#include "bitboards.h"
#include "types.h"
#include "moves.h"

using namespace std;


// The FEN for the start position.
string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


// Calls all individual init functions in turn.
void init(Position& pos, string FEN = startFEN) {
	
	initBitboards();
	
	pos.initPosition();
	
	pos.parseFEN(FEN);
	
}


uint64_t perft(Position& pos, int depth) {
	if (depth == 0) {
		return 1;
	}
	
	if (depth == 1) {
		return getLegalMoves(pos).size();
	}
	
	vector<Move> moveList = getLegalMoves(pos);
	
	Position next;
	
	int paths = 0;
	
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		next = Position(pos);
		next.doMove(*it);
		paths += perft(next, depth - 1);
	}
	
	return paths;
	
}





/*
 * Main function, first calls init, atm used for testing my code.
 */
int main(void) {
	
	string perf1 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";
	string perf2 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0";
	
	Position pos;
	init(pos);
	
	
	//vector<Move> moveList;
	
	clock_t t1, t2;
	//moveList = getLegalMoves(pos);
	
	/*
	pos.doMove(getMove(E2, E3, NORMAL));
	pos.doMove(getMove(E7, E5, NORMAL));
	pos.doMove(getMove(D1, H5, NORMAL));
	pos.doMove(getMove(B8, C6, NORMAL));
	pos.doMove(getMove(F1, B5, NORMAL));
	pos.doMove(getMove(G8, F6, NORMAL));
	pos.doMove(getMove(H5, F5, NORMAL));
	pos.doMove(getMove(E5, E4, NORMAL));
	pos.doMove(getMove(B2, B3, NORMAL));
	pos.doMove(getMove(C6, B4, NORMAL));
	pos.doMove(getMove(F5, E5, NORMAL));
	pos.doMove(getMove(F8, E7, NORMAL));
	pos.doMove(getMove(E1, D1, NORMAL));
	pos.doMove(getMove(A7, A6, NORMAL));
	*/
	
	
	
	int depth = 5;
	MoveNode root;
	
	t1 = clock();
	root = search(pos, depth);
	t2 = clock();
	
	
	
	
	//printMovesWithScores(pos, root.children);
	
	MoveNode node = root;
	int score = root.score;
	string negative = score < 0 ? "-" : "";
	printf("D%u (%s%d.%02d) :  ", depth, negative.c_str(), abs(score/100), abs(score % 100));
	
	bool first = true;
	MoveNode temp;
	Position tempPos(pos);
	Move m;
	while(node.children.size()) {
		
		temp = node.children.front();
		m = temp.move;
		score = temp.score;
		negative = score < 0 ? "-" : "";
		
		if (tempPos.getToMove() == WHITE) {
			printf("%u.  ", tempPos.getMoveNumber());
		} else if (first) {
			printf("%u.  ...", tempPos.getMoveNumber());
		}
		printf("%s  ", getMoveStr(tempPos, m).c_str());
		
		tempPos.doMove(m);
		node = temp;
		first = false;
	}
	
	
	/*
	printf("\n\n");
	pos.drawBoard();
	
	//node = root.children.front().children.front();
	node = root;
	
	vector<MoveNode> moves = node.children;
	for(vector<MoveNode>::iterator it = moves.begin(); it != moves.end(); it++) {
		node = *it;
		m = node.move;
		score = node.score;
		negative = score < 0 ? "-" : "";
		printf("%s : %s%d.%02d (%u children)\n", getMoveStr(pos, m).c_str(), negative.c_str(), abs(score/100), abs(score % 100), node.children.size());
	}
	
	//printf("score : %d\n", root.score);
	*/
	
	
	/*
	int score = 0;
	string negative = "";
	for (int i = 0; i < 5; i++) {
		score = evaluate(pos, i);
		negative = score < 0 ? "-" : "";
		printf("%u : %s%d.%02d\n", i, negative.c_str(), abs(score/100), abs(score % 100));
	}
	*/
	
	
	printf("\n\n");
	float diff = (((float)t2 - (float)t1) / 1000000.0F) * 1000;
	printf("%f\n", diff);
	
	
	
	/*
	for (int i = 0; i < 5; i++) {
		
		printf("%u : %u\n", i, perft(pos, i));
		
	}
	*/
	
    return 0;
	
}
 
 
 
 