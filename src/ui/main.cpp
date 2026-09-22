#include "space_invaders/gui.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

int main(int argc, char **argv) {
  const bool smoke = argc > 1 && std::strcmp(argv[1], "--smoke") == 0;
  const char *seed_arg = argc > (smoke ? 2 : 1) ? argv[smoke ? 2 : 1] : nullptr;
  uint32_t seed = 0xC0FFEEu;
  if (seed_arg) {
    if (*seed_arg == '-') {
      std::fprintf(stderr, "Seed must be an unsigned 32-bit integer.\n");
      return 2;
    }
    char *end = nullptr;
    errno = 0;
    unsigned long value = std::strtoul(seed_arg, &end, 10);
    if (errno || end == seed_arg || *end ||
        value > std::numeric_limits<uint32_t>::max()) {
      std::fprintf(stderr, "Seed must be an unsigned 32-bit integer.\n");
      return 2;
    }
    seed = static_cast<uint32_t>(value);
  }

  Gui gui;
  if (!gui_open(&gui)) {
    std::fprintf(stderr, "Unable to open game window: %s\n", SDL_GetError());
    gui_close(&gui);
    return 1;
  }
  Game game;
  game_init_seed(&game, seed);
  char title[64];
  SDL_snprintf(title, sizeof title, "Smol Invaders - seed %u", game.seed);
  SDL_SetWindowTitle(gui.window, title);

  bool running = true;
  Uint32 last = SDL_GetTicks();
  int frames = 0;
  while (running && (!smoke || frames++ < 4)) {
    if (smoke && frames == 2) {
      game_score_kill(&game, 5, false);
      game_room_progress(&game);
    }
    if (smoke && frames == 3) game_choose_upgrade(&game, 4);
    if (smoke && frames == 4) {
      game_score_kill(&game, 5, false);
      game_room_progress(&game);
    }
    running = gui_input(&gui, &game);
    Uint32 now = SDL_GetTicks();
    if (now - last >= 70) {
      game_step(&game);
      last = now;
    }
    gui_render(&gui, &game);
    SDL_Delay(smoke ? 0 : 8);
  }
  gui_close(&gui);
  return 0;
}
