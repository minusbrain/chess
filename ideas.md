# Ideas

* Unicode chess pieces for debug output
* Generic state / move tree for chess but also for other games. A node is a state, current board position. An edge is a move from one position to the next.
* Add support for UCI https://www.shredderchess.com/chess-features/uci-universal-chess-interface.html
    * Accept board states as UCI string
* Collect debug output in one file and move as much as possible into a source file
* Add support for https://en.wikipedia.org/wiki/Portable_Game_Notation
    * PGN SpecificationL: https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt
* https://justinfagnani.github.io/chessboard-element/examples/1002-fen/ <- Tools for FEN / HTML, ////