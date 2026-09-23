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
  g.credits = 20;
  int offer = g.module_offer;
  key(SDL_KEYDOWN, SDLK_5);
  assert(gui_input(&gui, &g));
  assert(g.module_offer != offer && g.credits == 18 && g.paused);
  key(SDL_KEYDOWN, SDLK_m);
  assert(gui_input(&gui, &g));
  assert(g.module_slots == MODULE_START_SLOTS + 1 && g.credits == 12);
  key(SDL_KEYDOWN, SDLK_4);
  assert(gui_input(&gui, &g));
  assert(g.modules && g.credits == 7 && g.upgrade_offer && g.paused);
  key(SDL_KEYDOWN, SDLK_0);
  assert(gui_input(&gui, &g));
  assert(!g.paused && !g.upgrade_offer);
  key(SDL_KEYDOWN, SDLK_x, true);
  assert(gui_input(&gui, &g));
  assert(g.emp_charges == 1 && g.emp_ticks == 0);
  key(SDL_KEYDOWN, SDLK_x);
  assert(gui_input(&gui, &g));
  assert(g.emp_charges == 0 && g.emp_ticks == EMP_DURATION);
  key(SDL_KEYDOWN, SDLK_n);
  assert(gui_input(&gui, &g) && gui.seed_entry && g.seed == 42);
  key(SDL_KEYDOWN, SDLK_1);
  key(SDL_KEYDOWN, SDLK_2);
  key(SDL_KEYDOWN, SDLK_3);
  key(SDL_KEYDOWN, SDLK_RETURN);
  assert(gui_input(&gui, &g));
  assert(!gui.seed_entry && g.seed == 123 && g.room == 1 && g.emp_charges == 1);
  key(SDL_KEYDOWN, SDLK_n);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_9);
  key(SDL_KEYDOWN, SDLK_RETURN);
  assert(gui_input(&gui, &g) && gui.seed_entry && g.seed == 123);
  key(SDL_KEYDOWN, SDLK_ESCAPE);
  assert(gui_input(&gui, &g) && !gui.seed_entry);
  key(SDL_KEYDOWN, SDLK_n);
  key(SDL_KEYDOWN, SDLK_ESCAPE);
  assert(gui_input(&gui, &g) && !gui.seed_entry && g.seed == 123);
  g.upgrade_offer = g.paused = true;
  g.room = 1;
  g.emp_charges = 0;
  key(SDL_KEYDOWN, SDLK_9);
  assert(gui_input(&gui, &g));
  assert(g.room_type == ROOM_CACHE && g.emp_charges == 1 && g.paused);
  key(SDL_KEYDOWN, SDLK_9);
  assert(gui_input(&gui, &g) && g.emp_charges == 1);
  g.room = 3;
  g.room_type = ROOM_ELITE;
  g.alien_hp = 4;
  key(SDL_KEYDOWN, SDLK_7);
  assert(gui_input(&gui, &g));
  assert(g.room_type == ROOM_ELITE && g.alien_hp == 4);
  g.upgrade_offer = false;
  g.paused = false;
  g.player = 10;
  key(SDL_KEYDOWN, SDLK_LSHIFT);
  assert(gui_input(&gui, &g));
  assert(g.player == 14 && g.dash_cooldown == DASH_COOLDOWN);
  SDL_Quit();
}
