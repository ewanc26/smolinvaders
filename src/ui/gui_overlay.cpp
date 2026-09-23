#include "space_invaders/gui.h"
#include <cstdio>
void gui_shop(const Gui *gui, const Game *g);

static constexpr SDL_Color white{225, 232, 245, 255};
static constexpr SDL_Color muted{150, 164, 190, 255};
static constexpr SDL_Color gold{250, 205, 105, 255};

static void hud(const Gui *gui, const Game *g) {
  char line[128];
  std::snprintf(line, sizeof line, "ANTE %d/8  BLIND %d/24  %s", g->ante, g->room,
                game_is_boss(g) ? "BOSS" : g->room_type == ROOM_ELITE ? "ELITE" :
                g->room_type == ROOM_CACHE ? "CACHE" : "COMBAT");
  gui_text(gui, line, GUI_LEFT, 20, white);
  std::snprintf(line, sizeof line, "SCORE %d / %d", g->score, g->blind_target);
  gui_text(gui, line, GUI_LEFT, 50, gold);
  std::snprintf(line, sizeof line, "LIVES %d    CREDITS %d    RELIC %d/3",
                g->lives, g->credits,
                g->relics);
  gui_text(gui, line, GUI_LEFT, 80, white);
  std::snprintf(line, sizeof line, "THREAT %dx   COMBO %d   DASH %s   EMP %d/%d %s   GRACE %s",
                game_enemy_speed(g), g->combo, g->dash_cooldown ? "COOL" : "READY",
                g->emp_charges, EMP_CAPACITY,
                g->emp_ticks ? "ACTIVE" : "READY",
                g->damage_grace ? "ACTIVE" : "READY");
  gui_text(gui, line, GUI_LEFT + 430, 80, g->emp_ticks ? gold : muted);
  if (g->modules & MODULE_BARRIER)
    gui_text(gui, g->barrier_used ? "BARRIER USED" : "BARRIER READY",
             GUI_LEFT + 430, 112, g->barrier_used ? muted : gold);
  for (int i = 0; i < 6; ++i)
    if (g->modules & (1 << i))
      gui_text(gui, game_module_name(1 << i), GUI_LEFT + (i % 3) * 240,
               112 + (i / 3) * 22, gold);
  gui_text(gui, "ARROWS move   SPACE fire   P pause   N seed   ESC quit",
           GUI_LEFT, 510, muted);
  std::snprintf(line, sizeof line, "X EMP %d/%d  %s   R replay after run",
                g->emp_charges, EMP_CAPACITY,
                g->emp_ticks ? "ACTIVE" : g->emp_charges ? "READY" : "EMPTY");
  gui_text(gui, line, GUI_LEFT, 540, g->emp_ticks ? gold : muted);
  gui_text(gui, game_boss_description(g), GUI_LEFT, 570, gold);
}

void gui_overlay(const Gui *gui, const Game *g) {
  hud(gui, g);
  if (gui->seed_entry) {
    gui_box(gui, 270, 220, 420, 170, {22, 30, 56, 255});
    gui_text(gui, "NEW SEEDED RUN", 380, 245, gold);
    char line[64];
    std::snprintf(line, sizeof line, "SEED %s", gui->seed_length ? gui->seed_text : "_");
    gui_text(gui, line, 370, 285, white);
    gui_text(gui, "type 0-4294967295   ENTER start   ESC cancel", 305, 330, muted);
  } else if (g->upgrade_offer) gui_shop(gui, g);
  else if (g->won || g->over || g->paused) {
    gui_box(gui, 310, 250, 340, 120, {22, 30, 56, 255});
    gui_text(gui, g->won ? "RUN COMPLETE" : g->over ? "RUN ENDED" : "PAUSED",
             390, 270, g->won ? gold : white);
    if (g->over || g->won) {
      char summary[96];
      std::snprintf(summary, sizeof summary, "SEED %u  SCORE %d  BEST BLIND %d",
                    g->seed, g->score, g->best_room);
      gui_text(gui, summary, 330, 310, muted);
      gui_text(gui, "R replay seed   N new seed", 370, 340, white);
    } else {
      gui_text(gui, "P to resume", 390, 310, muted);
    }
  }
}
