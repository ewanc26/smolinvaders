#ifndef SPACE_INVADERS_GUI_H
#define SPACE_INVADERS_GUI_H
#include "game.h"
#include <SDL.h>
#include <SDL_ttf.h>

enum { GUI_WIDTH = 960, GUI_HEIGHT = 640, GUI_CELL = 16,
       GUI_LEFT = 96, GUI_TOP = 152 };

struct Gui {
  SDL_Window *window{};
  SDL_Renderer *renderer{};
  TTF_Font *font{};
  bool left{}, right{}, fire{};
  bool seed_entry{};
  char seed_text[11]{};
  int seed_length{};
  uint32_t seed{};
};

bool gui_open(Gui *);
void gui_close(Gui *);
bool gui_input(Gui *, Game *);
void gui_tick(Gui *, Game *);
void gui_render(const Gui *, const Game *);
void gui_box(const Gui *, int x, int y, int width, int height, SDL_Color);
void gui_text(const Gui *, const char *, int x, int y, SDL_Color);
#endif
