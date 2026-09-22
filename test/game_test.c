#include "space_invaders/game.h"
#include <assert.h>
int main(void) { Game g; game_init(&g); assert(g.player == 22 && g.bullet == -1); game_move(&g, -1); assert(g.player == 21); game_fire(&g); assert(g.bullet >= 0); return 0; }
