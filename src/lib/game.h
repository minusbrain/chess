#pragma once
#include <base/helpers.h>

#include <memory>
#include <optional>

template <class TGameState, class TMove, class TExtraData>
struct GameState {
    using MyType = GameState<TGameState, TMove, TExtraData>;

    std::shared_ptr<TGameState> content = nullptr;
    std::shared_ptr<TMove> moveToNext = nullptr;
    std::shared_ptr<MyType> prev = nullptr;
    std::shared_ptr<MyType> next = nullptr;
    std::shared_ptr<TExtraData> data = nullptr;
};

template <class TGameState, class TMove, class TExtraData>
class GameStateIterator {
   public:
    using State = GameState<TGameState, TMove, TExtraData>;
    GameStateIterator(std::shared_ptr<State> curr) : _curr(curr) {}

    bool operator!=(const GameStateIterator& other) const { return !operator==(other); }
    bool operator==(const GameStateIterator& other) const { return _curr == other._curr; }
    GameStateIterator& operator++() {
        if (_curr != nullptr) _curr = _curr->next;
        return *this;
    }

    std::shared_ptr<State> operator*() { return _curr; }

   private:
    std::shared_ptr<State> _curr;
};

template <class TGameState, class TMove, class TExtraData>
class Game : public base::NONCOPYANDMOVEABLE {
   public:
    using MyType = Game<TGameState, TMove, TExtraData>;
    using State = GameState<TGameState, TMove, TExtraData>;
    using Iterator = GameStateIterator<TGameState, TMove, TExtraData>;

    Game() = delete;
    Game(const TGameState& rootState, const TExtraData& extraData) : _root(std::make_shared<State>()), _last(_root) {
        _root->content = std::make_shared<TGameState>(rootState);
        _root->data = std::make_shared<TExtraData>(extraData);
    }

    void addMove(const TMove& move, const TGameState& resultingState, const TExtraData& extraData) {
        std::shared_ptr<State> oldLast = _last;
        _last = std::make_shared<State>();
        _last->content = std::make_shared<TGameState>(resultingState);
        _last->data = std::make_shared<TExtraData>(extraData);
        _last->prev = oldLast;
        oldLast->next = _last;
        oldLast->moveToNext = std::make_shared<TMove>(move);
    }
    std::shared_ptr<State> getCurrentState() { return _last; }

    const Iterator begin() { return Iterator{_root}; }
    const Iterator end() { return Iterator{nullptr}; }

   private:
    std::shared_ptr<State> _root = nullptr;
    std::shared_ptr<State> _last = nullptr;
};
