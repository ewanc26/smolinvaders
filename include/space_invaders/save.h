#ifndef SPACE_INVADERS_SAVE_H
#define SPACE_INVADERS_SAVE_H

#include "game.h"

#ifdef __cplusplus
bool game_save(const Game *game);
bool game_load(Game *game);
#endif

#endif
