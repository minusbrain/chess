#include "chess_player.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <random>

#include "base/helpers.h"
#include "fmt/core.h"
#include "move_debug.h"

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

const Move& PickFirstChessPlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    return potentialMoves[0];
}

PickRandomChessPlayer::PickRandomChessPlayer(const std::string& name) : ChessPlayer(name) {}

const Move& PickRandomChessPlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
    assert(potentialMoves.size() > 0);
    return *select_randomly(potentialMoves.begin(), potentialMoves.end());
}

HumanChessPlayer::HumanChessPlayer(const std::string& name) : ChessPlayer(name) {}

const Move& HumanChessPlayer::getMove(const Board&, const std::vector<Move>& potentialMoves) {
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
