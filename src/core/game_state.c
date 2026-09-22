#include "space_invaders/game.h"

void game_init_seed(Game *g, uint32_t seed) {
  *g = (Game){ .player = 22, .alien = 5, .alien_row = 2, .alien_hp = 1,
    .bullet = -1, .enemy_bullet = -1, .direction = 1, .lives = 3, .wave = 1,
    .rng = seed, .seed = seed, .room = 1, .room_type = ROOM_COMBAT,
    .ante = 1, .blind_target = 5, .credits = 3, .bonus_x = -3,
    .bonus_direction = 1 };
  game_shields_init(g);
  g->alien = game_random(g) % (GAME_WIDTH - 3);
}

void game_init(Game *g) { game_init_seed(g, 0xC0FFEEu); }

void game_restart(Game *g) { game_init_seed(g, g->seed); }

void game_toggle_pause(Game *g) {
  if (!g->over && !g->won && !g->upgrade_offer) g->paused = !g->paused;
}

int game_upgrade_cost(int choice) {
  return choice == 1 ? 1 : choice == 2 ? 3 : choice == 3 ? 4 : -1;
}

bool game_upgrade_available(const Game *g, int choice) {
  int cost = game_upgrade_cost(choice);
  return g->upgrade_offer && cost >= 0 && g->credits >= cost &&
         !(choice == 2 && g->lives >= 5) &&
         !(choice == 3 && g->upgrade_level >= 3);
}

void game_choose_upgrade(Game *g, int choice) {
  if (!game_upgrade_available(g, choice)) return;
  g->credits -= game_upgrade_cost(choice);
  if (choice == 1) game_shields_init(g);
  if (choice == 2 && g->lives < 5) ++g->lives;
  if (choice == 3) ++g->upgrade_level;
  g->upgrade_offer = false; g->paused = false;
}

void game_skip_upgrade(Game *g) {
  if (g->upgrade_offer) g->upgrade_offer = false, g->paused = false;
}

void game_move(Game *g, int d) {
  if (!g->over && !g->won && !g->paused && g->player + d >= 0 &&
      g->player + d <= GAME_WIDTH - 3)
    g->player += d;
}

void game_fire(Game *g) {
  if (!g->over && !g->won && !g->paused && g->bullet < 0) {
    g->bullet = GAME_HEIGHT - 2;
    g->bullet_x = g->player + 1;
  }
}
int game_ai_mood(const Game *g) { return g->ai_mood; }
