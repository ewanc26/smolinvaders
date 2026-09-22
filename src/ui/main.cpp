#include "space_invaders/game.h"
#include "space_invaders/terminal.h"

void run_game(Game *game);

int main() {
  Game game; game_init(&game); terminal_begin(); run_game(&game); terminal_end();
}
