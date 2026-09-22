cmake_minimum_required(VERSION 3.20)
project(space_invaders VERSION 0.1.0 LANGUAGES C CXX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(space_invaders_core STATIC
  src/core/game_state.c
  src/core/game_step.c)
target_include_directories(space_invaders_core PUBLIC include)

add_executable(space-invaders
  src/ui/gui.cpp
  src/ui/gui_input.cpp
  src/ui/gui_render.cpp
  src/ui/main.cpp
  )
target_link_libraries(space-invaders PRIVATE space_invaders_core PkgConfig::SDL2)

enable_testing()
add_executable(space-invaders-test test/game_test.c)
target_link_libraries(space-invaders-test PRIVATE space_invaders_core)
add_test(NAME game COMMAND space-invaders-test)
