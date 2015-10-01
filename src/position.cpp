/* 
 * POSITION.CPP
 * 
 * Contains move functions, checking if they are legal, and finding
 * all moves in the current position.
 * 
 * 
 */
 
#include "types.h"
#include "bitboards.h"
#include "moves.h"
#include "position.h"


/* 
 * TODO : MAKE THIS A CLASS
 */
/* 
 * TODO : ADD 3 MOVE REPETITION RULE.
 */

/* 
 * Variables to keep track of in a position, that aren't piece locations.
 */
// The side to move.
Colour toMove;
// The castling rights.
CastlingRight castling;
// If there is a square which can be captures en passant.
Square enPassantTarget;
// How mane moves since a capture or a pawn push (for rule 50).
int movesSinceCapture;
// The move number.
int moveNumber;

/* 
 * Variables to keep track of piece locations.
 */
// An array of pieces, size 64, one for each square.
//Piece board[TOTAL_SQUARES];
// An array of piece bitboards, one for each piece type.
//Bitboard pieceBB[TOTAL_PIECETYPES];
// An array of colour bitboards, one for each colour.
//Bitboard colourBB[TOTAL_COLOURS];
// A 2D array, for each piecetype and colour, the number of that piece.
//int pieceCount[TOTAL_COLOURS][TOTAL_PIECETYPES];
// A 3D array, for each piecetype and colour, contains an array of square locations for that piece.
//Square pieceList[TOTAL_COLOURS][TOTAL_PIECETYPES][16];




/* 
 * Initialisation function for position, zeroes out most of the variables.
 */
void Position::initPosition(void) {
	
	toMove = WHITE;
	castling = NO_CASTLING;
	enPassantTarget = NO_SQUARE;
	movesSinceCapture = 0;
	moveNumber = 0;
	
	for (int piece = 0; piece < TOTAL_PIECETYPES; piece++) {
		pieceBB[piece] = 0;
	}
	
	for (int colour = 0; colour < TOTAL_COLOURS; colour++) {
		colourBB[colour] = 0;
		for (int piece = 0; piece < TOTAL_PIECETYPES; piece++) {
			pieceCount[colour][piece] = 0;
			for (int i = 0; i < 16; i++) {
				pieceList[colour][piece][i] = NO_SQUARE;
			}
		}
	}
	
	for (int square = 0; square < TOTAL_SQUARES; square++) {
		board[square] = NO_PIECE;
	}
}



/* 
 * Getter functions for the pieces.
 */
// Gets the bitboard of all pieces.
Bitboard Position::getPieces() {
	return colourBB[WHITE] | colourBB[BLACK];
}

// Gets the bitboard of all pieces of one colour.
Bitboard Position::getPieces(Colour c) {
	return colourBB[c];
}

// Gets the bitboard of all pieces of one piece type.
Bitboard Position::getPieces(PieceType pt) {
	return pieceBB[pt];
}

// Gets the bitboard of all pieces of one piece type and colour.
Bitboard Position::getPieces(Colour c, PieceType pt) {
	return pieceBB[pt] & colourBB[c];
}

// Gets the square that the king of a colour is on.
Square Position::getKingSquare(Colour c) {
	return pieceList[c][KING][0]; // Can only be one king.
}

// Gets the piece on a square.
Piece Position::pieceAt(Square s) {
	return board[s];
}

// Returns if a square is empty.
bool Position::isEmpty(Square s) {
	return pieceAt(s) == NO_PIECE;
}

// Returns the number of a piece.
int Position::getPieceCount(Colour c, PieceType pt) {
	return pieceCount[c][pt];
}

/* 
 * Getters.
 */

// Gets the colour it is to move.
Colour Position::getToMove() {
	return toMove;
}

// Gets the en passant target square.
Square Position::getEPTarget() {
	return enPassantTarget;
}

//
void Position::setEPTarget(Square s) {
	enPassantTarget = s;
}

