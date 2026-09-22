#include "space_invaders/game.h"

void game_room_progress(Game *g) {
  if (g->room >= 10 || g->score < g->blind_target) return;

  int interest = g->credits / 5;
  if (interest > 3) interest = 3;
  g->credits += 2 + interest;

  ++g->room;
  g->wave = g->room;
  g->ante = 1 + (g->room - 1) / 3;
  g->blind_target = g->room * 5;
  g->room_type = g->room == 10 ? ROOM_ELITE : game_random(g) % 3;
  g->alien_hp = g->room == 10 ? 5 : g->room_type == ROOM_ELITE ? 3 : 1;
  g->alien_row = 2;
  g->alien = game_random(g) % (GAME_WIDTH - 3);
  g->enemy_bullet = -1;
  g->relic_charges = g->relics ? 1 : 0;
  g->bullet = -1;
  g->bonus_active = false;
  g->upgrade_offer = true;
  g->paused = true;
}
