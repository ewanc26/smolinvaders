#include "space_invaders/game.h"
#include "space_invaders/render.h"
#include "space_invaders/terminal.h"

bool input_game(Game *game);

void run_game(Game *game) {
  while (!game->over && input_game(game)) {
    render_game(game); game_step(game);
  }
  render_game(game);
}