int Position::getMoveNumber() {
	return moveNumber;
}


/* 
 * Functions for adding, moving and removing pieces from a position.
 */
// Adds a piece to a square.
int Position::addPiece(Colour c, PieceType pt, Square s) {
	if (board[s] != NO_PIECE) {
		return 0;
	}
	pieceBB[pt] = addToBB(pieceBB[pt], s);
	colourBB[c] = addToBB(colourBB[c], s);
	pieceList[c][pt][pieceCount[c][pt]] = s;
	pieceCount[c][pt]++;
	board[s] = getPiece(c, pt);
	
	return 1;
}

// Adds a piece to a square (for a given file and rank).
int Position::addPiece(Colour c, PieceType pt, File f, Rank r) {
	addPiece(c, pt, getSquare(f, r));
}

// Adds a piece to a square.
int Position::addPiece(Piece p, Square s) {
	addPiece(getPieceColour(p), getPieceType(p), s);
}

// Adds a piece to a square (for a given file and rank).
int Position::addPiece(Piece p, File f, Rank r) {
	addPiece(p, getSquare(f, r));
}

// Moves a piece from a square to another (the destination must be empty).
int Position::movePiece(Square from, Square to) {
	if (pieceAt(from) == NO_PIECE) {
		return 0;
	}
	if (pieceAt(to) != NO_PIECE) {
		return 0;
	}
	
	Piece p = pieceAt(from);
	PieceType pt = getPieceType(p);
	Colour c = getPieceColour(p);
	
	pieceBB[pt] = removeFromBB(pieceBB[pt], from);
	colourBB[c] = removeFromBB(colourBB[c], from);
	
	pieceBB[pt] = addToBB(pieceBB[pt], to);
	colourBB[c] = addToBB(colourBB[c], to);
	
	
	
	for (int i = 0; i < getPieceCount(c, pt); i++) {
		
		if (pieceList[c][pt][i] == from) {
			// Update the piece list.
			pieceList[c][pt][i] = to;
			
		}
		
	}
	
	board[from] = NO_PIECE;
	board[to] = p;
	
	return 1;
}

// Helper function used by remove piece, removes it from the piece list, and shifts all further indexes back one.
void Position::removePieceFromList(Colour c, PieceType pt, Square s) {
	
	for (int i = 0; i < pieceCount[c][pt]; i++) {
		
		if (pieceList[c][pt][i] == s) {
			
			for (int j = 0; j < pieceCount[c][pt] - i - 1; j++) {
				
				pieceList[c][pt][i + j] = pieceList[c][pt][i + j + 1];
				
			}
			
			pieceList[c][pt][pieceCount[c][pt]] = NO_SQUARE;
			
			return;
			
		}
		
	}
	
}

// Removes the piece that is on a given square.
int Position::removePiece(Square s) {
	if (board[s] == NO_PIECE) {
		return 0;
	}
	
	Piece p = board[s];
	PieceType pt = getPieceType(p);
	Colour c = getPieceColour(p);
	
	pieceBB[pt] = removeFromBB(pieceBB[pt], s);
	colourBB[c] = removeFromBB(colourBB[c], s);
	
	removePieceFromList(c, pt, s);
	
	pieceCount[c][pt]--;
	board[s] = NO_PIECE;
	
	return 1;
}

// Removes the piece that is on a given file and rank.
int Position::removePiece(File f, Rank r) {
	removePiece(getSquare(f, r));
}



/*
 * Functions for doing, temporarily doing, and undoing moves.
 */

/* 
 * Do a move, keep track of relevant data like rule 50, ep target square etc.
 */
