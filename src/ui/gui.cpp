#include "space_invaders/gui.h"
#include <cstdlib>
#include <cstdio>

bool gui_open(Gui *gui) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) return false;
  gui->window = SDL_CreateWindow("Smol Invaders", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, GUI_WIDTH, GUI_HEIGHT,
                                 SDL_WINDOW_SHOWN);
  if (!gui->window) return false;
  gui->renderer = SDL_CreateRenderer(gui->window, -1, SDL_RENDERER_ACCELERATED);
  if (!gui->renderer)
    gui->renderer = SDL_CreateRenderer(gui->window, -1, SDL_RENDERER_SOFTWARE);
  if (!gui->renderer) return false;

  const char *fonts[] = {
    std::getenv("SMOLINVADERS_FONT"),
    "/System/Library/Fonts/SFNSMono.ttf",
    "/System/Library/Fonts/Geneva.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "C:/Windows/Fonts/arial.ttf"
  };
  for (const char *path : fonts) {
    if (path && (gui->font = TTF_OpenFont(path, 18))) break;
  }
  if (!gui->font) std::fprintf(stderr, "No font found; set SMOLINVADERS_FONT to a TTF file.\n");
  return gui->font != nullptr;
}

void gui_close(Gui *gui) {
  if (gui->font) TTF_CloseFont(gui->font);
  if (gui->renderer) SDL_DestroyRenderer(gui->renderer);
  if (gui->window) SDL_DestroyWindow(gui->window);
  TTF_Quit();
  SDL_Quit();
}
