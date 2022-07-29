cmake_minimum_required (VERSION 3.23)

# Name of the project
project (chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_MODULE_PATH "${CMAKE_CXX_FLAGS}" "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(ext/fmt EXCLUDE_FROM_ALL)
add_subdirectory(ext/aelib)

set (SOURCE_CPP
   src/main.cpp

   src/board.cpp
   src/move.cpp
   src/board_factory.cpp
)

add_executable(chess ${SOURCE_CPP} ${SOURCE_H})
target_include_directories(chess PRIVATE ext/fmt/include)
target_include_directories(chess PRIVATE ext/aelib)
target_link_libraries(chess PRIVATE fmt::fmt-header-only)

install(TARGETS chess DESTINATION bin)
