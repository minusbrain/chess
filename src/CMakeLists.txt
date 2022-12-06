set (SOURCE_CPP
   board.cpp
   piece_rules.cpp
   move.cpp
   rules.cpp
   chess_game.cpp
   chess_player.cpp
   board_factory.cpp
)

add_library(chess ${SOURCE_CPP})
target_link_libraries(chess PRIVATE fmt::fmt-header-only)

add_executable(run_chess main.cpp)
target_link_libraries(run_chess PRIVATE chess fmt::fmt-header-only)

add_subdirectory(test)