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
 * This function shouldn't be here.
 */
Move getMoveFromString(string moveInput, Position& pos) {
	if (moveInput.size() > 5 || moveInput.size() < 4) {
		return 0;
	}
	
	vector<Move> moveList = getLegalMoves(pos);
	
	moveInput[0] = tolower(moveInput[0]);
	moveInput[2] = tolower(moveInput[2]);
	
	Square from = Position::getSquareFromStr(moveInput.substr(0, 2));
	Square to = Position::getSquareFromStr(moveInput.substr(2, 2));
	
	
	PieceType promotionType = NO_PIECE_TYPE;
	
	if (moveInput.size() == 5) {
		moveInput[4] = tolower(moveInput[4]);
		promotionType = 
		moveInput[4] == 'n' ? KNIGHT:
		moveInput[4] == 'b' ? BISHOP:
		moveInput[4] == 'r' ? ROOK:
		moveInput[4] == 'q' ? QUEEN:
		NO_PIECE_TYPE;
		if (promotionType = NO_PIECE_TYPE) {
			return 0;
		}
	}
	
	Move m; // Check if the move is one of the legal ones for this position.
	for(vector<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		m = *it;
		if (getFrom(m) == from && getTo(m) == to) {
			if (getMoveType(m) == PROMOTION) {
				
				if (getPromotionPiece(m) == promotionType) {
					return m;
				} else if (getPromotionPiece(m) == QUEEN && promotionType == NO_PIECE_TYPE) { // Promotion with no type specified, default to queening.
					return m;
				}
				
			} else {
				return m;
			}
			
		}
		
	}
	
	return 0;
	
}


/*
 * Main function, first calls init, atm used for testing my code.
 */
int main(void) {
	
	string perf1 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";
	string perf2 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0";
	
	
	
	//vector<Move> moveList;
	
	Position pos;
	init(pos);
	
	clock_t t1, t2;
	//moveList = getLegalMoves(pos);
	
	
	
	
	//t1 = clock();
	//root = search(pos, depth);
	//t2 = clock();
	
	string game = "";
	string moveStr;
	Move m;
	int depth = 4;
	MoveNode root;
	
	char moveNumStr[2];
	pos.drawBoard();
	
	Colour playerColour = WHITE;
	
	while(!pos.isCheckMate() && !pos.isDraw()) {
		//moveStr = "";
		m = 0;
		
		if (pos.getToMove() == WHITE) {
			sprintf(moveNumStr, "%u.", pos.getMoveNumber());
			game.append(moveNumStr);
			game.append(" ");
		}
		
		while (!m) {
			printf("Please input a move : ");
			getline(cin, moveStr);
			m = getMoveFromString(moveStr, pos);
		}
		printf("\n");
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard();
		
		if (pos.isCheckMate() || pos.isDraw()) {
			break;
		}
		
		printf("Engine thinking...\n\n");
		
		root = search(pos, depth);
		m = root.children.front().move;
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard();
		
		
	}
	
	if(pos.isDraw()) {
		
		game.append("1/2 - 1/2");
		
		printf("The game was a draw.\n");
		
	} else {
		
		game.append(pos.getToMove() == WHITE ? "0 - 1" : "1 - 0");
		
		if (pos.getToMove() == playerColour) {
			printf("Congrats you won.\n");
		} else {
			printf("You lost.\n");
		}
		
	}
	
	printf"Here was the game :\n%s\n", game);
	
	
	//printMovesWithScores(pos, root.children);
	/*
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
	*/
	
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
 
 
 
 