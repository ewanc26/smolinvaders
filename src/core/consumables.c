#include "space_invaders/game.h"

int game_emp_duration(const Game *g) {
  int duration = EMP_DURATION + g->relics * 2;
  return duration > EMP_DURATION + 6 ? EMP_DURATION + 6 : duration;
}

bool game_use_emp(Game *g) {
  if (g->over || g->won || g->paused || g->upgrade_offer ||
      g->emp_charges == 0 || g->emp_ticks > 0) return false;
  --g->emp_charges;
  g->emp_ticks = game_emp_duration(g);
  g->enemy_bullet = -1;
  return true;
}
