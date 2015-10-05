BluebearEngine is a chess engine I wrote with inspiration
taken from the open source engine Stockfish 6.

Most of the types, and simple functions for the board and
position representation were taken from Stockfish, all the
evaluation and search amongst other things were written
myself.

This engine is a work in progress, to do list(in order of priority):
 - Fix bug with castling not adding check flag O-O-O+
 - Move string methods to a util header.
 - Add simple and short opening book.
 - Make all the score stuff and piece values magic numbers.
 - Other code cleaning and commenting.
 - Make the search better than just brute force.
 - Improve the evaluation function.
 - Multithread it.
 - Add draw by repetition rule (Only keep track of positions since a pawn move
or capture (includeing that move), since after that all moves prior cannot occur again).
 - Learn how to make 64 bit executables...
 - Improve the makefile.


