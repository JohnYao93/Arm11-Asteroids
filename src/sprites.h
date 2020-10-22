#ifndef EXTENSION_SPRITES
#define EXTENSION_SPRITES

#include "sprite_graphics.h"
#include "fixedpoint.h"

typedef enum {
  PLAYER = PLAYER_TYPE,
  AST_S = AST_S_TYPE, //small, medium, and large asteroid
  AST_M = AST_M_TYPE,
  AST_L = AST_L_TYPE,
  BULLET_BASE,
  BULLET_MISSILE = BULLET_BASE+1,
  BULLET_MINE    = BULLET_BASE+2
} SPRITE_TYPES;

typedef struct sprite{
  SPRITE_TYPES type;
  int variant;
  fixed_t radius;
  fixed_t x, y; // top left
  fixed_t x_vel;
  fixed_t y_vel;
  fixed_t rotation;
  fixed_t rot_vel;
} sprite_t;

void init_sprite(sprite_t* sprite, SPRITE_TYPES type, int variant, int radius, int x, int y, int x_vel, int y_vel,
   int rotation, int rot_vel);
  
void init_sprite_fixed(sprite_t* sprite, SPRITE_TYPES type, int variant, fixed_t radius, 
    fixed_t x, fixed_t y, fixed_t x_vel, fixed_t y_vel, fixed_t rotation, fixed_t rot_vel);
    
void update_pos_rot(sprite_t* sprite, fixed_t dt); //update pos and rot after time passes

#endif
