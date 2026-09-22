#include "space_invaders/gui.h"
#include <cassert>

static void key(Uint32 type, SDL_Keycode code, bool repeat = false) {
  SDL_Event event{};
  event.type = type;
  event.key.keysym.sym = code;
  event.key.repeat = repeat;
  assert(SDL_PushEvent(&event) == 1);
}

int main() {
  assert(SDL_Init(SDL_INIT_EVENTS) == 0);
  Gui gui;
  Game g;
  game_init_seed(&g, 42);
  g.player = 12; // Fire through a gap so cover does not consume the shot.
  key(SDL_KEYDOWN, SDLK_RIGHT);
  key(SDL_KEYDOWN, SDLK_SPACE);
  assert(gui_input(&gui, &g));
  assert(g.player == 12 && g.bullet == -1); // Events do not advance simulation.
  gui_tick(&gui, &g);
  assert(g.player == 13 && g.bullet_x == 14);
  gui_tick(&gui, &g);
  assert(g.player == 14 && g.bullet_x == 14);
  key(SDL_KEYDOWN, SDLK_LEFT);
  assert(gui_input(&gui, &g));
  gui_tick(&gui, &g);
  assert(g.player == 14); // Opposite directions cancel.
  key(SDL_KEYUP, SDLK_RIGHT);
  assert(gui_input(&gui, &g));
  gui_tick(&gui, &g);
  assert(g.player == 13);

  SDL_Event focus{};
  focus.type = SDL_WINDOWEVENT;
  focus.window.event = SDL_WINDOWEVENT_FOCUS_LOST;
  assert(SDL_PushEvent(&focus) == 1);
  assert(gui_input(&gui, &g));
  assert(g.paused && !gui.left && !gui.right && !gui.fire);
  int row = g.bullet;
  gui_tick(&gui, &g);
  assert(g.player == 13 && g.bullet == row);
  key(SDL_KEYDOWN, SDLK_p, true);
  assert(gui_input(&gui, &g));
  assert(g.paused); // OS repeat cannot toggle pause.
  key(SDL_KEYDOWN, SDLK_p);
  assert(gui_input(&gui, &g));
  gui_tick(&gui, &g);
  assert(!g.paused && g.player == 13);
  key(SDL_KEYDOWN, SDLK_SPACE);
  key(SDL_KEYUP, SDLK_SPACE);
  assert(gui_input(&gui, &g));
  assert(!gui.fire);

  g.score = g.blind_target;
  game_room_progress(&g);
  key(SDL_KEYDOWN, SDLK_LEFT);
  assert(gui_input(&gui, &g));
  gui_tick(&gui, &g);
  assert(g.player == 13 && g.upgrade_offer && !gui.left);
  key(SDL_KEYDOWN, SDLK_0);
  assert(gui_input(&gui, &g));
  assert(!g.paused && !g.upgrade_offer);
  SDL_Quit();
}
