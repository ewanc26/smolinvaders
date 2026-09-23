#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game g;
  game_init_seed(&g, 42);
  Game noisy = g;
  for (int draw = 0; draw < 40; ++draw) game_random(&noisy);
  assert(g.player == noisy.player);
  assert(g.player == 12 + game_seed_hash(g.seed, 0x5000u) % (GAME_WIDTH - 3 - 24));
  assert(g.bonus_direction == (game_seed_hash(g.seed, 2) & 1 ? 1 : -1));
  Game shield_replay;
  game_init_seed(&shield_replay, 42);
  assert(g.shields[0][0] == shield_replay.shields[0][0]);
  assert(g.shields[0][1] == shield_replay.shields[0][1]);
  assert(game_bonus_interval(&g) >= 140 && game_bonus_interval(&g) < 170);
  g.room_type = ROOM_CACHE;
  assert(game_bonus_interval(&g) >= 90 && game_bonus_interval(&g) < 110);
  g.room_type = ROOM_COMBAT;
  Game noisy_bonus = g;
  for (int draw = 0; draw < 40; ++draw) game_random(&noisy_bonus);
  assert(game_bonus_interval(&g) == game_bonus_interval(&noisy_bonus));
  assert(g.bonus_direction == noisy_bonus.bonus_direction);
  game_fire(&g);
  int lane = g.bullet_x;
  game_move(&g, 1);
  assert(g.player_velocity == 1);
  game_step(&g);
  assert(g.bullet_x == lane);
  assert(g.player + 1 != lane);
  game_move(&g, 0);
  assert(g.player_velocity == 0);

  g.enemy_bullet = SHIELD_ROW - 1;
  g.enemy_bullet_x = 1;
  g.alien = 30;
  game_step(&g);
  assert(g.shields[0][0] == 2);
  g.enemy_bullet = GAME_HEIGHT - 2;
  g.enemy_bullet_x = g.player + 1;
  g.lives = 3;
  g.damage_grace = 0;
  g.flawless_streak = 2;
  game_step(&g);
  assert(g.lives == 2 && g.damage_grace == DAMAGE_GRACE &&
         g.flawless_streak == 0);
  g.enemy_bullet = GAME_HEIGHT - 2;
  game_step(&g);
  assert(g.lives == 2);
  g.kills = 0;
  assert(game_enemy_speed(&g) == 1);
  g.kills = 16;
  assert(game_enemy_speed(&g) == 3);
  g.player = 10;
  assert(game_dash(&g, 1) && g.player == 14 && g.dash_cooldown == DASH_COOLDOWN);
  g.relics = 3;
  assert(game_dash_cooldown(&g) == DASH_COOLDOWN - 6);
  assert(!game_dash(&g, 1) && g.player == 14);
  for (int tick = 0; tick < DASH_COOLDOWN; ++tick) game_tick(&g, 0, false);
  assert(g.dash_cooldown == 0 && game_dash(&g, -1) && g.player == 10);
  g.kills = 1000;
  assert(game_enemy_speed(&g) == 3);

  game_toggle_pause(&g);
  g.emp_ticks = EMP_DURATION;
  g.enemy_bullet = -1;
  game_ai(&g);
  assert(g.enemy_bullet == -1);
  g.emp_ticks = 0;
  g.upgrade_level = 3;
  g.enemy_bullet = -1;
  game_ai(&g);
  assert(g.enemy_bullet == -1);
  int frozen = g.alien;
  game_step(&g);
  assert(g.alien == frozen);
  game_toggle_pause(&g);
  game_init_seed(&g, 42);
  g.bullet = SHIELD_ROW + 2;
  g.bullet_x = 1;
  game_step(&g);
  assert(g.bullet == -1 && g.shields[0][0] == 2);
  game_init_seed(&g, 42);
  g.bullet = g.alien_row + game_player_shot_speed(&g);
  g.bullet_x = g.alien + 1;
  game_step(&g);
  assert(g.score == 1 && g.bullet == -1);
  return 0;
}
