#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { GAME_WIDTH = 48, GAME_HEIGHT = 20 };
enum { SHIELD_COUNT = 3, SHIELD_WIDTH = 6, SHIELD_ROW = 15 };
enum { ROOM_COMBAT, ROOM_ELITE, ROOM_CACHE };

typedef struct {
  int player, alien, alien_row, bullet, enemy_bullet, direction;
  int score, ai_mood, lives, wave, rng, bonus_x, bonus_timer, bonus_direction;
  uint32_t seed;
  int room, room_type, upgrade_level;
  bool bonus_active, upgrade_offer;
  uint8_t shields[SHIELD_COUNT][SHIELD_WIDTH];
  bool over, paused;
} Game;

void game_init(Game *game);
void game_init_seed(Game *game, uint32_t seed);
void game_shields_init(Game *game);
void game_restart(Game *game);
void game_toggle_pause(Game *game);
void game_choose_upgrade(Game *game, int choice);
void game_move(Game *game, int direction);
void game_fire(Game *game);
void game_step(Game *game);
void game_ai(Game *game);
int game_ai_mood(const Game *game);
bool game_shield_hit(Game *game, int x, int row);

#ifdef __cplusplus
}
#endif

#endif
