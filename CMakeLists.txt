cmake_minimum_required (VERSION 3.10)

# Name of the project
project (chess)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_MODULE_PATH "${CMAKE_CXX_FLAGS}" "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(ext/fmt EXCLUDE_FROM_ALL)
add_subdirectory(ext/aelib)

enable_testing()

set (SOURCE_CPP
   src/board.cpp
   src/move.cpp
   src/rules.cpp
   src/board_factory.cpp
)

set (TEST_SOURCE_CPP
   test/test_board.cpp
   test/test_rules.cpp
)

include_directories(chess PRIVATE ext/fmt/include)
include_directories(chess PRIVATE ext/aelib)

add_library(chess ${SOURCE_CPP})
target_link_libraries(chess PRIVATE fmt::fmt-header-only)

add_executable(run_chess src/main.cpp)
target_link_libraries(run_chess PRIVATE chess fmt::fmt-header-only)

add_executable(test_chess ${TEST_SOURCE_CPP})
target_link_libraries(test_chess PRIVATE chess GTest::gtest_main)

add_test(Test_Chess test_chess)

add_custom_target(chess_meta DEPENDS chess run_chess test_chess)

install(TARGETS chess DESTINATION bin)
