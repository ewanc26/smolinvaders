#include "space_invaders/game.h"
#include "space_invaders/terminal.h"

bool input_game(Game *game) {
  char key;
  if (!terminal_read(&key)) return true;
  if (key == 'q') return false;
  if (key == 'a') game_move(game, -1);
  if (key == 'd') game_move(game, 1);
  if (key == ' ') game_fire(game);
  return true;
}
