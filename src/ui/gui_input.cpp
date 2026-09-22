#include "space_invaders/gui.h"

static void clear_controls(Gui *gui) {
  gui->left = gui->right = gui->fire = false;
}

void gui_tick(Gui *gui, Game *g) {
  game_tick(g, int(gui->right) - int(gui->left), gui->fire);
  if (g->paused || g->over || g->won) clear_controls(gui);
}

bool gui_input(Gui *gui, Game *g) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return false;
    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
      clear_controls(gui);
      if (!g->paused) game_toggle_pause(g);
    }
    if (event.type == SDL_KEYUP) {
      if (event.key.keysym.sym == SDLK_LEFT) gui->left = false;
      if (event.key.keysym.sym == SDLK_RIGHT) gui->right = false;
      if (event.key.keysym.sym == SDLK_SPACE) gui->fire = false;
    }
    if (event.type != SDL_KEYDOWN || event.key.repeat) continue;
    switch (event.key.keysym.sym) {
      case SDLK_ESCAPE: return false;
      case SDLK_LEFT: gui->left = !g->paused; break;
      case SDLK_RIGHT: gui->right = !g->paused; break;
      case SDLK_SPACE: gui->fire = !g->paused; break;
      case SDLK_p: game_toggle_pause(g); break;
      case SDLK_r:
        if (g->over || g->won) {
          game_restart(g);
          clear_controls(gui);
        }
        break;
      case SDLK_0: game_skip_upgrade(g); break;
      case SDLK_1: game_choose_upgrade(g, 1); break;
      case SDLK_2: game_choose_upgrade(g, 2); break;
      case SDLK_3: game_choose_upgrade(g, 3); break;
      case SDLK_4: game_choose_upgrade(g, 4); break;
      default: break;
    }
    if (g->paused || g->over || g->won) clear_controls(gui);
  }
  return true;
}
