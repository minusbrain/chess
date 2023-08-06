#include <sml.hpp>

namespace cgsm {
namespace sml = boost::sml;

struct Idle {};
struct Running {};
struct WhitesTurn {};
struct BlacksTurn {};
struct Finished {};

struct EvStarted {};
struct EvWhiteMoved {};
struct EvBlackMoved {};
struct EvWhiteWon {};
struct EvBlackWon {};
struct EvNewGame {};

auto evStarted = sml::event<EvStarted>;
auto evWhiteMoved = sml::event<EvWhiteMoved>;
auto evBlackMoved = sml::event<EvBlackMoved>;
auto evWhiteWon = sml::event<EvWhiteWon>;
auto evBlackWon = sml::event<EvBlackWon>;
auto evNewGame = sml::event<EvNewGame>;

struct hello_world {
    auto operator()() const {
        using namespace sml;
        // clang-format off
    return make_transition_table(
      *state<Idle> + evStarted / send_fin = "fin wait 1"_s,
       "fin wait 1"_s + event<ack> [ is_ack_valid ] = "fin wait 2"_s,
       "fin wait 2"_s + event<fin> [ is_fin_valid ] / send_ack = "timed wait"_s,
       "timed wait"_s + event<timeout> / send_ack = X
    );
        // clang-format on
    }
};

}  // namespace cgsm