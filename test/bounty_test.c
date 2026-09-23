#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game elite, combat, boss, static_boss;
  game_init_seed(&elite, 9u);
  combat = boss = static_boss = elite;
  elite.modules = combat.modules = boss.modules = static_boss.modules = MODULE_BOUNTY;
  elite.room_type = ROOM_ELITE;
  combat.room_type = ROOM_COMBAT;
  boss.room = static_boss.room = 3;
  boss.room_type = static_boss.room_type = ROOM_ELITE;
  static_boss.boss_rules = BOSS_STATIC;

  int initial = elite.credits;
  game_score_kill(&elite, 1, false);
  game_score_kill(&combat, 1, false);
  game_score_kill(&boss, 1, false);
  game_score_kill(&static_boss, 1, false);
  assert(elite.credits == initial + 1);
  assert(combat.credits == initial);
  assert(boss.credits == initial + 1);
  assert(static_boss.credits == initial);
  return 0;
}
