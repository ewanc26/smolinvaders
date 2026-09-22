#include "space_invaders/game.h"
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
  termios old, raw; tcgetattr(0, &old); raw = old;
  raw.c_lflag &= ~(ICANON | ECHO); tcsetattr(0, TCSANOW, &raw);
  Game game; game_init(&game); std::printf("\033[2J\033[?25l");
  while (!game.over) {
    std::printf("\033[HScore: %d  a/d move, space fire, q quit\n", game.score);
    for (int y = 0; y < GAME_HEIGHT; ++y) { for (int x = 0; x < GAME_WIDTH; ++x) {
      char c = ' '; if (y == GAME_HEIGHT - 1 && x >= game.player && x < game.player + 3) c = 'A';
      if (y == game.alien_row && x >= game.alien && x < game.alien + 3) c = 'W';
      if (y == game.bullet && x == game.player + 1) c = '|'; std::putchar(c);
    } std::putchar('\n'); }
    game_tick(&game); fd_set input; FD_ZERO(&input); FD_SET(0, &input);
    timeval wait{0, 70000}; if (select(1, &input, nullptr, nullptr, &wait) > 0) {
      char c; read(0, &c, 1); if (c == 'q') break; if (c == 'a') game_move(&game, -1);
      if (c == 'd') game_move(&game, 1); if (c == ' ') game_fire(&game);
    }
  }
  tcsetattr(0, TCSANOW, &old); std::printf("\033[?25h\nGAME OVER\n");
}
