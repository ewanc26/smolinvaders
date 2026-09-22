#include "space_invaders/gui.h"
#include <SDL.h>
#include <cstdlib>
int main(int argc,char**argv){Gui gui{};if(!gui_open(&gui))return 1;Game game;if(argc>1)game_init_seed(&game,(uint32_t)std::strtoul(argv[1],nullptr,10));else game_init(&game);char title[64];SDL_snprintf(title,sizeof title,"Smol Invaders - seed %u",game.seed);SDL_SetWindowTitle(gui.window,title);bool run=true;Uint32 last=SDL_GetTicks();while(run&&!game.over&&!game.won){run=gui_input(&gui,&game);Uint32 now=SDL_GetTicks();if(now-last>=70)game_step(&game),last=now;gui_render(&gui,&game);SDL_Delay(8);}gui_close(&gui);}
