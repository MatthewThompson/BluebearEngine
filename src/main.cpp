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
string wmateIn1w = "k7/8/1K6/8/8/8/8/7R w - - 0 0";
string wmateIn1b = "k7/8/1K6/8/8/8/8/7R b - - 0 0";
string wmateIn2w = "1k6/8/1KR/8/8/8/8/8 w - - 0 0";
string wmateIn2b = "k7/8/1KR/8/8/8/8/8 b - - 0 0";
string bmateIn1w = "K7/8/1k6/8/8/8/8/7r w - - 0 0";
string bmateIn1b = "K7/8/1k6/8/8/8/8/7r b - - 0 0";
string bmateIn2w = "K7/8/1kr/8/8/8/8/8 w - - 0 0";
string bmateIn2b = "1K6/8/1kr/8/8/8/8/8 b - - 0 0";
string perft1 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";
string perft2 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0";
	

/* 
 * Calls all individual init functions in turn.
 */
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
		char promoChar = tolower(moveInput[4]);
		
		promotionType = (
		promoChar == 'n' ? KNIGHT:
		promoChar == 'b' ? BISHOP:
		promoChar == 'r' ? ROOK:
		promoChar == 'q' ? QUEEN:
		NO_PIECE_TYPE);
		
		if (promotionType == NO_PIECE_TYPE) {
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
 * 
 */
void playGame(Colour playerColour, int difficulty) {
	
	clock_t t1, t2;
	
	Position pos;
	init(pos);
	
	string game = "";
	string moveStr;
	Move m;
	int depth = difficulty;
	MoveNode root;
	
	float whiteTime = 0;
	float blackTime = 0;
	
	char moveNumStr[2];
	pos.drawBoard(playerColour);
	
	if (playerColour == BLACK) {
		game.append("1. ");
		
		printf("Engine thinking...\n\n");
		
		t1 = clock();
		root = depthFirst(pos, depth);
		t2 = clock();
		
		blackTime += ((((float)t2 - (float)t1) / 1000000.0F) * 1000);
		m = root.children.front().move;
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard(playerColour);
		
	}
	
	while(!pos.isCheckMate() && !pos.isDraw()) {
		m = 0;
		
		if (pos.getToMove() == WHITE) {
			sprintf(moveNumStr, "%u.", pos.getMoveNumber());
			game.append(moveNumStr);
			game.append(" ");
		}
		
		t1 = clock();
		while (!m) {
			printf("Please input a move : ");
			getline(cin, moveStr);
			m = getMoveFromString(moveStr, pos);
		}
		t2 = clock();
		
		whiteTime += (((float)t2 - (float)t1) / 1000000.0F) * 1000;
		printf("\n");
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard(playerColour);
		
		if (pos.isCheckMate() || pos.isDraw()) {
			break;
		}
		
		
		if (pos.getToMove() == WHITE) {
			sprintf(moveNumStr, "%u.", pos.getMoveNumber());
			game.append(moveNumStr);
			game.append(" ");
		}
		
		printf("Engine thinking...\n\n");
		
		t1 = clock();
		root = depthFirst(pos, depth);
		t2 = clock();
		
		blackTime += (((float)t2 - (float)t1) / 1000000.0F) * 1000;
		m = root.children.front().move;
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard(playerColour);
		
		
	}
	
	if(pos.isDraw()) {
		
		game.append("1/2 - 1/2");
		
		printf("The game was a draw.\n");
		
	} else {
		
		game.append(pos.getToMove() == WHITE ? "0 - 1" : "1 - 0");
		
		if (pos.getToMove() == playerColour) {
			printf("You lost.\n");
		} else {
			printf("Congrats you won.\n");
		}
		
	}
	
	printf("Here was the game :\n%s\n\n", game.c_str());
	
	printf("White thought for : %f seconds\n", whiteTime);
	printf("Black thought for : %f seconds\n", blackTime);
	printf("Enter anything to quit.");
	getline(cin, moveStr);
	
}


/* 
 * 
 */
void startGame() {
	// Play a game vs computer.
	printf("\n\n");
	printf("Welcome to the Bluebear Chess engine.\n");
	printf("To play, input a move by giving a to and from coordinate,\n");
	printf("e.g. e2e4, or, a1h8.\n");
	printf("The only time you must indicate something else is when\n");
	printf("promoting a pawn, a7a8n will promote to a knight, a1a8b to\n");
	printf("a bishop etc, if no piece type is specified it will default to a queen.\n\n");
	printf("To start a game, choose a colour (w/b).");
	
	
	char colourChar = 0;
	while (colourChar != 'w' && colourChar != 'b') {
		cin >> colourChar;
	}
	
	printf("Now choose a difficulty (1-4)");
	
	int difficulty = 0;
	while (difficulty < 1 || difficulty > 4) {
		cin >> difficulty;
	}
	
	Colour playerColour = colourChar == 'w' ? WHITE : BLACK;
	playGame(playerColour, difficulty);
}


/* 
 * 
 */
void enginevengine(int depth) {
	
	clock_t t1, t2;
	
	Position pos;
	init(pos);
	
	string game = "";
	string moveStr;
	Move m;
	MoveNode root;
	
	float whiteTime = 0;
	float blackTime = 0;
	
	char moveNumStr[2];
	pos.drawBoard(WHITE);
	
	while(!pos.isCheckMate() && !pos.isDraw()) {
		m = 0;
		
		if (pos.getToMove() == WHITE) {
			sprintf(moveNumStr, "%u.", pos.getMoveNumber());
			game.append(moveNumStr);
			game.append(" ");
		}
		
		
		printf("Search Engine thinking...\n\n");
		
		t1 = clock();
		root = search(pos, depth);
		t2 = clock();
		
		whiteTime += ((((float)t2 - (float)t1) / 1000000.0F) * 1000);
		m = root.children.front().move;
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard(BLACK);
		
		if (pos.isCheckMate() || pos.isDraw()) {
			break;
		}
		
		
		if (pos.getToMove() == WHITE) {
			sprintf(moveNumStr, "%u.", pos.getMoveNumber());
			game.append(moveNumStr);
			game.append(" ");
		}
		
		printf("DFS Engine thinking...\n\n");
		
		t1 = clock();
		root = depthFirst(pos, depth + 1);
		t2 = clock();
		
		blackTime += (((float)t2 - (float)t1) / 1000000.0F) * 1000;
		m = root.children.front().move;
		
		game.append(getMoveStr(pos, m));
		game.append(" ");
		
		pos.doMove(m);
		
		pos.drawBoard(BLACK);
		
		
	}
	
	if(pos.isDraw()) {
		
		game.append("1/2 - 1/2");
		
		printf("The game was a draw.\n");
		
	} else {
		
		game.append(pos.getToMove() == WHITE ? "0 - 1" : "1 - 0");
		
		if (pos.getToMove() == WHITE) {
			printf("White won.\n");
		} else {
			printf("Black won.\n");
		}
		
	}
	
	printf("Here was the game :\n%s\n\n", game.c_str());
	printf("White thought for : %f seconds\n", whiteTime);
	printf("Black thought for : %f seconds\n", blackTime);
}

/* 
 * To be moved to util header soon tm.
 */
string getScoreStr(int score) {
	char scoreStr[8]; // I think this is the max length it can be.
	printf("score : %d\n\n", score);
	bool negative = score < 0;
	score = abs(score);
	if (score < 90000) {
		
		sprintf(scoreStr,"%s%u.%02u", negative ? "-" : "", score/100, score % 100);
		
	} else { // It's over nine th... never mind.
		
		int mateInN = 100000 - score;
		sprintf(scoreStr,"%sM%u", negative ? "-" : "", mateInN);
		
	}
	
	return string(scoreStr);
}

/*
 * Main function, first calls init, atm used for testing my code.
 */
int main(void) {
	
	clock_t t1, t2;
	//t1 = clock();
	//t2 = clock();
	
	/*
	Position pos;
	init(pos, bmateIn2w);
	int depth  = 3;
	MoveNode root = search(pos, depth);
	
	MoveNode node = root;
	int score = root.score;
	string negative = score < 0 ? "-" : "";
	printf("D%u (%s) :  ", depth, getScoreStr(score).c_str());
	
	bool first = true;
	MoveNode temp;
	Position tempPos(pos);
	Move m;
	
	while(node.children.size()) {
		
		temp = node.children.front();
		m = temp.move;
		
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
	
	// COMPUTER VS COMPUTER
	//enginevengine(3);
	// END COMPUTER VS COMPUTER
	
	
	// START PERSON V COMPUTER
	startGame();
	// END PLAY GAME
	
	
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
	
	/*
	printf("\n\n");
	float diff = (((float)t2 - (float)t1) / 1000000.0F) * 1000;
	printf("%f\n", diff);
	*/
	
	
	/*
	for (int i = 0; i < 5; i++) {
		
		printf("%u : %u\n", i, perft(pos, i));
		
	}
	*/
	
    return 0;
	
}
 
 
 
 