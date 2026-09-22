#include "space_invaders/game.h"

static void room_progress(Game *g) { int next=1+g->score/5; if(next>g->room){g->rng=g->rng*1103515245+12345;g->room=next;g->wave=next;g->room_type=(g->rng&0x7fffffff)%3;g->upgrade_offer=true;g->paused=true;} }

static void step_bullet(Game *g) {
  if (g->bullet < 0) return;
  if (game_shield_hit(g, g->player + 1, g->bullet)) { g->bullet = -1; return; }
  if (g->bonus_active && g->bullet <= 1 && g->player + 1 >= g->bonus_x && g->player <= g->bonus_x + 4) {
      g->score += 3; room_progress(g); g->bonus_active = false; g->bonus_timer = 0; g->bullet = -1; return;
  }
  if (g->bullet == g->alien_row) {
    if (g->player + 1 >= g->alien && g->player <= g->alien + 2) {
      g->score++; room_progress(g); g->rng = g->rng * 1103515245 + 12345; g->alien = (g->rng & 0x7fffffff) % (GAME_WIDTH - 3);
      g->alien_row = 2; g->bullet = -1; return;
    }
  }
  g->bullet--;
}

static void step_enemy_bullet(Game *g){if(g->enemy_bullet<0)return;if(game_shield_hit(g,g->alien+1,g->enemy_bullet)){g->enemy_bullet=-1;return;}if(++g->enemy_bullet>=GAME_HEIGHT-1){if(g->player+1>=g->alien&&g->player<=g->alien+2){if(--g->lives<=0)g->over=true;}g->enemy_bullet=-1;}}
static void step_alien(Game *g) {
  game_ai(g);
  g->alien += g->direction;
  if (g->alien < 0 || g->alien > GAME_WIDTH - 3)
    g->direction = -g->direction, g->alien_row++;
  if (g->alien_row > GAME_HEIGHT - 2) g->over = true;
}

static void step_bonus(Game *g) {
  if (!g->bonus_active) {
    if (++g->bonus_timer >= (g->room_type == ROOM_CACHE ? 90 : 140)) { g->bonus_active = true; g->bonus_timer = 0; g->bonus_x = g->bonus_direction > 0 ? -5 : GAME_WIDTH; }
    return;
  }
  g->bonus_x += g->bonus_direction;
  if (g->bonus_x < -6 || g->bonus_x > GAME_WIDTH) g->bonus_active = false, g->bonus_timer = 0, g->bonus_direction = -g->bonus_direction;
}

void game_step(Game *g) {
  if (!g->over && !g->paused) step_bullet(g), step_enemy_bullet(g), step_alien(g), step_bonus(g);
}
