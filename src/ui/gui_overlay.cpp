#include "space_invaders/gui.h"
#include <cstdio>

static constexpr SDL_Color white{225, 232, 245, 255};
static constexpr SDL_Color muted{150, 164, 190, 255};
static constexpr SDL_Color gold{250, 205, 105, 255};

static void hud(const Gui *gui, const Game *g) {
  char line[128];
  std::snprintf(line, sizeof line, "ANTE %d  BLIND %d  %s", g->ante, g->room,
                g->room == 10 ? "BOSS" : g->room_type == ROOM_ELITE ? "ELITE" :
                g->room_type == ROOM_CACHE ? "CACHE" : "COMBAT");
  gui_text(gui, line, GUI_LEFT, 20, white);
  std::snprintf(line, sizeof line, "SCORE %d / %d", g->score, g->blind_target);
  gui_text(gui, line, GUI_LEFT, 50, gold);
  std::snprintf(line, sizeof line, "LIVES %d    CREDITS %d    RELIC %s",
                g->lives, g->credits,
                !g->relics ? "NONE" : g->relic_charges ? "READY" : "SPENT");
  gui_text(gui, line, GUI_LEFT, 80, white);
  gui_text(gui, "ARROWS move   SPACE fire   P pause   ESC quit",
           GUI_LEFT, 510, muted);
  gui_text(gui, "R replay after a win or defeat", GUI_LEFT, 540, muted);
}

static void shop(const Gui *gui, const Game *g) {
  gui_box(gui, 190, 205, 580, 220, {22, 30, 56, 255});
  gui_text(gui, "BLIND CLEARED  -  CHOOSE AN UPGRADE", 236, 224, gold);
  const char *names[] = {"1 REPAIR", "2 EXTRA LIFE", "3 JAMMER"};
  const char *effects[] = {"Restore shields", "+1 life (max 5)", "Weaken AI"};
  for (int i = 0; i < 3; ++i) {
    const int cost = game_upgrade_cost(i + 1);
    const bool available = game_upgrade_available(g, i + 1);
    int x = 215 + i * 185;
    gui_box(gui, x, 275, 170, 102,
            available ? SDL_Color{53, 73, 110, 255} :
                        SDL_Color{42, 43, 56, 255});
    gui_text(gui, names[i], x + 10, 285, available ? white : muted);
    gui_text(gui, effects[i], x + 10, 312, muted);
    char price[32];
    std::snprintf(price, sizeof price, "%d credits", cost);
    gui_text(gui, price, x + 10, 341, available ? gold : muted);
  }
  gui_text(gui, "0 SKIP - bank credits for interest", 290, 389, white);
}

void gui_overlay(const Gui *gui, const Game *g) {
  hud(gui, g);
  if (g->upgrade_offer) shop(gui, g);
  else if (g->won || g->over || g->paused) {
    gui_box(gui, 310, 250, 340, 120, {22, 30, 56, 255});
    gui_text(gui, g->won ? "RUN COMPLETE" : g->over ? "RUN ENDED" : "PAUSED",
             390, 270, g->won ? gold : white);
    gui_text(gui, g->paused && !g->won ? "P to resume" : "R to replay seed",
             390, 310, muted);
  }
}
