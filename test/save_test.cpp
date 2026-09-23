#include "space_invaders/save.h"
#include <cassert>
#include <cstdlib>
#include <fstream>

int main() {
  setenv("HOME", "/tmp/smolinvaders-save-test", 1);
  Game original;
  game_init_seed(&original, 7331);
  original.room = 7;
  original.score = 42;
  original.credits = 19;
  original.modules = MODULE_SIGNAL | MODULE_BARRIER;
  original.shields[1][3] = 1;
  original.flawless_streak = 2;
  assert(game_save(&original));

  Game loaded;
  game_init_seed(&loaded, 1);
  assert(game_load(&loaded));
  assert(loaded.seed == original.seed && loaded.room == original.room);
  assert(loaded.score == original.score && loaded.credits == original.credits);
  assert(loaded.modules == original.modules);
  assert(loaded.shields[1][3] == 1);
  assert(loaded.flawless_streak == original.flawless_streak);

  std::ofstream invalid("/tmp/smolinvaders-save-test/.ewanc26/smolinvaders/save/run.json",
                        std::ios::trunc);
  invalid << "{\"format\": 99}";
  invalid.close();
  Game preserved = loaded;
  assert(!game_load(&preserved));
  assert(preserved.seed == loaded.seed && preserved.score == loaded.score);
  return 0;
}
