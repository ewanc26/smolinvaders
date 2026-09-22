#include "space_invaders/gui.h"
#include <SDL.h>
static void block(SDL_Renderer*r,int x,int y,int w,int h,SDL_Color c){SDL_SetRenderDrawColor(r,c.r,c.g,c.b,255);SDL_Rect q{x,y,w,h};SDL_RenderFillRect(r,&q);}
void gui_render(const Gui*g,const Game*x){SDL_SetRenderDrawColor(g->renderer,8,12,30,255);SDL_RenderClear(g->renderer);constexpr int c=24,l=192,t=72;for(int i=0;i<GAME_WIDTH;i++)for(int j=0;j<GAME_HEIGHT;j++)if((i+j)%7==0)block(g->renderer,l+i*c,t+j*c,2,2,{40,55,90,255});block(g->renderer,l+x->alien*c,t+x->alien_row*c,c*3-4,c-4,{235,75,110,255});block(g->renderer,l+x->player*c,t+(GAME_HEIGHT-1)*c,c*3-4,c-4,{65,220,180,255});if(x->bullet>=0)block(g->renderer,l+(x->player+1)*c+8,t+x->bullet*c,4,c-4,{250,235,120,255});SDL_RenderPresent(g->renderer);}
