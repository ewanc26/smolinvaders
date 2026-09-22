#include "space_invaders/game.h"
#include <assert.h>
int main(void) { Game g; game_init(&g); assert(g.player == 22 && g.bullet == -1 && g.lives == 3 && g.wave == 1); game_move(&g, -1); assert(g.player == 21); game_fire(&g); assert(g.bullet >= 0); game_step(&g); assert(g.ai_mood != 0 || g.enemy_bullet >= 0); for (int i = 0; i < 5; ++i) g.score++, g.wave = 1 + g.score / 5; assert(g.wave == 2); return 0; }
