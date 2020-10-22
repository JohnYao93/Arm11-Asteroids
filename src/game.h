#ifndef EXTENSION_RUN_GAME
#define EXTENSION_RUN_GAME

#include "sprites.h"
#include "player.h"
#include "input/input.h"

#define MAX_SPRITES 100
#define MAX_BULLETS 100

typedef struct __gamestate {
  unsigned long long rand_seed;
  sprite_t sprites[MAX_SPRITES];
  sprite_t bullets[MAX_BULLETS];
  int noSprites, noBullets;
  player_t player;

  int joystickXPos;
  int joystickYPos;
  BTN_STATE btnWhite;
  BTN_STATE btnGreen;
  BTN_STATE btnRed;

  fixed_t MISSILE_MINE_RADIUS;
  fixed_t explosionRadius;
  int explosionX;
  int explosionY;

  int regainMissileCounter;
} gamestate_t;

void draw_asteroid(display_t* display, sprite_t* sprite, bool blackout);
void draw_player  (display_t* display, sprite_t* sprite, bool blackout);
void draw_bullet(display_t* display, sprite_t* sprite, bool blackout);
bool update_asteroid(display_t* display, sprite_t* sprite, gamestate_t* game);
bool update_bullet  (display_t* display, sprite_t* sprite, gamestate_t* game);

void split_asteroid(gamestate_t* game, int i);

void check_bounds(display_t* display, sprite_t* sprite);

void init_game(gamestate_t* ret, int px, int py);
void draw_game(display_t* display, gamestate_t* game, bool blackout);
void update_game(display_t* display, gamestate_t* game);
void draw_bullet(display_t* display, sprite_t* sprite, bool blackout);
void draw_hud(display_t* display, player_t* player);

void add_sprite(gamestate_t* game, SPRITE_TYPES type, int variant, int radius, int x, int y, int x_vel, int y_vel,
   int rotation, int rot_vel);
void add_sprite_fixed(gamestate_t* game, SPRITE_TYPES type, int variant, fixed_t radius, fixed_t x, fixed_t y, fixed_t x_vel, fixed_t y_vel,
   fixed_t rotation, fixed_t rot_vel);
void add_bullet(gamestate_t* game, int type);

void copy_game(const gamestate_t* src, gamestate_t* dst);

void waitForFrame(gamestate_t* game);

void gameOverScreen(display_t* display, gamestate_t* game);

#endif
