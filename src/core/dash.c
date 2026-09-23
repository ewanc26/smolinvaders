#include "space_invaders/game.h"

int game_dash_cooldown(const Game *g) {
  int cooldown = DASH_COOLDOWN - g->relics * 2;
  return cooldown < DASH_COOLDOWN - 6 ? DASH_COOLDOWN - 6 : cooldown;
}

bool game_dash(Game *g, int direction) {
  if (g->over || g->won || g->paused || g->upgrade_offer ||
      g->dash_cooldown || direction == 0) return false;
  int next = g->player + (direction > 0 ? DASH_DISTANCE : -DASH_DISTANCE);
  if (next < 0) next = 0;
  if (next > GAME_WIDTH - 3) next = GAME_WIDTH - 3;
  if (next == g->player) return false;
  g->player = next;
  g->dash_cooldown = game_dash_cooldown(g);
  return true;
}
