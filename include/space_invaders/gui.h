#ifndef SPACE_INVADERS_GUI_H
#define SPACE_INVADERS_GUI_H
#include "game.h"
struct SDL_Renderer; struct SDL_Window;
struct Gui { SDL_Window *window; SDL_Renderer *renderer; };
bool gui_open(Gui *); void gui_close(Gui *); bool gui_input(Gui *, Game *); void gui_render(const Gui *, const Game *);
#endif
