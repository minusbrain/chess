cmake_minimum_required (VERSION 3.13)

# Name of the project
project (chess)

enable_testing()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ext/cmake")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("Activating Code Coverage")
    set(CHESS_CODE_COVERAGE YES)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dangling-reference")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

if(DEFINED CHESS_CODE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CXX_FLAGS}" "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(ext/fmt EXCLUDE_FROM_ALL)
include_directories(chess PRIVATE ext/fmt/include)

add_subdirectory(ext/aelib)
include_directories(chess PRIVATE ext/aelib)

add_subdirectory(src)
add_subdirectory(docs)

add_custom_target(chess_meta DEPENDS chess run_chess test_chess)

install(TARGETS chess DESTINATION lib)
