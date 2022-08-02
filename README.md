# Chess Engine

This is going to be a simple chess engine allowing local matches between two humans, human vs CPU or CPU vs CPU.
There are two reasons for its creation. I wanna improve my understanding of chess and I wanna train my C++ skills
as my work doesn't allow me to do that currently.

# Usage

The project crates two binaries:
* libchess library -> The engine
* run_chess executable -> A simple executable that uses the library. See below for details of its function.

Currently this only takes a [FEN String](https://www.chess.com/terms/fen-chess#en-passant-targets) as the only input
and prints the board in ASCII style and shows the valid moves for the color whos turn it is currently.

Try it: 
```bash
./run_chess "k1p5/8/P1r1p3/8/8/2P1R2p/8/K3P3 b - -"
```

You can use a FEN String generator like [this](http://www.netreal.de/Forsyth-Edwards-Notation/index.php) to
generate the first part of the string. The other 3 parts are explained in the link above and have to be added
manually.

# How to build

## Prerequisites

* C++20 compatible compiler (might work with older standards, I did not check)
* cmake >= 3.10
* googletest installed

## Build

* Clone repo and sync / update submodules
* mkdir build && cd build
* cmake ..
* make

# Current status

This is work in progress

## What the library can do

* Create an empty chess board
* Create a chess board in its start configuration for a game
* Create a chess board from a FEN String
* Display a chess board in ASCII
* Represent chess moves (class Move)
* Apply a chess move to a board
* Display a chess move in Long Algebraic Notation
* Determine potential moves for a given board
* En Passant moves
* Promotion moves

## What the library can't do yet

* Play a chess game (neither for humans or CPUs)
* Support any kind of GUI
* Manipulate a board via command line
* Detect check-mate
* Castling moves
* Be properly tested  ;-)

# Licenses and external dependencies

* [This engine is published under MIT license](LICENSE)
* This engine depends on [aelib](https://github.com/minusbrain/aelib)
* This engine depends on [fmt](https://github.com/fmtlib/fmt)

# Ideas

[Ideas for future changes](ideas.md)