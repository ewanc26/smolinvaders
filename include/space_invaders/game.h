#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { GAME_WIDTH = 48, GAME_HEIGHT = 20 };
enum { PLAYER_SHOT_SPEED = 3 };
enum { EMP_DURATION = 12, EMP_CAPACITY = 2 };
enum { SHIELD_COUNT = 3, SHIELD_WIDTH = 6, SHIELD_ROW = 15 };
enum { ROOM_COMBAT, ROOM_ELITE, ROOM_CACHE };
enum { RUN_ANTES = 8, RUN_BLINDS = RUN_ANTES * 3 };
enum { BOSS_STATIC = 1, BOSS_BREACH = 2, BOSS_BLACKOUT = 4 };
enum { MODULE_AMPLIFIER = 1, MODULE_SIGNAL = 2, MODULE_CADENCE = 4,
       MODULE_AFTERBURNER = 8, MODULE_SCAVENGER = 16, MODULE_MASK = 31 };

typedef struct {
  int player, alien, alien_row, alien_hp, bullet, bullet_x;
  int enemy_bullet, enemy_bullet_x, direction;
  int score, ai_mood, lives, wave, bonus_x, bonus_timer, bonus_direction;
  uint32_t rng;
  uint32_t seed;
  uint32_t shop_rng;
  int modules, module_offer, kills;
  int shop_bought, shop_rerolls;
  int emp_charges, emp_ticks;
  int boss_rules;
  int room, room_type, upgrade_level, relics, relic_charges;
  int ante, blind_target, credits;
  bool bonus_active, upgrade_offer, won;
  uint8_t shields[SHIELD_COUNT][SHIELD_WIDTH];
  bool over, paused;
} Game;

void game_init(Game *game);
void game_init_seed(Game *game, uint32_t seed);
void game_shields_init(Game *game);
void game_restart(Game *game);
void game_toggle_pause(Game *game);
void game_choose_upgrade(Game *game, int choice);
void game_skip_upgrade(Game *game);
int game_upgrade_cost(int choice);
bool game_upgrade_available(const Game *game, int choice);
int game_reroll_cost(const Game *game);
bool game_reroll_available(const Game *game);
void game_reroll_shop(Game *game);
void game_move(Game *game, int direction);
void game_fire(Game *game);
void game_step(Game *game);
void game_tick(Game *game, int movement, bool fire);
int game_player_shot_speed(const Game *game);
bool game_use_emp(Game *game);
void game_ai(Game *game);
int game_ai_mood(const Game *game);
bool game_shield_hit(Game *game, int x, int row);
uint32_t game_random(Game *game);
void game_room_progress(Game *game);
bool game_is_boss(const Game *game);
void game_boss_prepare(Game *game);
const char *game_boss_description(const Game *game);
void game_module_offer(Game *game);
void game_score_kill(Game *game, int base, bool saucer);
const char *game_module_name(int module);
const char *game_module_effect(int module);

#ifdef __cplusplus
}
#endif

#endif
