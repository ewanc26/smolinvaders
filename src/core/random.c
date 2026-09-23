#include "space_invaders/game.h"

uint32_t game_random(Game *g) {
  g->rng = g->rng * 1664525u + 1013904223u;
  return g->rng;
}

uint32_t game_seed_hash(uint32_t seed, uint32_t coordinate) {
  uint32_t value = seed + coordinate * 0x9e3779b9u + 0x85ebca6bu;
  value ^= value >> 16;
  value *= 0x85ebca6bu;
  value ^= value >> 13;
  value *= 0xc2b2ae35u;
  return value ^ (value >> 16);
}
