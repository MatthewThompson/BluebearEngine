/* 
 * 
 * TYPES.H
 * 
 * This header is used to define a lot of the types used in the other code.
 * A lot of this code is from the Stockfish 6 open source code, which I used
 * to help me get started writing this engine.
 * 
 */


#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED


// Because writing unsigned long long int is a pain and uint64_t wouldn't work for me.
typedef unsigned long long int uint64_t;
/*
 * A bitboard is just a 64 bit unsigned int,
 * one bit for each square, format as follows :
 *
 *
       56 57 58 59 60 61 62 63 -> msb
       48 49 50 51 52 53 54 55
       40 41 42 43 44 45 46 47
       32 33 34 35 36 37 38 39
       24 25 26 27 28 29 30 31
       16 17 18 19 20 21 22 23
       8  9  10 11 12 13 14 15
lsb -> 0  1  2  3  4  5  6  7
 */
typedef uint64_t Bitboard;
/*
 * Only 16 bytes are needed to encode a move, 6 for the from square (0-63)
 * 6 bits for the to square, 2 bits for promotion piece (knight = 00, queen = 11)
 * and 2 bits for type.
 */
typedef unsigned short Move;

enum MoveType {
	NORMAL,
	PROMOTION = 1 << 14,
	ENPASSANT = 2 << 14,
	CASTLING  = 3 << 14
};

enum Colour {
	WHITE, BLACK, TOTAL_COLOURS, NO_COLOUR
};

enum Square {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	
	TOTAL_SQUARES,
	NO_SQUARE,
	
	NORTH   =  8,
	EAST    =  1,
	SOUTH   = -8,
	WEST    = -1,
	
	NORTH_NORTH = NORTH + NORTH,
	NORTH_EAST  = NORTH + EAST,
	SOUTH_EAST  = SOUTH + EAST,
	SOUTH_SOUTH = SOUTH + SOUTH,
	SOUTH_WEST  = SOUTH + WEST,
	NORTH_WEST  = NORTH + WEST
};

enum File {
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, TOTAL_FILES
};

enum Rank {
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, TOTAL_RANKS
};

enum PieceType {
	NO_PIECE_TYPE,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	ALL_PIECES = 0,
	TOTAL_PIECETYPES = 8,
	
};

enum Piece {
	NO_PIECE,
	
	WPAWN = 1,
	WKNIGHT,
	WBISHOP,
	WROOK,
	WQUEEN,
	WKING,
	
	BPAWN = 9,
	BKNIGHT,
	BBISHOP,
	BROOK,
	BQUEEN,
	BKING,
	
	TOTAL_PIECES = 16
};



enum CastlingSide {
	KING_SIDE, QUEEN_SIDE, TOTAL_CASTLING_SIDES = 2
};

enum CastlingRight {
  NO_CASTLING,
  WHITE_OO,
  WHITE_OOO = WHITE_OO << 1,
  BLACK_OO  = WHITE_OO << 2,
  BLACK_OOO = WHITE_OO << 3,
  ANY_CASTLING = WHITE_OO | WHITE_OOO | BLACK_OO | BLACK_OOO,
  TOTAL_CASTLING_RIGHTS = 16
};

const uint64_t one = 1;

/* 
 * Enabling base and full operators on a type allows us to use it
 * like an int by adding them together for example.
 */
#define ENABLE_BASE_OPERATORS_ON(T)                             \
inline T operator+(T d1, T d2) { return T(int(d1) + int(d2)); } \
inline T operator-(T d1, T d2) { return T(int(d1) - int(d2)); } \
inline T operator*(int i, T d) { return T(i * int(d)); }        \
inline T operator*(T d, int i) { return T(int(d) * i); }        \
inline T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, T d2) { return d1 = d1 + d2; }      \
inline T& operator-=(T& d1, T d2) { return d1 = d1 - d2; }      \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); }

#define ENABLE_FULL_OPERATORS_ON(T)                             \
ENABLE_BASE_OPERATORS_ON(T)                                     \
inline T& operator++(T& d) { return d = T(int(d) + 1); }        \
inline T& operator++(T& d, int) { return d = T(int(d) + 1); }        \
inline T& operator--(T& d) { return d = T(int(d) - 1); }        \
inline T& operator--(T& d, int) { return d = T(int(d) - 1); }        \
inline T operator/(T d, int i) { return T(int(d) / i); }        \
inline int operator/(T d1, T d2) { return int(d1) / int(d2); }  \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); }

ENABLE_FULL_OPERATORS_ON(PieceType)
ENABLE_FULL_OPERATORS_ON(Piece)
ENABLE_FULL_OPERATORS_ON(Colour)
ENABLE_FULL_OPERATORS_ON(Square)
ENABLE_FULL_OPERATORS_ON(File)
ENABLE_FULL_OPERATORS_ON(Rank)


#undef ENABLE_FULL_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON


/*
 * Various inline functions for useful operations on types.
 */

// Gets the opposite colour.
inline Colour operator~(Colour c) {
  return Colour(c ^ BLACK);
}

// Gets a castling right from a side and colour, e.g. white kingside = WHITE_OO.
inline CastlingRight operator|(Colour c, CastlingSide s) {
  return CastlingRight(WHITE_OO << ((s == QUEEN_SIDE) + 2 * c));
}

// Gets a square from a file and a rank.
inline Square getSquare(File f, Rank r) {
  return Square((r << 3) | f);
}

// Gets a piece from a colour and piece type.
inline Piece getPiece(Colour c, PieceType pt) {
  return Piece((c << 3) | pt);
}

// Gets the piece type of a piece.
inline PieceType getPieceType(Piece pc)  {
  return PieceType(pc & 7);
}

// Gets the colour of a piece.
inline Colour getPieceColour(Piece pc) {
  return Colour(pc >> 3);
}

// Checks if a square is valid.
inline bool isOk(Square s) {
  return s >= A1 && s <= H8;
}

// Gets the file of a square.
inline File getFile(Square s) {
  return File(s & 7);
}

// Gets the rank of a square.
inline Rank getRank(Square s) {
  return Rank(s >> 3);
}



#endif // #ifndef TYPES_H_INCLUDED

