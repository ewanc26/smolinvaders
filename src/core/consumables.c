#include "space_invaders/game.h"

bool game_use_emp(Game *g) {
  if (g->over || g->won || g->paused || g->upgrade_offer ||
      g->emp_charges == 0 || g->emp_ticks > 0) return false;
  --g->emp_charges;
  g->emp_ticks = EMP_DURATION;
  g->enemy_bullet = -1;
  return true;
}
