#include "space_invaders/game.h"
#include <stdlib.h>

void game_init(Game *g) {
  *g = (Game){ .player = 22, .alien = 5, .alien_row = 2,
    .bullet = -1, .direction = 1 };
}

void game_move(Game *g, int d) {
  if (!g->over && g->player + d >= 0 && g->player + d < GAME_WIDTH - 3)
    g->player += d;
}

void game_fire(Game *g) { if (!g->over && g->bullet < 0) g->bullet = GAME_HEIGHT - 2; }

void game_tick(Game *g) {
  if (g->over) return;
  if (g->bullet >= 0) {
    if (g->bullet == g->alien_row) {
      if (g->player + 1 >= g->alien && g->player <= g->alien + 2)
        g->score++, g->alien = rand() % (GAME_WIDTH - 3), g->alien_row = 2, g->bullet = -1;
      else g->bullet--;
    } else g->bullet--;
  }
  g->alien += g->direction;
  if (g->alien < 0 || g->alien > GAME_WIDTH - 3) g->direction = -g->direction, g->alien_row++;
  if (g->alien_row > GAME_HEIGHT - 2) g->over = true;
}
