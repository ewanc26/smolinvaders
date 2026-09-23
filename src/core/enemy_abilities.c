#include "space_invaders/game.h"

int game_enemy_abilities(const Game *g) {
  return (int)(game_seed_hash(g->seed, 0x7000u + (uint32_t)g->room) % 8u);
}

const char *game_enemy_ability_description(const Game *g) {
  int abilities = g->enemy_abilities;
  if (!abilities) return "ABILITIES: NONE";
  if (abilities == ENEMY_HASTE) return "ABILITY: HASTE";
  if (abilities == ENEMY_PIERCING) return "ABILITY: PIERCING";
  if (abilities == ENEMY_VOLLEY) return "ABILITY: VOLLEY";
  return "ABILITIES: STACKED";
}
