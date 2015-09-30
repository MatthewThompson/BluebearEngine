BluebearEngine is a chess engine I wrote with inspiration
taken from the open source engine Stockfish 6.

Most of the types, and simple functions for the board and
position representation were taken from Stockfish, all the
evaluation and search amongst other things were written
myself.

This engine is a work in progress, to do list(in order of priority):
 - Fix the bug that makes it core dump when searching of a depth of usually 5 or more.
 - Add functionality to be able to play chess against it from the command line.
 - Make the search better than just brute force.
 - Improve the evaluation function.
 - Multithread it.