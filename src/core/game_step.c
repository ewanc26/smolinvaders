#include "space_invaders/game.h"
#include <stdlib.h>

static void step_bullet(Game *g) {
  if (g->bullet < 0) return;
  if (g->bullet == g->alien_row) {
    if (g->player + 1 >= g->alien && g->player <= g->alien + 2) {
      g->score++; g->alien = rand() % (GAME_WIDTH - 3);
      g->alien_row = 2; g->bullet = -1; return;
    }
  }
  g->bullet--;
}

static void step_enemy_bullet(Game *g){if(g->enemy_bullet<0)return;if(++g->enemy_bullet>=GAME_HEIGHT-1){if(g->player+1>=g->alien&&g->player<=g->alien+2)g->over=true;g->enemy_bullet=-1;}}
static void step_alien(Game *g) {
  game_ai(g);
  g->alien += g->direction;
  if (g->alien < 0 || g->alien > GAME_WIDTH - 3)
    g->direction = -g->direction, g->alien_row++;
  if (g->alien_row > GAME_HEIGHT - 2) g->over = true;
}

void game_step(Game *g) {
  if (!g->over) step_bullet(g), step_enemy_bullet(g), step_alien(g);
}
