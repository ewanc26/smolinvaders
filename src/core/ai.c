#include "space_invaders/game.h"

static float squash(float value) {
  float magnitude = value < 0 ? -value : value;
  return value / (1.0f + magnitude);
}

void game_ai(Game *g) {
  int cover = 0;
  for (int shield = 0; shield < SHIELD_COUNT; ++shield)
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell)
      cover += g->shields[shield][cell];
  const float input[5] = {
    (float)(g->player - g->alien) / GAME_WIDTH,
    (float)g->alien_row / GAME_HEIGHT,
    g->bullet < 0 ? 0.0f : 1.0f,
    (float)cover / (SHIELD_COUNT * SHIELD_WIDTH * 3),
    g->emp_ticks ? 1.0f : 0.0f
  };
  const float weights[4][5] = {
    {2.4f, -.4f, -.8f, -.3f, .2f}, {-.9f, .2f, .4f, .7f, -.5f},
    {1.2f, .8f, -.3f, -.9f, -1.0f}, {-1.4f, .5f, .6f, .5f, -.8f}
  };
  float hidden[4];
  for (int i = 0; i < 4; ++i)
    hidden[i] = squash(weights[i][0] * input[0] +
                       weights[i][1] * input[1] + weights[i][2] * input[2] +
                       weights[i][3] * input[3] + weights[i][4] * input[4]);

  float steer = (1.4f * hidden[0] - 1.1f * hidden[1] +
                 .5f * hidden[2] - .8f * hidden[3]) *
                (1.0f + g->wave * .04f +
                 (g->room_type == ROOM_ELITE ? .2f : 0.0f));
  for (int level = 0; level < g->upgrade_level; ++level) steer *= .75f;
  g->ai_mood = (int)(steer * 100);
  if (steer > .12f) g->direction = 1;
  if (steer < -.12f) g->direction = -1;
  if (!g->emp_ticks && g->enemy_bullet < 0 && hidden[2] + hidden[3] > .2f) {
    g->enemy_bullet = g->alien_row + 1;
    g->enemy_bullet_x = g->alien + 1;
  }
}
