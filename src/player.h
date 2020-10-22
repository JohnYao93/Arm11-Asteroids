#ifndef EXTENSION_PLAYER
#define EXTENSION_PLAYER

#include "sprites.h"
#include "fixedpoint.h"

#define MAX_SPEED 20
#define MAX_SPEED_SQ (MAX_SPEED * MAX_SPEED)

typedef struct player{
  sprite_t sprite;
  int score;
  int lives;
  bool firedLastFrame;
  int cooldown;

  int invincibilityFrames;

  int numMissiles, numMines;
} player_t;

// initalize at center of screen
void init_player(player_t* player, int x, int y);

// update player position & rotation
void update_player(player_t* player, sprite_t* asteroids, int noAsteroids, int xJoy, int yJoy);

// reset on death
void reset(player_t* player);

#endif
