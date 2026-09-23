#include "space_invaders/gui.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>

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
    if (event.key.keysym.mod & KMOD_SHIFT &&
        event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_9) {
      game_sell_module(g, 1 << (event.key.keysym.sym - SDLK_1));
      continue;
    }
    if (gui->seed_entry) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        gui->seed_entry = false;
        gui->seed_length = 0;
        gui->seed_text[0] = 0;
      } else if (event.key.keysym.sym == SDLK_BACKSPACE && gui->seed_length) {
        gui->seed_text[--gui->seed_length] = 0;
      } else if (event.key.keysym.sym == SDLK_RETURN && gui->seed_length) {
        char *end = nullptr;
        errno = 0;
        unsigned long value = std::strtoul(gui->seed_text, &end, 10);
        if (!errno && end != gui->seed_text && !*end && value <= UINT32_MAX) {
          game_init_seed(g, static_cast<uint32_t>(value));
          gui->seed = static_cast<uint32_t>(value);
          char title[64];
          SDL_snprintf(title, sizeof title, "Smol Invaders - seed %u", gui->seed);
          SDL_SetWindowTitle(gui->window, title);
          gui->seed_entry = false;
          gui->seed_length = 0;
          gui->seed_text[0] = 0;
          clear_controls(gui);
        }
      } else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9 &&
                 gui->seed_length < 10) {
        gui->seed_text[gui->seed_length++] =
            static_cast<char>('0' + event.key.keysym.sym - SDLK_0);
        gui->seed_text[gui->seed_length] = 0;
      }
      continue;
    }
    switch (event.key.keysym.sym) {
      case SDLK_ESCAPE: return false;
      case SDLK_LEFT: gui->left = !g->paused; break;
      case SDLK_RIGHT: gui->right = !g->paused; break;
      case SDLK_SPACE: gui->fire = !g->paused; break;
      case SDLK_p: game_toggle_pause(g); break;
      case SDLK_x: game_use_emp(g); break;
      case SDLK_LSHIFT:
        game_dash(g, gui->right ? 1 : gui->left ? -1 :
                  g->player < GAME_WIDTH / 2 ? 1 : -1);
        break;
      case SDLK_n:
        gui->seed_entry = true;
        gui->seed_length = 0;
        gui->seed_text[0] = 0;
        clear_controls(gui);
        break;
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
      case SDLK_5: game_reroll_shop(g); break;
      case SDLK_6: game_choose_upgrade(g, 6); break;
      case SDLK_h: game_hold_module(g); break;
      case SDLK_m: game_buy_module_slot(g); break;
      case SDLK_7: game_choose_route(g, ROOM_COMBAT); break;
      case SDLK_8: game_choose_route(g, ROOM_ELITE); break;
      case SDLK_9: game_choose_route(g, ROOM_CACHE); break;
      default: break;
    }
    if (g->paused || g->over || g->won) clear_controls(gui);
  }
  return true;
}
