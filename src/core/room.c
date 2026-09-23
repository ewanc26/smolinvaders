#include "space_invaders/game.h"

void game_room_progress(Game *g) {
  if (g->over || g->won || g->upgrade_offer ||
      (g->score < g->blind_target &&
       !(game_is_boss(g) && g->alien_hp <= 0)) ||
      (game_is_boss(g) && g->alien_hp > 0)) return;
  if (g->room == RUN_BLINDS) {
    g->won = true;
    return;
  }

  bool cache_clear = g->room_type == ROOM_CACHE;
  int interest = game_interest(g);
  int combo_reward = g->combo / 4;
  if (combo_reward > 2) combo_reward = 2;
  g->credits += 2 + interest + combo_reward;
  if (game_room_tag(g) == ROOM_TAG_RICH) ++g->credits;

  ++g->room;
  if (cache_clear) ++g->free_rerolls;
  if (g->room > g->best_room) g->best_room = g->room;
  g->wave = g->room;
  g->ante = 1 + (g->room - 1) / 3;
  g->blind_target = g->score + 4 + g->ante;
  g->room_type = game_seed_hash(g->seed, (uint32_t)g->room * 3u) % 3;
  g->alien_hp = g->room_type == ROOM_ELITE ? 3 : 1;
  g->enemy_abilities = game_enemy_abilities(g);
  game_boss_prepare(g);
  g->alien_row = 2;
  g->alien = game_seed_hash(g->seed, (uint32_t)g->room * 3u + 1u) %
             (GAME_WIDTH - 3);
  g->bonus_direction = game_seed_hash(g->seed, (uint32_t)g->room * 3u + 3u) & 1
                           ? 1 : -1;
  g->enemy_bullet = -1;
  g->player_velocity = 0;
  g->relic_charges = g->relics;
  g->bullet = -1;
  g->bonus_active = false;
  g->bonus_timer = 0;
  g->emp_ticks = 0;
  g->dash_cooldown = 0;
  g->damage_grace = 0;
  g->combo = g->combo_timer = 0;
  g->barrier_used = false;
  g->upgrade_offer = true;
  g->shop_bought = g->shop_rerolls = g->module_offer = 0;
  g->route_chosen = false;
  game_module_offer(g);
  g->paused = true;
}