void Position::doMove(Move m) {
	
	// Get the information from the move.
	MoveType mt = getMoveType(m);
	Square to = getTo(m);
	Square from = getFrom(m);
	
	Colour us = getPieceColour(pieceAt(from));
	PieceType type = getPieceType(pieceAt(from));
	
	// Increment the rule 50 (and set it to 0 later if needed).
	movesSinceCapture++;
	enPassantTarget = NO_SQUARE;
	
	if (mt == PROMOTION) {
		removePiece(from);
		addPiece(us, getPromotionPiece(m), from); // Move it later.
	}
	
	if (type == KING) { // If the king moves remove all castling rights.
		removeRights(us);
	}
	
	if (type == ROOK) {
		// If a rook moves and it is on a start square, then remove the rights for that side,
		// even if it is the wrong rook, it means that the rook that started there has moved anyway
		// so removing the rights again won't change anything.
		
		Rank relativeFirstRank = us == WHITE ? RANK_1 : RANK_8;
		if (getFile(from) == FILE_A && getRank(from) == relativeFirstRank) {
			removeRight(us, QUEEN_SIDE);
		}
		
		if (getFile(from) == FILE_H && getRank(from) == relativeFirstRank) {
			removeRight(us, KING_SIDE);
		}
	}
	
	if (isCapture(m)) {
		
		Square capturedSquare = to;
		
		if (mt == ENPASSANT) {
			capturedSquare = to + (us == WHITE ? SOUTH : NORTH);
		}
		
		removePiece(capturedSquare); // Remove the piece that was captured.
		
	} else { // No capture.
		
		if (type == PAWN) {
			
			if (to == from + (2 * forward(us))) { // Pawn double push, add ep target.
				enPassantTarget = from + forward(us);
			}
			
		} else { // Non pawn move or capture, so reset 50 move rule.
			movesSinceCapture = 0;
		}
	}
	
	if (mt == CASTLING) {
		
		Rank rank1st  = us == WHITE ? RANK_1 : RANK_8;
		File rookFileTo   = (to > from) ? FILE_F : FILE_D;
		File rookFileFrom = (to > from) ? FILE_H : FILE_A;
		
		Square rookFrom = getSquare(rookFileFrom, rank1st);
		Square rookTo = getSquare(rookFileTo, rank1st);
		
		// For castling, move the rook and remove all castling rights.
		movePiece(rookFrom, rookTo);
		removeRights(us);
	}
	
	// Move the piece.
	movePiece(from, to);
	
	// Change sides.
	toMove = ~toMove;
	if (toMove == WHITE) { // If it is now white to move again, it is the next move.
		moveNumber++;
	}
}

// Used to temp do a move.
PieceType recentCapture = NO_PIECE_TYPE;

/*
 * Do a move to check the legality, must be undone afterwards as
 * it specifically does not keep track of rule 50, ep target and castling rights.
 */
void Position::tempDoMove(Move m) {
	
	MoveType mt = getMoveType(m);
	Square to = getTo(m);
	Square from = getFrom(m);
	
	Colour us = getPieceColour(pieceAt(from));
	PieceType type = getPieceType(pieceAt(from));
	
	Square temp = getEPTarget();
	
	if (mt == PROMOTION) {
		removePiece(from);
		addPiece(us, getPromotionPiece(m), from); // Move it later.
	}
	
	if (isCapture(m)) {
		
		
		
		recentCapture = getPieceType(pieceAt(to));
		Square capturedSquare = to;
		
		if (mt == ENPASSANT) {
			
			recentCapture = PAWN;
			
			capturedSquare = to - forward(us);
			
		}
		
		removePiece(capturedSquare);
		
	} else {
		
		recentCapture = NO_PIECE_TYPE;
		
	}
	
	if (mt == CASTLING) {
		Rank rank1st  = us == WHITE ? RANK_1 : RANK_8;
		File rookFileTo   = (to > from) ? FILE_F : FILE_D;
		File rookFileFrom = (to > from) ? FILE_H : FILE_A;
		
		Square rookFrom = getSquare(rookFileFrom, rank1st);
		Square rookTo = getSquare(rookFileTo, rank1st);
	}
	
	// Move the piece.
	movePiece(from, to);
	
}

