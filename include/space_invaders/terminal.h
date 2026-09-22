#ifndef SPACE_INVADERS_TERMINAL_H
#define SPACE_INVADERS_TERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif

void terminal_begin(void);
void terminal_end(void);
int terminal_read(char *key);

#ifdef __cplusplus
}
#endif

#endif
