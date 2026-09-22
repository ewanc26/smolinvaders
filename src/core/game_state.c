#include "space_invaders/game.h"

void game_init(Game *g) {
  *g = (Game){ .player = 22, .alien = 5, .alien_row = 2,
    .bullet = -1, .enemy_bullet = -1, .direction = 1, .lives = 3, .wave = 1, .rng = 7 };
}

void game_move(Game *g, int d) {
  if (!g->over && g->player + d >= 0 && g->player + d < GAME_WIDTH - 3)
    g->player += d;
}

void game_fire(Game *g) {
  if (!g->over && g->bullet < 0) g->bullet = GAME_HEIGHT - 2;
}
int game_ai_mood(const Game *g) { return g->ai_mood; }
