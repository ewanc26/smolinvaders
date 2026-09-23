#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  for (int room = 1; room < RUN_BLINDS; ++room) {
    Game g;
    game_init_seed(&g, (uint32_t)room);
    g.room = room;
    g.upgrade_offer = g.paused = true;
    g.room_type = ROOM_COMBAT;
    g.alien_hp = 1;
    if (room % 3 == 0) {
      assert(!game_choose_route(&g, ROOM_ELITE));
      continue;
    }
    assert(game_choose_route(&g, ROOM_ELITE));
    assert(g.room_type == ROOM_ELITE && g.alien_hp == 3 && g.paused);
    assert(game_choose_route(&g, ROOM_CACHE));
    assert(g.room_type == ROOM_CACHE && g.alien_hp == 1);
    assert(!game_choose_route(&g, -1) && !game_choose_route(&g, 99));
  }
  Game boss;
  game_init(&boss);
  boss.room = 3;
  boss.upgrade_offer = boss.paused = true;
  boss.room_type = ROOM_ELITE;
  boss.alien_hp = 4;
  assert(!game_choose_route(&boss, ROOM_COMBAT));
  assert(boss.room_type == ROOM_ELITE && boss.alien_hp == 4);
  boss.upgrade_offer = false;
  assert(!game_choose_route(&boss, ROOM_CACHE));
  return 0;
}
