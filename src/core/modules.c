#include "space_invaders/game.h"

void game_module_offer(Game *g) {
  if (g->held_module) {
    g->module_offer = g->held_module;
    g->held_module = 0;
    return;
  }
  int pool[7], count = 0;
  for (int bit = 1; bit <= MODULE_OVERCLOCK; bit <<= 1)
    if (!(g->modules & bit) && bit != g->module_offer) pool[count++] = bit;
  g->shop_rng = game_seed_hash(g->seed,
                               0x1000u + (uint32_t)g->room * 16u +
                                   (uint32_t)g->shop_rerolls);
  g->module_offer = count ? pool[g->shop_rng % (unsigned)count] : 0;
}

void game_score_kill(Game *g, int base, bool saucer) {
  ++g->kills;
  if (g->combo_timer == 0) g->combo = 0;
  if (g->combo < COMBO_CAP) ++g->combo;
  g->combo_timer = game_combo_window(g);
  int active = g->boss_rules & BOSS_STATIC ? 0 : g->modules;
  if (active & MODULE_AMPLIFIER) ++base;
  if ((active & MODULE_CADENCE) && g->kills % 3 == 0) base += 2;
  if ((active & MODULE_SIGNAL) && saucer) base *= 2;
  if ((active & (MODULE_AMPLIFIER | MODULE_SIGNAL)) ==
      (MODULE_AMPLIFIER | MODULE_SIGNAL) && saucer)
    base += 1;
  if ((active & (MODULE_AMPLIFIER | MODULE_CADENCE)) ==
      (MODULE_AMPLIFIER | MODULE_CADENCE) && g->kills % 4 == 0)
    base += 2;
  if ((active & MODULE_OVERCLOCK) && g->kills % 5 == 0) base += 3;
  base *= game_combo_multiplier(g);
  g->score += base;
  if (saucer && (active & MODULE_SCAVENGER) && g->emp_charges < EMP_CAPACITY)
    ++g->emp_charges;
  if (saucer && (active & (MODULE_SIGNAL | MODULE_SCAVENGER)) ==
      (MODULE_SIGNAL | MODULE_SCAVENGER)) ++g->credits;
}

int game_combo_multiplier(const Game *g) {
  int multiplier = 1 + g->combo / 4;
  return multiplier > 3 ? 3 : multiplier;
}

int game_combo_window(const Game *g) {
  int window = COMBO_WINDOW + g->relics * 2;
  return window > COMBO_WINDOW + 6 ? COMBO_WINDOW + 6 : window;
}

const char *game_module_name(int module) {
  switch (module) {
    case MODULE_AMPLIFIER: return "AMPLIFIER";
    case MODULE_SIGNAL: return "SIGNAL";
    case MODULE_CADENCE: return "CADENCE";
    case MODULE_AFTERBURNER: return "AFTERBURNER";
    case MODULE_SCAVENGER: return "SCAVENGER";
    case MODULE_BARRIER: return "BARRIER";
    case MODULE_OVERCLOCK: return "OVERCLOCK";
    default: return "SOLD OUT";
  }
}

const char *game_module_effect(int module) {
  switch (module) {
    case MODULE_AMPLIFIER: return "+1 per kill";
    case MODULE_SIGNAL: return "2x saucer score";
    case MODULE_CADENCE: return "+2 every 3 kills";
    case MODULE_AFTERBURNER: return "+1 shot speed";
    case MODULE_SCAVENGER: return "saucers refill EMP";
    case MODULE_BARRIER: return "negate first shield hit";
    case MODULE_OVERCLOCK: return "+3 every 5 kills";
    default: return "All collected";
  }
}