// Undo a temporary move.
void Position::undoTempMove(Move m) {
	MoveType mt = getMoveType(m);
	Square to = getTo(m);
	Square from = getFrom(m);
	
	Colour us = getPieceColour(pieceAt(to));
	
	
	//
	movePiece(to, from);
	
	
	if (mt == CASTLING) {
		
		Rank rank1st  = us == WHITE ? RANK_1 : RANK_8;
		File rookFileTo   = (to > from) ? FILE_F : FILE_D;
		File rookFileFrom = (to > from) ? FILE_H : FILE_A;
		
		Square rookFrom = getSquare(rookFileFrom, rank1st);
		Square rookTo = getSquare(rookFileTo, rank1st);
		
		movePiece(rookTo, rookFrom);
	}
	
	if (recentCapture) { //
		
		Square capturedSquare = to;
		
		if (mt == ENPASSANT) {
			
			capturedSquare = to - forward(us);
			
		}
		
		addPiece(~us, recentCapture, capturedSquare);
		
	}
	
	if (mt == PROMOTION) {
		removePiece(from);
		addPiece(us, PAWN, from);
	}
	
	recentCapture = NO_PIECE_TYPE;
	
}


/* 
 * Castling helper functions.
 */
// Adds a castling right for a colour and side.
void Position::giveRight(Colour c, CastlingSide side) {
	CastlingRight r = (c | side);
	castling = CastlingRight(castling | (c | side));
}

// Removes a castling right for a colour and side.
void Position::removeRight(Colour c, CastlingSide side) {
	castling = CastlingRight(castling & ~(c | side));
}

// Removes all castling rights for a colour.
void Position::removeRights(Colour c) {
	castling = CastlingRight(castling & ~(c | KING_SIDE) & ~(c | QUEEN_SIDE));
}

// Returns if a colour has a castling right for a side.
bool Position::hasRight(Colour c, CastlingSide side) {
	return castling & (c | side);
}

// Returns if a colour for a side, can castle that side.
bool Position::canCastle(Colour c, CastlingSide side) {
	return hasRight(c, side) && castlingNotBlocked(c, side) && castlingNotAttacked(c, side);
}

// Checks if a castling path is unblocked.
bool Position::castlingNotBlocked(Colour c, CastlingSide side) {
	bool pathClear = true;
	
	Square kingSquare = getSquare(FILE_E, c == WHITE ? RANK_1 : RANK_8);
	Square direction  = getDirection(side);
	int distance      = side == KING_SIDE ? 3    : 4;
	Square rookSquare = kingSquare + (distance * direction);
	
	Square s = kingSquare;
	for (int i = 1; i < distance; i++) {
		s += direction;
		pathClear = pathClear && isEmpty(s);
	}
	return pathClear;
}

// Checks if the path a king must go to castle is attacked or not.
bool Position::castlingNotAttacked(Colour c, CastlingSide side) {
	bool pathUnattacked = true;
	
	Square kingSquare = getSquare(FILE_E, c == WHITE ? RANK_1 : RANK_8);
	Square direction  = getDirection(side);
	
	Square s;
	for (int i = 0; i < 3; i++) {
		s = kingSquare + (direction * i);
		pathUnattacked = pathUnattacked && !getAttackersTo(s, ~c);
	}
	
	return pathUnattacked;
}


/*
 * Checks if a colour is in check.
 */
bool Position::isInCheck(Colour c) {
	return getAttackersTo(getKingSquare(c), ~c);
}

/*
 * Checks if a colour is in checkmate.
 */
bool Position::isInCheckMate(Colour c) {
	return isInCheck(c) && (getLegalMoves(*this, c).size() == 0);
}

/* 
 * 
 */
bool Position::isCheckMate() {
	return isInCheck(toMove) && (getLegalMoves(*this).size() == 0);
}

/* 
 * True if a position is drawn by 50 move rule or stalemate
 * TODO : 3 move repetition.
 */
