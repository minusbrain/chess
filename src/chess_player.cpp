#include "chess_player.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>

#include "ai_helper.h"
#include "base/helpers.h"
#include "fmt/core.h"
#include "move_debug.h"
#include "rules.h"

template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template <typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device r;
    std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()};
    static std::default_random_engine gen(seed2);
    return select_randomly(start, end, gen);
}

ChessPlayer::ChessPlayer(const std::string& name) : _name(name) {}

const std::string& ChessPlayer::getName() const { return _name; }

PickFirstChessPlayer::PickFirstChessPlayer(const std::string& name) : ChessPlayer(name) {}

Move PickFirstChessPlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    return potentialMoves[0];
}

PickRandomChessPlayer::PickRandomChessPlayer(const std::string& name) : ChessPlayer(name) {}

Move PickRandomChessPlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    return *select_randomly(potentialMoves.begin(), potentialMoves.end());
}

OneMoveDeepBestPositionChessPlayer::OneMoveDeepBestPositionChessPlayer(const std::string& name) : ChessPlayer(name) {}

Move OneMoveDeepBestPositionChessPlayer::getMove(const Board& board, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    std::vector<int32_t> ratingsInMyFavor;
    Color myColor = board.whosTurnIsIt();

    for (const Move& move : potentialMoves) {
        Board resultingBoard(board);
        ChessRules::applyMove(resultingBoard, move);
        auto rating = getPositionRating(resultingBoard);
        ratingsInMyFavor.push_back(myColor == Color::WHITE ? rating.white_pieces - rating.black_pieces
                                                           : rating.black_pieces - rating.white_pieces);
    }
    auto maxIt = std::max_element(ratingsInMyFavor.begin(), ratingsInMyFavor.end());
    return potentialMoves[std::distance(ratingsInMyFavor.begin(), maxIt)];
}

HumanConsolePlayer::HumanConsolePlayer(const std::string& name) : ChessPlayer(name) {}

Move HumanConsolePlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    int i = 0;
    for (auto& move : potentialMoves) {
        ++i;
        fmt::print("{:2} - {} |", i, move);
    }
    int pick = 0;
    while (pick < 1 || pick > (int)potentialMoves.size()) {
        fmt::print("\nPick move\n");
        std::cin >> pick;
    }
    return potentialMoves[pick - 1];
}
