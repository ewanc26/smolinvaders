#include "space_invaders/game.h"
#include <assert.h>

static void timing(void) {
  Game g;
  game_init_seed(&g, 42);
  g.relics = 3;
  assert(game_emp_duration(&g) == EMP_DURATION + 6);
  g.relics = 0;
  g.enemy_bullet = GAME_HEIGHT - 2;
  g.enemy_bullet_x = g.player + 1;
  g.emp_charges = 2;
  assert(game_use_emp(&g));
  assert(g.emp_charges == 1 && g.enemy_bullet == -1);
  assert(!game_use_emp(&g) && g.emp_charges == 1);
  int alien = g.alien, row = g.alien_row, timer = g.bonus_timer;
  uint32_t rng = g.rng;
  game_toggle_pause(&g);
  game_step(&g);
  assert(g.emp_ticks == EMP_DURATION && !game_use_emp(&g));
  game_toggle_pause(&g);
  for (int tick = 0; tick < EMP_DURATION; ++tick) {
    game_tick(&g, -1, false);
    assert(g.alien == alien && g.alien_row == row && g.enemy_bullet == -1);
    assert(g.bonus_timer == timer && g.rng == rng && g.lives == 3);
  }
  assert(g.emp_ticks == 0 && g.player == 22 - EMP_DURATION);
  game_step(&g);
  assert(g.alien != alien || g.alien_row != row);
  assert(game_use_emp(&g) && g.emp_charges == 0);
  game_restart(&g);
  assert(g.emp_charges == 1 && g.emp_ticks == 0 && g.seed == 42);
}

static void combat_and_shop(void) {
  Game g;
  game_init(&g);
  assert(game_use_emp(&g));
  g.score = g.blind_target - 1;
  g.bullet_x = g.alien + 1;
  g.bullet = g.alien_row + game_player_shot_speed(&g);
  game_step(&g);
  assert(g.room == 2 && g.upgrade_offer && g.emp_ticks == 0);
  assert(g.emp_charges == 0); /* Charges do not regenerate each blind. */
  g.credits = 10;
  game_choose_upgrade(&g, 6);
  assert(g.emp_charges == 1 && g.credits == 7 && g.paused);
  assert(!game_use_emp(&g) && g.emp_charges == 1);
  game_choose_upgrade(&g, 6);
  assert(g.emp_charges == 1 && g.credits == 7); /* One stock per visit. */
  game_skip_upgrade(&g);
  g.score = g.blind_target;
  game_room_progress(&g);
  game_choose_upgrade(&g, 6);
  assert(g.emp_charges == EMP_CAPACITY);
  g.shop_bought = 0;
  assert(!game_upgrade_available(&g, 6));
  game_skip_upgrade(&g);
  g.boss_rules = BOSS_STATIC | BOSS_BREACH | BOSS_BLACKOUT;
  assert(game_use_emp(&g)); /* Consumables are not scoring modules. */
  g.emp_ticks = 0;
  g.over = true;
  assert(!game_use_emp(&g));
  g.over = false;
  g.won = true;
  assert(!game_use_emp(&g));
  g.won = false;
  g.emp_charges = 0;
  assert(!game_use_emp(&g));
}

int main(void) {
  timing();
  combat_and_shop();
  return 0;
}
