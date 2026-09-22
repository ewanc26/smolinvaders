#include "space_invaders/gui.h"
#include <cstdio>
void gui_shop(const Gui *gui, const Game *g);

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
  for (int i = 0; i < 3; ++i)
    if (g->modules & (1 << i))
      gui_text(gui, game_module_name(1 << i), GUI_LEFT + i * 240, 112, gold);
  gui_text(gui, "ARROWS move   SPACE fire   P pause   ESC quit",
           GUI_LEFT, 510, muted);
  gui_text(gui, "R replay after a win or defeat", GUI_LEFT, 540, muted);
}

void gui_overlay(const Gui *gui, const Game *g) {
  hud(gui, g);
  if (g->upgrade_offer) gui_shop(gui, g);
  else if (g->won || g->over || g->paused) {
    gui_box(gui, 310, 250, 340, 120, {22, 30, 56, 255});
    gui_text(gui, g->won ? "RUN COMPLETE" : g->over ? "RUN ENDED" : "PAUSED",
             390, 270, g->won ? gold : white);
    gui_text(gui, g->paused && !g->won ? "P to resume" : "R to replay seed",
             390, 310, muted);
  }
}
