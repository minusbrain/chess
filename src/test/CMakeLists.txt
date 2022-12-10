find_package(GTest REQUIRED)

set (TEST_SOURCE_CPP
   main.cpp
   common.cpp
   test_board.cpp
   test_rules.cpp
   test_move.cpp
)

add_executable(test_chess ${TEST_SOURCE_CPP})
target_link_libraries(test_chess PRIVATE chess GTest::gtest fmt::fmt-header-only)

setup_target_for_coverage_gcovr_html(
   NAME test_chess_coverage          # New target name
   EXECUTABLE test_chess             # Executable in PROJECT_BINARY_DIR
   DEPENDENCIES test_chess
   BASE_DIRECTORY "${CMAKE_SOURCE_DIR}"
   EXCLUDE "ext/*"
)

add_test(Test_Chess test_chess)