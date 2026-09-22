#include "space_invaders/gui.h"
#include <SDL.h>
bool gui_input(Gui *,Game *g){SDL_Event e;while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT)return false;if(e.type==SDL_KEYDOWN&&!e.key.repeat){if(e.key.keysym.sym==SDLK_ESCAPE)return false;if(e.key.keysym.sym==SDLK_LEFT)game_move(g,-1);if(e.key.keysym.sym==SDLK_RIGHT)game_move(g,1);if(e.key.keysym.sym==SDLK_SPACE)game_fire(g);if(e.key.keysym.sym==SDLK_p)game_toggle_pause(g);if(e.key.keysym.sym==SDLK_r&&g->over)game_restart(g);}}return true;}
