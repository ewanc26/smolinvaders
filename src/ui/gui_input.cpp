#include "space_invaders/gui.h"

bool gui_input(Gui *, Game *g) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return false;
    if (event.type != SDL_KEYDOWN || event.key.repeat) continue;
    switch (event.key.keysym.sym) {
      case SDLK_ESCAPE: return false;
      case SDLK_LEFT: game_move(g, -1); break;
      case SDLK_RIGHT: game_move(g, 1); break;
      case SDLK_SPACE: game_fire(g); break;
      case SDLK_p: game_toggle_pause(g); break;
      case SDLK_r:
        if (g->over || g->won) game_restart(g);
        break;
      case SDLK_0: game_skip_upgrade(g); break;
      case SDLK_1: game_choose_upgrade(g, 1); break;
      case SDLK_2: game_choose_upgrade(g, 2); break;
      case SDLK_3: game_choose_upgrade(g, 3); break;
      default: break;
    }
  }
  return true;
}