bool Position::isDraw() {
	return (movesSinceCapture >= 100) || isStaleMate();
}


/*
 * Checks if a colour is in stalemate.
 */
bool Position::isInStaleMate(Colour c) {
	return !isInCheck(c) && (getLegalMoves(*this, c).size() == 0);
}

/* 
 * 
 */
bool Position::isStaleMate() {
	return !isInCheck(toMove) && (getLegalMoves(*this).size() == 0);
}

// Checks if a move is a capture by seeing if there is a piece on the to square, or it's enpassant.
bool Position::isCapture(Move m) {
	return getMoveType(m) == ENPASSANT || pieceAt(getTo(m));
}


/* 
 * Checks if a move is legal by temporarily doing the move
 * and seeing if the side just moved is in check. (This isn't
 * the best way to do this).
 */
bool Position::isLegal(Move m) {
	tempDoMove(m);
	
	bool legal = !isInCheck(toMove); // Temp move doesn't change the colour.
	undoTempMove(m);
	
	return legal;
}

/* 
 * Checks if a move is legal by temporarily doing the move
 * and seeing if the side just moved is in check. (This isn't
 * the best way to do this).
 */
bool Position::isLegal(Move m, Colour c) {
	
	tempDoMove(m);
	
	bool legal = !isInCheck(c); // Temp move doesn't change the colour.
	undoTempMove(m);
	
	return legal;
}


// True if a move in this position would be check.
bool Position::isMoveCheck(Move m) {
	tempDoMove(m);
	
	bool check = isInCheck(~toMove);
	undoTempMove(m);
	
	return check;
}

// True if a move in this position would be checkmate.
bool Position::isMoveCheckmate(Move m) {
	tempDoMove(m);
	
	bool checkMate = isInCheckMate(~toMove);
	undoTempMove(m);
	
	return checkMate;
}



/* 
 * 
 * FUNCTIONS FOR GETTING THE ATTACKERS TO A SQUARE.
 * 
 */

/* 
 * Gets all of the pawns of a colour that are attacking a given square.
 */
Bitboard Position::getPawnAttackersTo(Square s, Colour c) {
	
	Square backLeft  = c == WHITE ? SOUTH_WEST : NORTH_EAST;
	Square backRight = c == WHITE ? SOUTH_EAST : NORTH_WEST;
	
	return (shiftBB(getBB(s), backLeft) | shiftBB(getBB(s), backRight)) & getPieces(c, PAWN);
	
}

/* 
 * Gets all of the knights that are attacking a given square.
 */
Bitboard Position::getKnightAttackersTo(Square s) {
	
	Bitboard attackers = 0;
	Bitboard knight = getBB(s);
	Bitboard temp;
	
	for (int i = 0; i < 8; i++) {
		temp = shiftBB(shiftBB(knight, KNIGHT_DIRECTIONS[i][0]), KNIGHT_DIRECTIONS[i][1]);
		attackers |= temp;
	}
	
	return removeFromBB(attackers, s) & getPieces(KNIGHT);
	
}

/* 
 * Gets all of the bishops that are attacking a given square.
 */
Bitboard Position::getBishopAttackersTo(Square s) {
	
	Bitboard attackers = 0;
	Bitboard temp;
	
	for (int i = 0; i < 4; i++) {
		temp = getBB(s);
		while (temp) {
			temp = shiftBB(temp, BISHOP_DIRECTIONS[i]);
			attackers |= temp;
			
			temp &= ~getPieces();
		}
	}
	
	return attackers & getPieces(BISHOP);
	
}

/* 
 * Gets all of the rooks that are attacking a given square.
 */
Bitboard Position::getRookAttackersTo(Square s) {
	
	Bitboard attackers = 0;
	Bitboard temp;
	
	for (int i = 0; i < 4; i++) {
		temp = getBB(s);
		while (temp) {
			temp = shiftBB(temp, ROOK_DIRECTIONS[i]);
			attackers |= temp;
			
			temp &= ~getPieces();
		}
	}
	
	return attackers & getPieces(ROOK);
	
}

