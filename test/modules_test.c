#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game a, b;
    game_init_seed(&a, seed);
    game_init_seed(&b, seed);
    a.relics = 3;
    assert(game_combo_window(&a) == COMBO_WINDOW + 6);
    uint32_t combat_rng = a.rng;
    for (int i = 0; i < 6; ++i) {
      game_module_offer(&a);
      game_module_offer(&b);
      assert(a.module_offer == b.module_offer && a.module_offer);
      assert(!(a.modules & a.module_offer));
      a.upgrade_offer = a.paused = true;
      a.module_slots = MODULE_MAX_SLOTS;
      a.credits = 4;
      game_choose_upgrade(&a, 4);
      assert(a.credits == 4 && a.upgrade_offer);
      a.credits = 5;
      game_choose_upgrade(&a, 4);
      assert(a.credits == 0 && a.paused && a.upgrade_offer);
      b.modules = a.modules;
    }
    game_module_offer(&a);
    assert(a.module_offer == 0 && a.modules == MODULE_MASK && a.rng == combat_rng);
    a.upgrade_offer = true;
    a.credits = 100;
    assert(!game_upgrade_available(&a, 4));
    game_restart(&a);
    assert(a.seed == seed && !a.modules && !a.kills && !a.module_offer);
  }

  Game g;
  game_init(&g);
  g.modules = MODULE_AMPLIFIER | MODULE_SIGNAL | MODULE_CADENCE;
  g.kills = 2;
  g.bonus_active = true;
  g.bonus_x = 20;
  g.bullet_x = 21;
  g.bullet = 1;
  game_step(&g);
  assert(g.score == 13 && g.kills == 3); /* ((3 + 1 + 2) * 2) + 1 */
  assert(g.room == 2 && g.blind_target == 18 && g.upgrade_offer);
  game_room_progress(&g);
  assert(g.room == 2);
  game_skip_upgrade(&g);
  g.alien_hp = 2;
  g.bullet_x = g.alien + 1;
  g.bullet = g.alien_row + 1;
  game_step(&g);
  assert(g.score == 13 && g.kills == 3); /* Armor hits are not kills. */
  g.bullet_x = g.alien + 1;
  g.bullet = g.alien_row + 1;
  game_step(&g);
  assert(g.score == 17 && g.kills == 4 && g.room == 2);
  game_score_kill(&g, 1, false);
  game_score_kill(&g, 1, false);
  game_score_kill(&g, 1, false);
  assert(g.combo == 4 && g.score == 27); /* four-kill combo reaches 2x */
  assert(game_combo_multiplier(&g) == 2);
  g.modules = MODULE_AMPLIFIER | MODULE_CADENCE;
  g.kills = 3;
  g.combo = g.combo_timer = 0;
  int before = g.score;
  game_score_kill(&g, 1, false);
  assert(g.score == before + 4); /* amplifier + fourth-kill synergy */
  g.modules = MODULE_SIGNAL | MODULE_SCAVENGER;
  g.emp_charges = 0;
  int credits = g.credits;
  game_score_kill(&g, 1, true);
  assert(g.credits == credits + 1 && g.emp_charges == 1);
  g.modules = MODULE_SCAVENGER;
  g.emp_charges = 0;
  game_score_kill(&g, 3, true);
  assert(g.emp_charges == 1);
  g.emp_charges = EMP_CAPACITY;
  game_score_kill(&g, 3, true);
  assert(g.emp_charges == EMP_CAPACITY);
  g.modules = MODULE_BARRIER | MODULE_SCAVENGER;
  g.emp_charges = 0;
  g.barrier_used = false;
  g.enemy_bullet = SHIELD_ROW - 1;
  g.enemy_bullet_x = 1;
  int shield = g.shields[0][0];
  game_step(&g);
  assert(g.barrier_used && g.shields[0][0] == shield && g.emp_charges == 1);
  g.enemy_bullet = SHIELD_ROW - 1;
  game_step(&g);
  assert(g.shields[0][0] == shield - 1);
  g.barrier_used = false;
  g.boss_rules = BOSS_BREACH;
  g.enemy_bullet = SHIELD_ROW - 1;
  shield = g.shields[0][0];
  game_step(&g);
  assert(!g.barrier_used && g.shields[0][0] == shield);
  return 0;
}
