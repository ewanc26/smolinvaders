#include "space_invaders/game.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void pilot(Game *g) {
  if (g->upgrade_offer) {
    int choice = g->lives < 3 && game_upgrade_available(g, 2) ? 2 : 4;
    if (game_upgrade_available(g, choice)) game_choose_upgrade(g, choice);
    else game_skip_upgrade(g);
    return;
  }
  int travel = (GAME_HEIGHT - 2 - g->alien_row) / PLAYER_SHOT_SPEED;
  int target = g->alien + g->direction * travel;
  if (target < 0) target = 0;
  if (target > GAME_WIDTH - 3) target = GAME_WIDTH - 3;
  if (g->enemy_bullet >= SHIELD_ROW &&
      g->enemy_bullet_x >= g->player && g->enemy_bullet_x < g->player + 3)
    target = g->player < GAME_WIDTH / 2 ? g->player + 3 : g->player - 3;
  game_tick(g, (target > g->player) - (target < g->player), true);
}

static void same_state(const Game *a, const Game *b) {
  /* Compare fields, not padding bytes in the struct. */
#define SAME(field) assert(a->field == b->field)
  SAME(player); SAME(alien); SAME(alien_row); SAME(alien_hp);
  SAME(bullet); SAME(bullet_x); SAME(enemy_bullet); SAME(enemy_bullet_x);
  SAME(direction); SAME(score); SAME(ai_mood); SAME(lives); SAME(wave);
  SAME(bonus_x); SAME(bonus_timer); SAME(bonus_direction); SAME(bonus_active);
  SAME(rng); SAME(seed); SAME(shop_rng); SAME(modules); SAME(module_offer);
  SAME(kills); SAME(boss_rules); SAME(room); SAME(room_type);
  SAME(shop_bought); SAME(shop_rerolls);
  SAME(upgrade_level); SAME(relics); SAME(relic_charges); SAME(ante);
  SAME(blind_target); SAME(credits); SAME(upgrade_offer);
  SAME(won); SAME(over); SAME(paused);
#undef SAME
  assert(memcmp(a->shields, b->shields, sizeof a->shields) == 0);
}

int main(void) {
  int wins = 0, losses = 0, timed_out = 0, best_room = 1, total_score = 0;
  for (uint32_t seed = 0; seed < 32; ++seed) {
    Game a, b;
    game_init_seed(&a, seed);
    game_init_seed(&b, seed);
    for (int tick = 0; tick < 20000 && !a.over && !a.won; ++tick) {
      pilot(&a);
      pilot(&b);
      same_state(&a, &b);
      assert(a.player >= 0 && a.player <= GAME_WIDTH - 3);
      assert(a.alien >= 0 && a.alien <= GAME_WIDTH - 3);
      assert(a.lives >= 0 && a.lives <= 5 && a.credits >= 0);
      assert(a.room >= 1 && a.room <= RUN_BLINDS);
    }
    wins += a.won;
    losses += a.over;
    timed_out += !a.won && !a.over;
    if (a.room > best_room) best_room = a.room;
    total_score += a.score;
  }
  assert(total_score > 0); /* At least one natural projectile collision. */
  printf("32 seeded pilots: %d wins, %d losses, %d tick limits; best blind %d; score %d\n",
         wins, losses, timed_out, best_room, total_score);
  return 0;
}
