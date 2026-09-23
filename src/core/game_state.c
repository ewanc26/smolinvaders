#include "space_invaders/game.h"

void game_init_seed(Game *g, uint32_t seed) {
  *g = (Game){ .player = 22, .alien = 5, .alien_row = 2, .alien_hp = 1,
    .bullet = -1, .enemy_bullet = -1, .direction = 1, .lives = 3, .wave = 1,
    .rng = seed, .seed = seed, .shop_rng = seed ^ 0x9e3779b9u,
    .room = 1, .best_room = 1, .room_type = ROOM_COMBAT,
    .module_slots = MODULE_START_SLOTS,
    .ante = 1, .blind_target = 5, .credits = 3, .emp_charges = 1, .bonus_x = -3,
    .bonus_direction = 1 };
  game_shields_init(g);
  g->alien = game_seed_hash(seed, 1) % (GAME_WIDTH - 3);
  g->bonus_direction = game_seed_hash(seed, 2) & 1 ? 1 : -1;
}

void game_init(Game *g) { game_init_seed(g, 0xC0FFEEu); }

void game_restart(Game *g) { game_init_seed(g, g->seed); }

void game_toggle_pause(Game *g) {
  if (!g->over && !g->won && !g->upgrade_offer) g->paused = !g->paused;
}

void game_move(Game *g, int d) {
  if (!d || g->over || g->won || g->paused || g->player + d < 0 ||
      g->player + d > GAME_WIDTH - 3) {
    g->player_velocity = 0;
    return;
  }
  g->player_velocity = d;
  g->player += d;
}

void game_fire(Game *g) {
  if (!g->over && !g->won && !g->paused && g->bullet < 0) {
    g->bullet = GAME_HEIGHT - 2;
    g->bullet_x = g->player + 1;
  }
}
int game_ai_mood(const Game *g) { return g->ai_mood; }
