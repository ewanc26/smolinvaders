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

  int interest = g->credits / 5;
  if (interest > 3) interest = 3;
  g->credits += 2 + interest;

  ++g->room;
  g->wave = g->room;
  g->ante = 1 + (g->room - 1) / 3;
  g->blind_target = g->score + 4 + g->ante;
  g->room_type = game_random(g) % 3;
  g->alien_hp = g->room_type == ROOM_ELITE ? 3 : 1;
  game_boss_prepare(g);
  g->alien_row = 2;
  g->alien = game_random(g) % (GAME_WIDTH - 3);
  g->enemy_bullet = -1;
  g->relic_charges = g->relics ? 1 : 0;
  g->bullet = -1;
  g->bonus_active = false;
  g->bonus_timer = 0;
  g->emp_ticks = 0;
  g->upgrade_offer = true;
  g->shop_bought = g->shop_rerolls = g->module_offer = 0;
  g->route_chosen = false;
  game_module_offer(g);
  g->paused = true;
}
