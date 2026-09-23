#include "space_invaders/game.h"
#include <assert.h>

static void aim_at_alien(Game *g) {
  g->bullet_x = g->alien + 1;
  g->bullet = g->alien_row + 1;
}

int main(void) {
  Game first, replay;
  game_init_seed(&first, 42);
  game_init_seed(&replay, 42);
  Game other_seed;
  game_init_seed(&other_seed, 43);
  assert(first.alien == replay.alien && first.alien != other_seed.alien);
  first.score = replay.score = 4;
  first.player_velocity = replay.player_velocity = 1;
  aim_at_alien(&first);
  aim_at_alien(&replay);
  game_step(&first);
  game_step(&replay);
  assert(first.room == 2 && first.upgrade_offer && first.paused);
  assert(first.player_velocity == 0);
  assert(first.credits == 5);
  assert(first.room_type == replay.room_type && first.alien == replay.alien);
  assert(first.rng == replay.rng);
  assert(first.rng == replay.rng && first.blind_target == 10);
  for (int draw = 0; draw < 50; ++draw) game_random(&replay);
  Game next = first;
  Game noisy = replay;
  next.score = noisy.score = next.blind_target;
  next.alien_hp = noisy.alien_hp = 0;
  game_room_progress(&next);
  game_room_progress(&noisy);
  assert(next.room_type == noisy.room_type && next.alien == noisy.alien &&
         next.boss_rules == noisy.boss_rules);
  Game streak;
  game_init_seed(&streak, 44);
  streak.score = streak.blind_target - 1;
  streak.combo = COMBO_CAP;
  streak.combo_timer = COMBO_WINDOW;
  aim_at_alien(&streak);
  game_step(&streak);
  assert(streak.upgrade_offer && streak.credits == 7 && streak.combo == 0);
  assert(game_choose_route(&first, ROOM_ELITE));
  assert(first.room_type == ROOM_ELITE && first.alien_hp == 3);
  assert(!game_choose_route(&first, ROOM_CACHE));
  game_skip_upgrade(&first);

  Game saucer;
  game_init_seed(&saucer, 7);
  saucer.score = 4;
  saucer.room_type = ROOM_ELITE;
  saucer.bonus_active = true;
  saucer.bonus_x = saucer.player;
  saucer.bullet_x = saucer.player + 1;
  saucer.bullet = 1;
  game_step(&saucer);
  assert(saucer.score == 7 && saucer.room == 2);
  assert(saucer.relics == 0 && saucer.credits == 8);

  Game elite;
  game_init_seed(&elite, 9);
  elite.room_type = ROOM_ELITE;
  elite.alien_hp = 1;
  aim_at_alien(&elite);
  game_step(&elite);
  assert(elite.relics == 1 && elite.relic_charges == 1 && elite.credits == 7);
  assert(elite.flawless_streak == 1);
  Game worn = elite;
  worn.room_type = ROOM_ELITE;
  worn.alien_hp = 1;
  worn.relics = 1;
  worn.relic_charges = 0;
  worn.credits = 3;
  aim_at_alien(&worn);
  game_step(&worn);
  assert(worn.relics == 2 && worn.relic_charges == 2 && worn.credits == 6);

  elite.enemy_bullet = GAME_HEIGHT - 2;
  elite.enemy_bullet_x = elite.player + 1;
  game_step(&elite);
  assert(elite.relic_charges == 0 && elite.lives == 3);
  assert(elite.flawless_streak == 0);
  elite.enemy_bullet = GAME_HEIGHT - 2;
  elite.enemy_bullet_x = elite.player + 1;
  game_step(&elite);
  assert(elite.lives == 2);

  elite.score = elite.blind_target - 1;
  elite.bullet_x = elite.alien + 1;
  elite.bullet = elite.alien_row + 1;
  elite.alien_hp = 1;
  game_step(&elite);
  assert(elite.upgrade_offer && elite.relic_charges == 2);
  elite.upgrade_offer = false;
  elite.room = 2;
  elite.score = 0;
  elite.room_type = ROOM_ELITE;
  elite.alien_hp = 1;
  elite.bullet_x = elite.alien + 1;
  elite.bullet = elite.alien_row + 1;
  game_step(&elite);
  assert(elite.relics == 2 && elite.relic_charges == 2);

  Game boss;
  game_init_seed(&boss, 0);
  boss.room = RUN_BLINDS;
  boss.ante = RUN_ANTES;
  boss.blind_target = 50;
  boss.score = 45;
  boss.alien_hp = 1;
  aim_at_alien(&boss);
  game_step(&boss);
  assert(boss.won && !boss.over && boss.score >= boss.blind_target);
  int position = boss.alien;
  game_step(&boss);
  assert(boss.alien == position);
  game_restart(&boss);
  assert(boss.seed == 0 && !boss.won && boss.room == 1);
  return 0;
}
