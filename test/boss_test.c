#include "space_invaders/game.h"
#include <assert.h>

static void hit(Game *g) {
  g->bullet = g->alien_row + 1;
  g->bullet_x = g->alien + 1;
  game_step(g);
}

static void restrictions(void) {
  Game g;
  game_init(&g);
  g.room = 3;
  g.boss_rules = BOSS_STATIC;
  g.modules = 7;
  g.kills = 2;
  game_score_kill(&g, 3, true);
  assert(g.score == 3 && g.modules == 7 && g.kills == 3);
  g.score = g.blind_target;
  game_room_progress(&g);
  assert(g.room == 3 && !g.upgrade_offer); /* Boss cannot be skipped. */

  g.boss_rules = BOSS_BREACH;
  g.enemy_bullet = SHIELD_ROW - 1;
  g.enemy_bullet_x = 1;
  game_step(&g);
  assert(g.enemy_bullet == SHIELD_ROW && g.shields[0][0] == 3);
  g.boss_rules = 0;
  g.enemy_bullet = SHIELD_ROW - 1;
  game_step(&g);
  assert(g.shields[0][0] == 2);

  g.boss_rules = BOSS_BLACKOUT;
  g.bonus_timer = 140;
  game_step(&g);
  assert(!g.bonus_active && g.bonus_timer == 140);
  g.boss_rules = 0;
  game_step(&g);
  assert(g.bonus_active);
}

static void complete_run(uint32_t seed) {
  Game g, replay;
  game_init_seed(&g, seed);
  int bosses = 0;
  for (int room = 1; room <= RUN_BLINDS; ++room) {
    assert(g.room == room && g.ante == (room - 1) / 3 + 1);
    assert(!g.won && !g.over);
    if (game_is_boss(&g)) {
      ++bosses;
      replay = g;
      game_boss_prepare(&replay);
      assert(replay.boss_rules == g.boss_rules);
      assert(g.alien_hp == 2 + g.ante);
      assert(g.boss_rules != 0);
      int credits = g.credits;
      while (g.room == room && g.alien_hp > 0) {
        g.bullet = g.alien_row + 1;
        g.bullet_x = g.alien + 1;
        game_step(&g);
      }
      assert(g.relics >= 1 && g.relics <= 3 &&
             g.relic_charges == g.relics && g.credits >= credits + 3);
      if (room < RUN_BLINDS) {
        assert(g.upgrade_offer && g.paused);
        game_skip_upgrade(&g);
      }
      if (room == RUN_BLINDS) assert(g.boss_rules == 7 && g.won);
      continue;
    } else assert(g.boss_rules == 0);
    /* Controlled collisions test the entire progression, not bot skill. */
    for (int shots = 0; g.room == room && !g.won && shots < 100; ++shots) {
      g.enemy_bullet = -1;
      hit(&g);
    }
    if (room < RUN_BLINDS) {
      assert(g.room == room + 1 && g.upgrade_offer && g.paused);
      assert(g.blind_target == g.score + 4 + g.ante);
      game_skip_upgrade(&g);
    }
  }
  assert(bosses == 8 && g.won && !g.over && !g.upgrade_offer);
  int score = g.score;
  game_step(&g);
  assert(g.score == score);
  game_restart(&g);
  assert(g.seed == seed && !g.boss_rules && !g.won && g.room == 1 && g.best_room == 1);
}

int main(void) {
  restrictions();
  unsigned seen = 0;
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game g;
    game_init_seed(&g, seed);
    g.room = 3;
    game_boss_prepare(&g);
    seen |= (unsigned)g.boss_rules;
    complete_run(seed);
  }
  assert(seen == 7);
  return 0;
}
