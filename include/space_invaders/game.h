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
enum { DASH_DISTANCE = 4, DASH_COOLDOWN = 18 };
enum { DAMAGE_GRACE = 8 };
enum { COMBO_WINDOW = 24, COMBO_CAP = 8 };
enum { MODULE_SLOT_COST = 6, MODULE_START_SLOTS = 3, MODULE_MAX_SLOTS = 6 };
enum { SHIELD_COUNT = 3, SHIELD_WIDTH = 6, SHIELD_ROW = 15 };
enum { ROOM_COMBAT, ROOM_ELITE, ROOM_CACHE };
enum { RUN_ANTES = 8, RUN_BLINDS = RUN_ANTES * 3 };
enum { BOSS_STATIC = 1, BOSS_BREACH = 2, BOSS_BLACKOUT = 4 };
enum { MODULE_AMPLIFIER = 1, MODULE_SIGNAL = 2, MODULE_CADENCE = 4,
       MODULE_AFTERBURNER = 8, MODULE_SCAVENGER = 16, MODULE_BARRIER = 32,
       MODULE_MASK = 63 };

typedef struct {
  int player, alien, alien_row, alien_hp, bullet, bullet_x;
  int enemy_bullet, enemy_bullet_x, direction;
  int score, ai_mood, lives, wave, bonus_x, bonus_timer, bonus_direction;
  uint32_t rng;
  uint32_t seed;
  uint32_t shop_rng;
  int modules, module_offer, module_slots, kills;
  int held_module;
  int shop_bought, shop_rerolls;
  bool route_chosen;
  int emp_charges, emp_ticks;
  int dash_cooldown;
  int damage_grace;
  int combo, combo_timer;
  int boss_rules;
  int room, best_room, room_type, upgrade_level, relics, relic_charges;
  int ante, blind_target, credits;
  bool bonus_active, upgrade_offer, won;
  bool barrier_used;
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
bool game_choose_route(Game *game, int room_type);
int game_upgrade_cost(int choice);
bool game_upgrade_available(const Game *game, int choice);
int game_reroll_cost(const Game *game);
bool game_reroll_available(const Game *game);
void game_reroll_shop(Game *game);
bool game_hold_available(const Game *game);
void game_hold_module(Game *game);
bool game_module_slot_available(const Game *game);
void game_buy_module_slot(Game *game);
bool game_modules_full(const Game *game);
int game_module_count(const Game *game);
void game_move(Game *game, int direction);
void game_fire(Game *game);
void game_step(Game *game);
void game_tick(Game *game, int movement, bool fire);
bool game_dash(Game *game, int direction);
int game_player_shot_speed(const Game *game);
int game_enemy_speed(const Game *game);
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
