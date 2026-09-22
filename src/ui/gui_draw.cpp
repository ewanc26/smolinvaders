#include "space_invaders/gui.h"

void gui_box(const Gui *gui, int x, int y, int width, int height, SDL_Color color) {
  SDL_SetRenderDrawColor(gui->renderer, color.r, color.g, color.b, color.a);
  SDL_Rect rect{x, y, width, height};
  SDL_RenderFillRect(gui->renderer, &rect);
}

void gui_text(const Gui *gui, const char *text, int x, int y, SDL_Color color) {
  SDL_Surface *surface = TTF_RenderUTF8_Blended(gui->font, text, color);
  if (!surface) return;
  SDL_Texture *texture = SDL_CreateTextureFromSurface(gui->renderer, surface);
  SDL_Rect target{x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  if (!texture) return;
  SDL_RenderCopy(gui->renderer, texture, nullptr, &target);
  SDL_DestroyTexture(texture);
}
