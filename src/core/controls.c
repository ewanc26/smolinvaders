#include "space_invaders/game.h"

void game_tick(Game *g, int movement, bool fire) {
  if (g->over || g->won || g->paused || g->upgrade_offer) return;
  if (g->dash_cooldown) --g->dash_cooldown;
  game_move(g, (movement > 0) - (movement < 0));
  if (fire) game_fire(g);
  game_step(g);
}
