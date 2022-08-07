find_package(GTest REQUIRED)

enable_testing()

set (TEST_SOURCE_CPP
   test_board.cpp
   test_rules.cpp
)

add_executable(test_chess ${TEST_SOURCE_CPP})
target_link_libraries(test_chess PRIVATE chess GTest::gtest_main fmt::fmt-header-only)
add_test(Test_Chess test_chess)