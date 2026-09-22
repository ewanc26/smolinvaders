#include "space_invaders/game.h"

void game_init_seed(Game *g, uint32_t seed) {
  *g = (Game){ .player = 22, .alien = 5, .alien_row = 2, .alien_hp = 1,
    .bullet = -1, .enemy_bullet = -1, .direction = 1, .lives = 3, .wave = 1,
    .rng = (int)seed, .seed = seed, .room = 1, .room_type = ROOM_COMBAT, .bonus_x = -3, .bonus_timer = 0, .bonus_direction = 1 };
  game_shields_init(g);
}

void game_init(Game *g) { game_init_seed(g, 0xC0FFEEu); }

void game_restart(Game *g) { uint32_t seed = g->seed; game_init_seed(g, seed ? seed : 0xC0FFEEu); }

void game_toggle_pause(Game *g) { if (!g->over) g->paused = !g->paused; }

void game_choose_upgrade(Game *g, int choice) {
  if (!g->upgrade_offer || choice < 1 || choice > 3) return;
  if (choice == 1) game_shields_init(g);
  if (choice == 2 && g->lives < 5) ++g->lives;
  if (choice == 3) ++g->upgrade_level;
  g->upgrade_offer = false; g->paused = false;
}

void game_move(Game *g, int d) {
  if (!g->over && g->player + d >= 0 && g->player + d < GAME_WIDTH - 3)
    g->player += d;
}

void game_fire(Game *g) {
  if (!g->over && g->bullet < 0) g->bullet = GAME_HEIGHT - 2;
}
int game_ai_mood(const Game *g) { return g->ai_mood; }
