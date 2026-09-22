#include "space_invaders/gui.h"
#include <SDL.h>
int main(){Gui gui{};if(!gui_open(&gui))return 1;Game game;game_init(&game);bool run=true;Uint32 last=SDL_GetTicks();while(run&&!game.over){run=gui_input(&gui,&game);Uint32 now=SDL_GetTicks();if(now-last>=70)game_step(&game),last=now;gui_render(&gui,&game);SDL_Delay(8);}gui_close(&gui);}
