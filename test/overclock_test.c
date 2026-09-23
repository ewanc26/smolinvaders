#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game base, overclock;
  game_init_seed(&base, 0x0C0FFEEu);
  overclock = base;
  overclock.modules = MODULE_OVERCLOCK;

  base.kills = 4;
  overclock.kills = 4;
  game_score_kill(&base, 1, false);
  game_score_kill(&overclock, 1, false);
  assert(overclock.score == base.score + 3);

  base.kills = 3;
  overclock.kills = 3;
  base.score = overclock.score = 0;
  game_score_kill(&base, 1, false);
  game_score_kill(&overclock, 1, false);
  assert(overclock.score == base.score);

  Game offer;
  game_init_seed(&offer, 17u);
  offer.modules = MODULE_MASK & ~MODULE_OVERCLOCK;
  game_module_offer(&offer);
  assert(offer.module_offer == MODULE_OVERCLOCK);
  return 0;
}
