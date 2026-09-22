cmake_minimum_required(VERSION 3.20)
project(space_invaders VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(space_invaders_core STATIC
  src/core/game.c)
target_include_directories(space_invaders_core PUBLIC include)

add_executable(space-invaders src/ui/main.cpp)
target_link_libraries(space-invaders PRIVATE space_invaders_core)

enable_testing()
add_executable(space-invaders-test test/game_test.c)
target_link_libraries(space-invaders-test PRIVATE space_invaders_core)
add_test(NAME game COMMAND space-invaders-test)