/* 
 * Gets all of the queens that are attacking a given square.
 */
Bitboard Position::getQueenAttackersTo(Square s) {
	
	Bitboard attackers = 0;
	Bitboard temp;
	
	for (int i = 0; i < 8; i++) {
		temp = getBB(s);
		while (temp) {
			temp = shiftBB(temp, QUEEN_DIRECTIONS[i]);
			attackers |= temp;
			
			temp &= ~getPieces();
		}
	}
	
	return attackers & getPieces(QUEEN);
	
}

/* 
 * Gets all of the kings that are attacking a given square.
 */
Bitboard Position::getKingAttackersTo(Square s) {
	
	Bitboard square = getBB(s);
	Bitboard attackers = 0;
	
	for (int i = 0; i < 8; i++) {
		
		attackers |= shiftBB(square, KING_DIRECTIONS[i]);
		
	}
	
	return attackers & getPieces(KING);
	
}

/* 
 * Gets all of the attackers to a given square of a certain type.
 */
Bitboard Position::getAttackersTo(Square s, PieceType pt) {
	
	return
	pt == PAWN   ? getPawnAttackersTo(s, WHITE) | getPawnAttackersTo(s, BLACK) :
	pt == KNIGHT ? getKnightAttackersTo(s) :
	pt == BISHOP ? getBishopAttackersTo(s) :
	pt == ROOK   ? getRookAttackersTo(s) :
	pt == QUEEN  ? getQueenAttackersTo(s) :
	pt == KING   ? getKingAttackersTo(s) :
	0;
	
}

/*
 * Gets all of the attackers to a given square.
 */
Bitboard Position::getAttackersTo(Square s) {
	
	Bitboard attackers = 0;
	
	for (PieceType pt = PAWN; pt <= KING; pt++) {
		
		attackers |= getAttackersTo(s, pt);
		
	}
	
	return attackers;
}

/*
 * Gets all of the attackers to a given square of a certain colour.
 */
Bitboard Position::getAttackersTo(Square s, Colour c) {
	return getAttackersTo(s) & getPieces(c);
}

/*
 * Gets all of the attackers to a given square of a certain colour and piece type.
 */
Bitboard Position::getAttackersTo(Square s, Colour c, PieceType pt) {
	return getAttackersTo(s, pt) & getPieces(c);
}



/*
 *
 * STRING + FEN UTIL FUNCTIONS
 *
 */


/* 
 * Prints a board from perspective of a given colour.
 */
void Position::drawBoard(Colour c) {
	c == WHITE ? drawBoard() : drawFlipBoard();
}

/* 
 * Prints a board.
 */
void Position::drawBoard(void) {
	
	string line = " +---+---+---+---+---+---+---+---+";
	
	printf("%s\n", line.c_str());
	
	
	Piece p;
	for (Rank r = RANK_8; r >= RANK_1; r--) {
		
		printf("%u|", r + 1);
		
		for (File f = FILE_A; f <= FILE_H; f++) {
			
			p = board[getSquare(f, r)];
			
			printf(" %c |", getCharFromPiece(p));
			
		}
		
		printf("\n");
		printf("%s\n", line.c_str());
		
		
	}
	
	printf("   A   B   C   D   E   F   G   H\n");
	
}


/* 
 * Prints a board from perspective of black.
 */
void Position::drawFlipBoard(void) {
	
	string line = " +---+---+---+---+---+---+---+---+";
	
	printf("%s\n", line.c_str());
	
	
	Piece p;
	for (Rank r = RANK_1; r <= RANK_8; r++) {
		
		printf("%u|", r + 1);
		
		for (File f = FILE_H; f >= FILE_A; f--) {
			
			p = board[getSquare(f, r)];
			
			printf(" %c |", getCharFromPiece(p));
			
		}
		
		printf("\n");
		printf("%s\n", line.c_str());
		
		
	}
	
	printf("   H   G   F   E   D   C   B   A\n");
	
}



