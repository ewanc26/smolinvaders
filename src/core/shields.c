#include "space_invaders/game.h"
void game_shields_init(Game*g){for(int i=0;i<SHIELD_COUNT;i++)for(int x=0;x<SHIELD_WIDTH;x++)g->shields[i][x]=3;}
bool game_shield_hit(Game*g,int x,int row){if(row!=SHIELD_ROW||x<0||x>=GAME_WIDTH)return false;int s=x/16,c=(x%16)/2;if(c>=SHIELD_WIDTH||!g->shields[s][c])return false;--g->shields[s][c];return true;}
