#include "space_invaders/game.h"

void game_shields_init(Game *g) {
  for (int shield = 0; shield < SHIELD_COUNT; ++shield) {
    int notch = 1 + game_seed_hash(g->seed, 0x6000u + (uint32_t)shield) %
                (SHIELD_WIDTH - 1);
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell)
      g->shields[shield][cell] = cell == notch ? 2 : 3;
  }
}
bool game_shield_hit(Game*g,int x,int row){if(row!=SHIELD_ROW||x<0||x>=GAME_WIDTH)return false;int s=x/16,c=(x%16)/2;if(c>=SHIELD_WIDTH||!g->shields[s][c])return false;--g->shields[s][c];return true;}
