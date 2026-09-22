#include "space_invaders/gui.h"
#include <cstdio>

void gui_shop(const Gui *gui, const Game *g) {
  constexpr SDL_Color white{225, 232, 245, 255};
  constexpr SDL_Color muted{150, 164, 190, 255};
  constexpr SDL_Color gold{250, 205, 105, 255};
  gui_box(gui, 70, 205, 820, 220, {22, 30, 56, 255});
  gui_text(gui, "BLIND CLEARED - CHOOSE ONE", 236, 224, gold);
  const char *names[] = {"REPAIR", "EXTRA LIFE", "JAMMER",
                         game_module_name(g->module_offer)};
  const char *effects[] = {"Restore shields", "+1 life (max 5)", "Weaken AI",
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
  gui_text(gui, "0 SKIP - bank credits for interest", 290, 389, white);
}