/* 
 * Splits a string up with a delimiter.
 */
vector<string> Position::stringSplit(string str, char delim) {
	
	vector<string> tokens; // To hold the substrings.
	int startIndex = 0; // To hold the starting index of each new substring.
	string temp; // To hold the current substring found.
	char current; // The current character.
	for (int i = 0; i < str.size(); i++) {
		
		current = str.at(i);
		
		if (current == delim) {
			if (i > startIndex) {
				temp = str.substr(startIndex, (i - startIndex));
				tokens.push_back(temp);
			}
			startIndex = i + 1;
		}
		
	}
	
	
	if (str.at(str.size() - 1) != delim) {
		temp = str.substr(startIndex);
		tokens.push_back(temp);
	}
	
	
	return tokens;
}



/*
 * Splits a FEN into the 6 parts it contains : 
 * 1: Board position.
 * 2: Active colour.
 * 3: Castling availability.
 * 4: En passant target square.
 * 5: Halfmoves since pawn move or piece capture.
 * 6: Fullmove number, starting at 1, incrementing after black's move.
 */
vector<string> Position::splitFEN(string FEN) {
	return stringSplit(FEN, ' ');
}

int Position::parseFEN(string FEN) {
	
	vector<string> FENparts = splitFEN(FEN);
	
	vector<string>::iterator it = FENparts.begin();
	FENtoBB(*it); it++;
	
	
	string toMoveStr = *it; it++;
	toMove = toMoveStr == "w" ? WHITE : BLACK;
	
	string castlingStr = *it; it++;
	castling = getCastlingFromString(castlingStr);
	
	string enPassantTargetStr = *it; it++;
	enPassantTarget = getEPTarget(enPassantTargetStr);
	
	string movesSinceCaptureStr = *it; it++;
	movesSinceCapture = atoi(movesSinceCaptureStr.c_str());
	
	//
	string moveNumberStr = *it; it++;
	moveNumber = atoi(moveNumberStr.c_str());
	
	
	return 1;
	
}


/* 
 * Taking just the position part of the FEN, it fills up all the relevant
 * position variables.
 */
int Position::FENtoBB(string FEN) {
	
	File file = FILE_A;
	Rank rank = RANK_8;
	
	for (int i = 0; i < FEN.size(); i++) {
		
		char current = FEN.at(i);
		
		if (isalpha(current)) {
			
			addPiece(getPieceFromChar(current), file, rank);
			file++;
			
		} else if (isdigit(current)) {
			
			file += File(current - '0');
			
		} else if (current == '/') {
			
			file = FILE_A;
			rank--;
			
		} else {
		
			return 0;
			
		}
		
	}
	return 1;
}

/* 
 * Gets the ep target square from a string.
 */
Square Position::getEPTarget(string epstr) {
	
	if (epstr.at(0) == '-') {
		return NO_SQUARE;
	}
	
	File file = File(epstr.at(0) - 'a');
	Rank rank = Rank(epstr.at(1) - '0' - 1);
	
	return getSquare(file, rank);
	
}

/* 
 * Gets all the castling rights from a string.
 */
CastlingRight Position::getCastlingFromString(string castling) {
	
	CastlingRight rights = NO_CASTLING;
	Colour colour;
	CastlingSide side;
	
	char c;
	for (int i = 0; i < castling.size(); i++) {
		c = castling.at(i);
		
		colour = islower(castling.at(i)) ? BLACK : WHITE;
		c = toupper(c);
		if (c == 'K') {
			rights = CastlingRight(rights | (colour | KING_SIDE));
		} else if (c == 'Q') {
			rights = CastlingRight(rights | (colour | QUEEN_SIDE));
		}
	}
	
	return rights;
	
}

/* 
 * Gets a piece from a character.
 */
