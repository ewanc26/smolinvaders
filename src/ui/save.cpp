#include "space_invaders/save.h"
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

namespace {
constexpr long long SAVE_FORMAT = 1;
std::filesystem::path save_file() {
  const char *home = std::getenv("HOME");
  return (home ? std::filesystem::path(home) : std::filesystem::path(".")) /
         ".ewanc26/smolinvaders/save/run.json";
}

bool number(const std::string &json, const char *key, long long &out) {
  std::regex pattern(std::string("\\\"") + key + "\\\"\\s*:\\s*(-?[0-9]+)");
  std::smatch match;
  if (!std::regex_search(json, match, pattern)) return false;
  out = std::stoll(match[1].str());
  return true;
}

bool read_game(const std::string &json, Game *g) {
  long long format;
  if (!number(json, "format", format) || format != SAVE_FORMAT) return false;
#define LOAD_INT(field) do { long long value; if (!number(json, #field, value)) return false; g->field = decltype(g->field)(value); } while (0)
#define LOAD_BOOL(field) do { long long value; if (!number(json, #field, value)) return false; g->field = value != 0; } while (0)
  LOAD_INT(player); LOAD_INT(player_velocity); LOAD_INT(alien); LOAD_INT(alien_row);
  LOAD_INT(alien_hp); LOAD_INT(bullet); LOAD_INT(bullet_x); LOAD_INT(enemy_bullet);
  LOAD_INT(enemy_bullet_x); LOAD_INT(direction); LOAD_INT(enemy_abilities); LOAD_INT(score); LOAD_INT(ai_mood);
  LOAD_INT(lives); LOAD_INT(wave); LOAD_INT(bonus_x); LOAD_INT(bonus_timer);
  LOAD_INT(bonus_direction); LOAD_INT(rng); LOAD_INT(seed); LOAD_INT(shop_rng);
  LOAD_INT(modules); LOAD_INT(module_offer); LOAD_INT(module_slots); LOAD_INT(kills);
  LOAD_INT(held_module); LOAD_INT(shop_bought); LOAD_INT(shop_rerolls);
  LOAD_BOOL(route_chosen); LOAD_INT(emp_charges); LOAD_INT(emp_ticks);
  LOAD_INT(free_rerolls); LOAD_INT(skip_rerolls); LOAD_INT(dash_cooldown);
  LOAD_INT(damage_grace); LOAD_INT(combo); LOAD_INT(combo_timer);
  LOAD_INT(flawless_streak); LOAD_INT(boss_rules); LOAD_INT(room); LOAD_INT(best_room);
  LOAD_INT(room_type); LOAD_INT(upgrade_level); LOAD_INT(relics); LOAD_INT(relic_charges);
  LOAD_INT(ante); LOAD_INT(blind_target); LOAD_INT(credits);
  LOAD_BOOL(bonus_active); LOAD_BOOL(upgrade_offer); LOAD_BOOL(won); LOAD_BOOL(barrier_used);
  LOAD_BOOL(over); LOAD_BOOL(paused);
  for (int shield = 0; shield < SHIELD_COUNT; ++shield)
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell) {
      char key[32];
      std::snprintf(key, sizeof key, "shield_%d_%d", shield, cell);
      long long value;
      if (!number(json, key, value)) return false;
      g->shields[shield][cell] = static_cast<uint8_t>(value);
    }
#undef LOAD_INT
#undef LOAD_BOOL
  return true;
}

void write_number(std::ofstream &file, const char *key, long long value,
                  bool &first) {
  if (!first) file << ",\n";
  first = false;
  file << "  \"" << key << "\": " << value;
}
}

void write_game(std::ofstream &file, const Game *g) {
  bool first = true;
  file << "{\n";
  write_number(file, "format", SAVE_FORMAT, first);
#define SAVE_INT(field) write_number(file, #field, static_cast<long long>(g->field), first)
#define SAVE_BOOL(field) write_number(file, #field, g->field ? 1 : 0, first)
  SAVE_INT(player); SAVE_INT(player_velocity); SAVE_INT(alien); SAVE_INT(alien_row);
  SAVE_INT(alien_hp); SAVE_INT(bullet); SAVE_INT(bullet_x); SAVE_INT(enemy_bullet);
  SAVE_INT(enemy_bullet_x); SAVE_INT(direction); SAVE_INT(enemy_abilities); SAVE_INT(score); SAVE_INT(ai_mood);
  SAVE_INT(lives); SAVE_INT(wave); SAVE_INT(bonus_x); SAVE_INT(bonus_timer);
  SAVE_INT(bonus_direction); SAVE_INT(rng); SAVE_INT(seed); SAVE_INT(shop_rng);
  SAVE_INT(modules); SAVE_INT(module_offer); SAVE_INT(module_slots); SAVE_INT(kills);
  SAVE_INT(held_module); SAVE_INT(shop_bought); SAVE_INT(shop_rerolls);
  SAVE_BOOL(route_chosen); SAVE_INT(emp_charges); SAVE_INT(emp_ticks);
  SAVE_INT(free_rerolls); SAVE_INT(skip_rerolls); SAVE_INT(dash_cooldown);
  SAVE_INT(damage_grace); SAVE_INT(combo); SAVE_INT(combo_timer);
  SAVE_INT(flawless_streak); SAVE_INT(boss_rules); SAVE_INT(room); SAVE_INT(best_room);
  SAVE_INT(room_type); SAVE_INT(upgrade_level); SAVE_INT(relics); SAVE_INT(relic_charges);
  SAVE_INT(ante); SAVE_INT(blind_target); SAVE_INT(credits);
  SAVE_BOOL(bonus_active); SAVE_BOOL(upgrade_offer); SAVE_BOOL(won); SAVE_BOOL(barrier_used);
  SAVE_BOOL(over); SAVE_BOOL(paused);
  for (int shield = 0; shield < SHIELD_COUNT; ++shield)
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell) {
      char key[32];
      std::snprintf(key, sizeof key, "shield_%d_%d", shield, cell);
      write_number(file, key, g->shields[shield][cell], first);
    }
#undef SAVE_INT
#undef SAVE_BOOL
  file << "\n}\n";
}
bool game_save(const Game *game) {
  std::error_code error;
  auto path = save_file();
  std::filesystem::create_directories(path.parent_path(), error);
  if (error) return false;
  auto temporary = path;
  temporary += ".tmp";
  std::ofstream file(temporary, std::ios::trunc);
  if (!file) return false;
  write_game(file, game);
  if (!file.good()) return false;
  file.close();
  std::filesystem::rename(temporary, path, error);
  return !error;
}

bool game_load(Game *game) {
  std::ifstream file(save_file());
  if (!file) return false;
  std::string json((std::istreambuf_iterator<char>(file)), {});
  Game loaded = *game;
  if (!read_game(json, &loaded)) return false;
  *game = loaded;
  return true;
}
