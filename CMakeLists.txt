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

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(ext/aelib)
include_directories(ext/imgui)
include_directories(ext/imgui/backends)
include_directories(ext/json/include)
include_directories(ext/fmt/include)
include_directories(ext/sml)

include_directories(src/lib)
include_directories(src/gui)

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(docs)

configure_file(./assets/pieces.json ./assets/pieces.json COPYONLY)
configure_file(./assets/pieces.png ./assets/pieces.png COPYONLY)
configure_file(./assets/marker.png ./assets/marker.png COPYONLY)

install(TARGETS chess DESTINATION lib)
install(TARGETS chess_gui DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)

add_custom_target(chess_meta DEPENDS chess run_chess test_chess chess_gui)