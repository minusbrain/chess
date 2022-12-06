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
add_test(Test_Chess test_chess)