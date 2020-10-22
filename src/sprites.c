#include "sprites.h"
#include "fixedpoint.h"

void init_sprite(sprite_t* sprite, SPRITE_TYPES type, int variant, int radius, int x, int y, int x_vel, int y_vel,
   int rotation, int rot_vel){
  sprite->type = type;
  sprite->variant = variant;
  sprite->radius = to_fixed(radius);
  sprite->x = to_fixed(x);
  sprite->y = to_fixed(y);
  sprite->x_vel = to_fixed(x_vel);
  sprite->y_vel = to_fixed(y_vel);
  sprite->rotation = to_fixed(rotation);
  sprite->rot_vel = to_fixed(rot_vel);
}
void init_sprite_fixed(sprite_t* sprite, SPRITE_TYPES type, int variant, fixed_t radius, fixed_t x, fixed_t y, 
  fixed_t x_vel, fixed_t y_vel, fixed_t rotation, fixed_t rot_vel) {
  sprite->type = type;
  sprite->variant = variant;
  sprite->radius = (radius);
  sprite->x = (x);
  sprite->y = (y);
  sprite->x_vel = (x_vel);
  sprite->y_vel = (y_vel);
  sprite->rotation = rotation;
  sprite->rot_vel = rot_vel;
}

void update_pos_rot(sprite_t* sprite, fixed_t dt){
  sprite->x += mul(sprite->x_vel, dt);
  sprite->y += mul(sprite->y_vel, dt);
  sprite->rotation += mul(sprite->rot_vel, dt);
  sprite->rotation = (sprite->rotation + to_fixed(360)) % to_fixed(360);
} //update pos and rotation after time passes
