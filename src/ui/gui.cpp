#include "space_invaders/gui.h"
#include <SDL.h>
bool gui_open(Gui *g){if(SDL_Init(SDL_INIT_VIDEO)<0)return false;g->window=SDL_CreateWindow("Smol Invaders",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,960,640,SDL_WINDOW_SHOWN);g->renderer=g->window?SDL_CreateRenderer(g->window,-1,SDL_RENDERER_ACCELERATED):nullptr;return g->renderer;}
void gui_close(Gui *g){SDL_DestroyRenderer(g->renderer);SDL_DestroyWindow(g->window);SDL_Quit();}
