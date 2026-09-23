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
    Game cache = g;
    cache.route_chosen = false;
    cache.emp_charges = 0;
    assert(game_choose_route(&cache, ROOM_CACHE));
    assert(cache.room_type == ROOM_CACHE && cache.alien_hp == 1);
    assert(cache.emp_charges == 1);
    assert(!game_choose_route(&cache, ROOM_CACHE) && cache.emp_charges == 1);
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
  Game cache_clear;
  game_init_seed(&cache_clear, 77);
  cache_clear.room_type = ROOM_CACHE;
  cache_clear.score = cache_clear.blind_target;
  game_room_progress(&cache_clear);
  assert(cache_clear.free_rerolls == 1 && cache_clear.upgrade_offer);
  int credits = cache_clear.credits;
  game_reroll_shop(&cache_clear);
  assert(cache_clear.free_rerolls == 0 && cache_clear.credits == credits);
  Game skipped;
  game_init_seed(&skipped, 78);
  skipped.score = skipped.blind_target;
  game_room_progress(&skipped);
  game_skip_upgrade(&skipped);
  assert(skipped.skip_rerolls == 1);
  skipped.score = skipped.blind_target;
  game_room_progress(&skipped);
  credits = skipped.credits;
  game_reroll_shop(&skipped);
  assert(skipped.skip_rerolls == 0 && skipped.credits == credits);
  return 0;
}
