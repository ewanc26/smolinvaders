#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { GAME_WIDTH = 48, GAME_HEIGHT = 20 };

typedef struct {
  int player, alien, alien_row, bullet, direction, score;
  bool over;
} Game;

void game_init(Game *game);
void game_move(Game *game, int direction);
void game_fire(Game *game);
void game_step(Game *game);

#ifdef __cplusplus
}
#endif

#endif
