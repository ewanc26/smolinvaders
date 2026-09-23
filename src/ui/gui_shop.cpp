#include "space_invaders/gui.h"
#include <cstdio>

void gui_shop(const Gui *gui, const Game *g) {
  constexpr SDL_Color white{225, 232, 245, 255};
  constexpr SDL_Color muted{150, 164, 190, 255};
  constexpr SDL_Color gold{250, 205, 105, 255};
  gui_box(gui, 70, 205, 820, 260, {22, 30, 56, 255});
  gui_text(gui, "BLIND CLEARED - SHOP", 236, 224, gold);
  const char *names[] = {"REPAIR", "EXTRA LIFE", "JAMMER",
                         game_module_name(g->module_offer)};
  const char *effects[] = {"Restore shields", "+1 life (max 5)", "Weaken AI",
                           game_modules_full(g) ? "FULL: buy M or sell" :
                           game_module_effect(g->module_offer)};
  for (int i = 0; i < 4; ++i) {
    bool available = game_upgrade_available(g, i + 1);
    int x = 85 + i * 200;
    gui_box(gui, x, 275, 190, 102, available ? SDL_Color{53, 73, 110, 255} :
                                                           SDL_Color{42, 43, 56, 255});
    char line[40];
    std::snprintf(line, sizeof line, "%d %s", i + 1, names[i]);
    gui_text(gui, line, x + 8, 285, available ? white : muted);
    gui_text(gui, effects[i], x + 8, 312, muted);
    std::snprintf(line, sizeof line, "%d credits", game_upgrade_cost(i + 1));
    gui_text(gui, line, x + 8, 341, available ? gold : muted);
  }
  char reroll[48];
  if (g->free_rerolls)
    std::snprintf(reroll, sizeof reroll, "5 REROLL: FREE CACHE REWARD");
  else if (g->skip_rerolls)
    std::snprintf(reroll, sizeof reroll, "5 REROLL: FREE SKIP TAG");
  else
    std::snprintf(reroll, sizeof reroll, "5 REROLL: %d credits",
                  game_reroll_cost(g));
  gui_text(gui, reroll, 95, 389, game_reroll_available(g) ? gold : muted);
  gui_text(gui, "H HOLD MODULE: 2 credits", 475, 405,
           game_hold_available(g) ? gold : muted);
  gui_text(gui, "SHIFT+1..6 SELL MODULE: +2 credits", 475, 420, muted);
  char slots[64];
  std::snprintf(slots, sizeof slots, "M MODULE SLOT: %d credits (%d/%d)",
                game_module_slot_cost(g), game_module_count(g),
                MODULE_MAX_SLOTS);
  gui_text(gui, slots, 95, 405,
           game_module_slot_available(g) ? gold : muted);
  gui_text(gui, "0 LEAVE - keep unspent credits", 475, 389, white);
  char emp[80];
  std::snprintf(emp, sizeof emp, "6 EMP: %d credits - clear shot + freeze enemies (carry %d)",
                game_upgrade_cost(6), EMP_CAPACITY);
  gui_text(gui, emp, 95, 430, game_upgrade_available(g, 6) ? gold : muted);
  if (g->route_chosen) {
    const char *route = g->room_type == ROOM_ELITE ? "ELITE" :
                        g->room_type == ROOM_CACHE ? "CACHE" : "COMBAT";
    char locked[64];
    std::snprintf(locked, sizeof locked, "ROUTE LOCKED: %s", route);
    gui_text(gui, locked, 475, 430, gold);
  } else if (g->room % 3 != 0)
    gui_text(gui, "NEXT BLIND: 7 COMBAT   8 ELITE   9 CACHE", 475, 430, white);
  else
    gui_text(gui, "NEXT BLIND: BOSS (route locked)", 475, 430, gold);
}
