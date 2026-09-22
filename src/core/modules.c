#include "space_invaders/game.h"

void game_module_offer(Game *g) {
  int pool[3], count = 0;
  for (int bit = 1; bit <= MODULE_CADENCE; bit <<= 1)
    if (!(g->modules & bit)) pool[count++] = bit;
  g->shop_rng = g->shop_rng * 1664525u + 1013904223u;
  g->module_offer = count ? pool[g->shop_rng % (unsigned)count] : 0;
}

void game_score_kill(Game *g, int base, bool saucer) {
  ++g->kills;
  if (g->modules & MODULE_AMPLIFIER) ++base;
  if ((g->modules & MODULE_CADENCE) && g->kills % 3 == 0) base += 2;
  if ((g->modules & MODULE_SIGNAL) && saucer) base *= 2;
  g->score += base;
}

const char *game_module_name(int module) {
  switch (module) {
    case MODULE_AMPLIFIER: return "AMPLIFIER";
    case MODULE_SIGNAL: return "SIGNAL";
    case MODULE_CADENCE: return "CADENCE";
    default: return "SOLD OUT";
  }
}

const char *game_module_effect(int module) {
  switch (module) {
    case MODULE_AMPLIFIER: return "+1 per kill";
    case MODULE_SIGNAL: return "2x saucer score";
    case MODULE_CADENCE: return "+2 every 3 kills";
    default: return "All collected";
  }
}
