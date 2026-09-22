#include "space_invaders/terminal.h"
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static termios original;

void terminal_begin(void) {
  termios raw; tcgetattr(0, &original); raw = original;
  raw.c_lflag &= ~(ICANON | ECHO); tcsetattr(0, TCSANOW, &raw);
  write(1, "\033[2J\033[?25l", 10);
}

void terminal_end(void) {
  tcsetattr(0, TCSANOW, &original); write(1, "\033[?25h\n", 8);
}

int terminal_read(char *key) {
  fd_set input; FD_ZERO(&input); FD_SET(0, &input);
  timeval wait{0, 70000};
  return select(1, &input, nullptr, nullptr, &wait) > 0 && read(0, key, 1) == 1;
}