Piece Position::getPieceFromChar(char c) {
	if (!isalpha(c)) {
		return NO_PIECE;
	}
	
	Colour colour;
	
	if (isupper(c)) {
		c = tolower(c);
		colour = WHITE;
	} else {
		colour = BLACK;
	}
	
	switch (c) {
		case 'p':
			return getPiece(colour, PAWN);
		case 'n':
			return getPiece(colour, KNIGHT);
		case 'b':
			return getPiece(colour, BISHOP);
		case 'r':
			return getPiece(colour, ROOK);
		case 'q':
			return getPiece(colour, QUEEN);
		case 'k':
			return getPiece(colour, KING);
		default:
			return NO_PIECE;
	}
}

/* 
 * Gets the string for a piece type.
 */
string Position::getStringFromPieceType(PieceType pt) {
	
	switch(pt) {
		case PAWN:
			return "P";
		case KNIGHT:
			return "N";
		case BISHOP:
			return "B";
		case ROOK:
			return "R";
		case QUEEN:
			return "Q";
		case KING:
			return "K";
		default:
			return "X";
	}
	
}

/* 
 * Gets the char value for a piece.
 */
char Position::getCharFromPiece(Piece p) {
	if (p == NO_PIECE) {
		return ' ';
	}
	
	PieceType pt = getPieceType(p);
	Colour colour = getPieceColour(p);
	char c;
	
	
	switch(pt) {
		case PAWN:
			c = 'p';
			break;
		case KNIGHT:
			c = 'n';
			break;
		case BISHOP:
			c = 'b';
			break;
		case ROOK:
			c = 'r';
			break;
		case QUEEN:
			c = 'q';
			break;
		case KING:
			c = 'k';
			break;
	}
	
	return (colour == WHITE ? toupper(c) : c);
}

/* 
 * Gets the string for a file.
 */
string Position::getFileStr(File f) {
	switch (f) {
		case FILE_A: return "a";
		case FILE_B: return "b";
		case FILE_C: return "c";
		case FILE_D: return "d";
		case FILE_E: return "e";
		case FILE_F: return "f";
		case FILE_G: return "g";
		case FILE_H: return "h";
		default: return 0;
	}
}

/* 
 * Gets the string for a rank.
 */
string Position::getRankStr(Rank r) {
	switch (r) {
		case RANK_1: return "1";
		case RANK_2: return "2";
		case RANK_3: return "3";
		case RANK_4: return "4";
		case RANK_5: return "5";
		case RANK_6: return "6";
		case RANK_7: return "7";
		case RANK_8: return "8";
		default: return 0;
	}
}

/* 
 * Gets the string for a square.
 */
string Position::getSquareStr(Square s) {
	string file = getFileStr(getFile(s));
	string rank = getRankStr(getRank(s));
	return file.append(rank);
}

/* 
 * Gets the file from a char.
 */
File Position::getFileFromChar(char c) {
	switch (c) {
		case 'a': return FILE_A;
		case 'b': return FILE_B;
		case 'c': return FILE_C;
		case 'd': return FILE_D;
		case 'e': return FILE_E;
		case 'f': return FILE_F;
		case 'g': return FILE_G;
		case 'h': return FILE_H;
		default: return FILE_A;
	}
}

/* 
 * Gets the rank from a char.
 */
Rank Position::getRankFromChar(char c) {
	switch (c) {
		case '1': return RANK_1;
		case '2': return RANK_2;
		case '3': return RANK_3;
		case '4': return RANK_4;
		case '5': return RANK_5;
		case '6': return RANK_6;
		case '7': return RANK_7;
		case '8': return RANK_8;
		default: return RANK_1;
	}
}

/* 
 * Gets the square from a string.
 */
Square Position::getSquareFromStr(string str) {
	if (str.size() != 2) {
		return NO_SQUARE;
	}
	File f = getFileFromChar(str.at(0));
	Rank r = getRankFromChar(str.at(1));
	return getSquare(f, r);
}



















