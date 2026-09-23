#include "space_invaders/game.h"

static void step_player_shot(Game *g) {
  if (g->bullet < 0) return;
  --g->bullet;
  if (g->bullet < 0) return;
  if (game_shield_hit(g, g->bullet_x, g->bullet)) {
    g->bullet = -1;
    return;
  }
  if (g->bonus_active && g->bullet == 0 &&
      g->bullet_x >= g->bonus_x && g->bullet_x < g->bonus_x + 5) {
    game_score_kill(g, 3, true);
    g->credits += 2;
    g->bonus_active = false;
    g->bonus_timer = 0;
    g->bullet = -1;
    game_room_progress(g);
    return;
  }
  if (g->bullet != g->alien_row || g->bullet_x < g->alien ||
      g->bullet_x >= g->alien + 3) return;

  g->bullet = -1;
  if (--g->alien_hp > 0) return;
  if (game_is_boss(g)) {
    g->relics |= 1;
    g->relic_charges = 1;
    g->credits += 3;
    game_score_kill(g, 4 + g->ante, false);
    game_room_progress(g);
    return;
  }
  if (g->room_type == ROOM_ELITE) {
    g->relics |= 1;
    g->relic_charges = 1;
    g->credits += 3;
  }
  game_score_kill(g, 1, false);
  game_room_progress(g);
  if (!g->upgrade_offer) {
    g->alien = game_random(g) % (GAME_WIDTH - 3);
    g->alien_row = 2;
    g->alien_hp = g->room_type == ROOM_ELITE ? 3 : 1;
  }
}

static void step_enemy_shot(Game *g) {
  if (g->enemy_bullet < 0) return;
  ++g->enemy_bullet;
  if (!(g->boss_rules & BOSS_BREACH) &&
      game_shield_hit(g, g->enemy_bullet_x, g->enemy_bullet)) {
    g->enemy_bullet = -1;
    return;
  }
  if (g->enemy_bullet < GAME_HEIGHT - 1) return;
  if (g->enemy_bullet_x >= g->player && g->enemy_bullet_x < g->player + 3) {
    if (g->relic_charges) --g->relic_charges;
    else if (--g->lives == 0) g->over = true;
  }
  g->enemy_bullet = -1;
}

static void step_alien(Game *g) {
  game_ai(g);
  g->alien += g->direction;
  if (g->alien < 0 || g->alien > GAME_WIDTH - 3) {
    g->alien = g->alien < 0 ? 0 : GAME_WIDTH - 3;
    g->direction = -g->direction;
    ++g->alien_row;
  }
  if (g->alien_row >= GAME_HEIGHT - 1) g->over = true;
}

static void step_bonus(Game *g) {
  if (g->boss_rules & BOSS_BLACKOUT) return;
  if (!g->bonus_active) {
    if (++g->bonus_timer >= (g->room_type == ROOM_CACHE ? 90 : 140)) {
      g->bonus_active = true;
      g->bonus_timer = 0;
      g->bonus_x = g->bonus_direction > 0 ? -5 : GAME_WIDTH;
    }
    return;
  }
  g->bonus_x += g->bonus_direction;
  if (g->bonus_x < -5 || g->bonus_x >= GAME_WIDTH) {
    g->bonus_active = false;
    g->bonus_direction = -g->bonus_direction;
  }
}

void game_step(Game *g) {
  if (g->over || g->won || g->paused) return;
  for (int cell = 0; cell < game_player_shot_speed(g); ++cell) {
    step_player_shot(g);
    if (g->over || g->won || g->paused) return;
  }
  if (g->emp_ticks > 0) {
    --g->emp_ticks;
    return;
  }
  step_enemy_shot(g);
  if (g->over) return;
  step_alien(g);
  if (!g->over) step_bonus(g);
}
