/* 
 * 
 * POSITION.H
 * 
 * Header file for position.
 * 
 */


#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include <vector>
#include <string>
#include "types.h"


class Position;


//
class Position {

public:
	
	//
	void initPosition(void);
	
	//
	Bitboard getPieces();
	Bitboard getPieces(Colour c);
	Bitboard getPieces(PieceType pt);
	Bitboard getPieces(Colour c, PieceType pt);
	Square * getPieceList(Colour c, PieceType pt);
	Square getKingSquare(Colour c);
	Piece pieceAt(Square s);
	bool isEmpty(Square s);
	
	int getPieceCount(Colour c, PieceType pt);
	
	
	// Private variable getters
	Colour getToMove();
	//CastlingRight getCastlingRights();
	Square getEPTarget();
	void Position::setEPTarget(Square s);
	//int getMovesSinceCapture(); // Don't add unless needed.
	int getMoveNumber(); //
	Piece getCaptured();
	
	//
	void doMove(Move m);
	void tempDoMove(Move m);
	void undoTempMove(Move m);
	
	
	// Util stuff
	void drawBoard(Colour c);
	void drawBoard(void);
	void drawFlipBoard(void);
	int parseFEN(string FEN);
	
	// Move to util header???
	static Piece getPieceFromChar(char c);
	static string getStringFromPieceType(PieceType pt);
	static char getCharFromPiece(Piece p);
	static string getFileStr(File f);
	static string getRankStr(Rank r);
	static string getSquareStr(Square s);
	static File getFileFromChar(char c);
	static Rank getRankFromChar(char c);
	static Square getSquareFromStr(string str);
	
	//
	bool isCheck();
	bool isInCheck(Colour c);
	bool isCheckMate();
	bool isInCheckMate(Colour c);
	bool isDraw();
	bool isStaleMate();
	bool isInStaleMate(Colour c);
	bool isCapture(Move m);
	bool isLegal(Move m);
	bool isLegal(Move m, Colour c);
	bool isMoveCheck(Move m);
	bool isMoveCheckmate(Move m);
	
	//
	Bitboard getPawnAttackersTo(Square s, Colour c);
	Bitboard getKnightAttackersTo(Square s);
	Bitboard getBishopAttackersTo(Square s);
	Bitboard getRookAttackersTo(Square s);
	Bitboard getQueenAttackersTo(Square s);
	Bitboard getKingAttackersTo(Square s);
	Bitboard getAttackersTo(Square s, PieceType pt);
	Bitboard getAttackersTo(Square s);
	Bitboard getAttackersTo(Square s, Colour c);
	Bitboard getAttackersTo(Square s, Colour c, PieceType pt);
	
	//
	bool canCastle(Colour c, CastlingSide side);
	
private:
	
	// Helpers for doing and undoing moves.
	int addPiece(Colour c, PieceType pt, Square s);
	int addPiece(Colour c, PieceType pt, File f, Rank r);
	int addPiece(Piece p, Square s);
	int addPiece(Piece p, File f, Rank r);
	int movePiece(Square from, Square to);
	void removePieceFromList(Colour c, PieceType pt, Square s);
	int removePiece(Square s);
	int removePiece(File f, Rank r);
	
	// Castling helper functions.
	void giveRight(Colour c, CastlingSide side);
	void removeRight(Colour c, CastlingSide side);
	void removeRights(Colour c);
	bool hasRight(Colour c, CastlingSide side);
	bool castlingNotBlocked(Colour c, CastlingSide side);
	bool castlingNotAttacked(Colour c, CastlingSide side);
	
	// Helpers for parsing a FEN.
	int FENtoBB(string FEN);
	vector<string> stringSplit(string str, char delim);
	vector<string> splitFEN(string FEN);
	Square getEPTarget(string epstr);
	CastlingRight getCastlingFromString(string castling);
	
	// Variables
	Colour toMove;
	CastlingRight castling;
	Square enPassantTarget;
	int movesSinceCapture;
	int moveNumber;
	Piece captured;
	
	
	// 
	Piece board[TOTAL_SQUARES];
	Bitboard pieceBB[TOTAL_PIECETYPES];
	Bitboard colourBB[TOTAL_COLOURS];
	int pieceCount[TOTAL_COLOURS][TOTAL_PIECETYPES];
	Square pieceList[TOTAL_COLOURS][TOTAL_PIECETYPES][16];
	
};




#endif // #ifndef POSITION_H_INCLUDED

