#include "space_invaders/game.h"

uint32_t game_random(Game *g) {
  g->rng = g->rng * 1664525u + 1013904223u;
  return g->rng;
}
