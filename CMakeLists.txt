cmake_minimum_required(VERSION 3.20)
project(space_invaders VERSION 0.1.0 LANGUAGES C CXX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(space_invaders_core STATIC
  src/core/game_state.c
  src/core/controls.c
  src/core/consumables.c
  src/core/shot_profile.c
  src/core/enemy_profile.c
  src/core/enemy_abilities.c
  src/core/dash.c
  src/core/routes.c
  src/core/shop.c
  src/core/interest.c
  src/core/shop_reroll.c
  src/core/shop_hold.c
  src/core/module_slots.c
  src/core/module_sell.c
  src/core/ai.c
  src/core/shields.c
  src/core/random.c
  src/core/room.c
  src/core/room_tag.c
  src/core/modules.c
  src/core/boss.c
  src/core/bonus.c
  src/core/game_step.c)
target_include_directories(space_invaders_core PUBLIC include)
if(NOT MSVC)
  target_compile_options(space_invaders_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(space-invaders
  src/ui/gui.cpp
  src/ui/gui_draw.cpp
  src/ui/gui_input.cpp
  src/ui/gui_render.cpp
  src/ui/gui_overlay.cpp
  src/ui/gui_shop.cpp
  src/ui/save.cpp
  src/ui/main.cpp
  )
target_link_libraries(space-invaders PRIVATE space_invaders_core PkgConfig::SDL2 PkgConfig::SDL2_TTF)
include(GNUInstallDirs)
install(TARGETS space-invaders space_invaders_core
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

enable_testing()
if(MSVC)
  set(TEST_ASSERT_FLAG /UNDEBUG)
else()
  set(TEST_ASSERT_FLAG -UNDEBUG)
endif()
foreach(test_name gameplay room shop shop_session modules boss replay consumables routes overclock bounty room_tag banker enemy_abilities)
  add_executable(${test_name}-test test/${test_name}_test.c)
  target_link_libraries(${test_name}-test PRIVATE space_invaders_core)
  target_compile_options(${test_name}-test PRIVATE ${TEST_ASSERT_FLAG})
  if(NOT MSVC)
    target_compile_options(${test_name}-test PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  add_test(NAME ${test_name} COMMAND ${test_name}-test)
endforeach()
add_test(NAME gui-smoke COMMAND ${CMAKE_COMMAND} -E env SDL_VIDEODRIVER=dummy
  $<TARGET_FILE:space-invaders> --smoke 42)
set_tests_properties(gui-smoke PROPERTIES TIMEOUT 15)
add_executable(input-test test/input_test.cpp src/ui/gui_input.cpp)
target_compile_options(input-test PRIVATE ${TEST_ASSERT_FLAG})
target_link_libraries(input-test PRIVATE space_invaders_core PkgConfig::SDL2 PkgConfig::SDL2_TTF)
add_test(NAME input COMMAND ${CMAKE_COMMAND} -E env SDL_VIDEODRIVER=dummy
  $<TARGET_FILE:input-test>)
set_tests_properties(input PROPERTIES TIMEOUT 15)
add_executable(save-test test/save_test.cpp src/ui/save.cpp)
target_compile_options(save-test PRIVATE ${TEST_ASSERT_FLAG})
target_link_libraries(save-test PRIVATE space_invaders_core)
add_test(NAME save COMMAND save-test)
