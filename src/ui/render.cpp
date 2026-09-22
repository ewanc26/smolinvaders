#include "space_invaders/render.h"
#include <cstdio>

void render_game(const Game *g) {
  std::printf("\033[H\033[JScore: %d  a/d move, space fire, q quit\n", g->score);
  for (int y = 0; y < GAME_HEIGHT; ++y) { for (int x = 0; x < GAME_WIDTH; ++x) {
    char c = ' '; if (y == GAME_HEIGHT - 1 && x >= g->player && x < g->player + 3) c = 'A';
    if (y == g->alien_row && x >= g->alien && x < g->alien + 3) c = 'W';
    if (y == g->bullet && x == g->player + 1) c = '|'; std::putchar(c);
  } std::putchar('\n'); }
}
