#include "space_invaders/gui.h"

void gui_overlay(const Gui *, const Game *);

void gui_render(const Gui *gui, const Game *g) {
  constexpr SDL_Color background{8, 12, 30, 255};
  SDL_SetRenderDrawColor(gui->renderer, background.r, background.g,
                         background.b, background.a);
  SDL_RenderClear(gui->renderer);

  for (int y = 0; y < GAME_HEIGHT; ++y)
    for (int x = 0; x < GAME_WIDTH; ++x)
      if ((x + y) % 11 == 0)
        gui_box(gui, GUI_LEFT + x * GUI_CELL, GUI_TOP + y * GUI_CELL,
                2, 2, {36, 48, 78, 255});

  for (int s = 0; s < SHIELD_COUNT; ++s)
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell)
      if (g->shields[s][cell])
        gui_box(gui, GUI_LEFT + (s * 16 + cell * 2) * GUI_CELL,
                GUI_TOP + static_cast<int>(SHIELD_ROW) * GUI_CELL, GUI_CELL * 2 - 2,
                GUI_CELL - 2, {70, (Uint8)(110 + g->shields[s][cell] * 30), 150, 255});

  if (g->bonus_active)
    gui_box(gui, GUI_LEFT + g->bonus_x * GUI_CELL, GUI_TOP,
            GUI_CELL * 5 - 2, GUI_CELL - 2, {250, 190, 70, 255});
  SDL_Color enemy = g->room_type == ROOM_ELITE ? SDL_Color{255, 80, 180, 255} :
                    g->room_type == ROOM_CACHE ? SDL_Color{120, 180, 255, 255} :
                    SDL_Color{235, 75, 110, 255};
  gui_box(gui, GUI_LEFT + g->alien * GUI_CELL,
          GUI_TOP + g->alien_row * GUI_CELL,
          GUI_CELL * 3 - 2, GUI_CELL - 2, enemy);
  for (int hp = 0; hp < g->alien_hp; ++hp)
    gui_box(gui, GUI_LEFT + g->alien * GUI_CELL + hp * 9,
            GUI_TOP + g->alien_row * GUI_CELL - 7, 6, 4,
            {255, 235, 120, 255});
  gui_box(gui, GUI_LEFT + g->player * GUI_CELL,
          GUI_TOP + (GAME_HEIGHT - 1) * GUI_CELL,
          GUI_CELL * 3 - 2, GUI_CELL - 2, {65, 220, 180, 255});
  if (g->bullet >= 0)
    gui_box(gui, GUI_LEFT + g->bullet_x * GUI_CELL + 6,
            GUI_TOP + g->bullet * GUI_CELL, 4, GUI_CELL - 2,
            {250, 235, 120, 255});
  if (g->enemy_bullet >= 0)
    gui_box(gui, GUI_LEFT + g->enemy_bullet_x * GUI_CELL + 6,
            GUI_TOP + g->enemy_bullet * GUI_CELL, 4, GUI_CELL - 2,
            {255, 110, 80, 255});

  gui_overlay(gui, g);
  SDL_RenderPresent(gui->renderer);
}
