#include "player.h"
#include "sprites.h"
#include "fixedpoint.h"
#include "sprite_graphics.h"
#include "game.h"

#include "input/input.h"

void init_player(player_t* player, int x, int y){
  init_sprite(&player->sprite, PLAYER, 0, PLAYER_SIZE >> 1, x, y, 0, 0, 0, 0);
  player->score = 0;
  player->lives = 3;
  player->firedLastFrame = false;
  player->cooldown = -1;
  player->invincibilityFrames = -1;

  player->numMissiles = 30;
  player->numMines    = 10;
}

void update_player(player_t* player, sprite_t* asteroids, int noAsteroids, int xJoy, int yJoy) {

  if (player->invincibilityFrames == -1) {
    for (int i = 0; i < noAsteroids; ++i) {
      const sprite_t ast = asteroids[i];
      const fixed_t radius = ast.radius;
      const fixed_t absx = abs(ast.x-player->sprite.x);
      if (absx > radius) continue;
      const fixed_t absy = abs(ast.y-player->sprite.y);
      if (absy > radius) continue;
      if (mul(absx, absx) + mul(absy, absy) < mul(radius, radius)) {
        // Player ded
          player->invincibilityFrames = 60;
          player->lives--;
          break;
        }
      }
    }
    player->invincibilityFrames -= (player->invincibilityFrames != -1);

  fixed_t xAcc = ( xJoy << 8); // joystick value/256
  fixed_t yAcc = (-yJoy << 8); // joystick value/256

  fixed_t x_vel = player->sprite.x_vel + xAcc - (player->sprite.x_vel >> 6); // -1/64th player vel
  fixed_t y_vel = player->sprite.y_vel + yAcc - (player->sprite.y_vel >> 6);
  const fixed_t vel_sq = mul(x_vel, x_vel) + mul(y_vel, y_vel);

  if (vel_sq > (MAX_SPEED_SQ << 16)) {
    // multiply both by finvsqrt(vel_sq) * MAX_SPEED - cap to max speed
    const fixed_t inv_vel_sq = finvsqrt(vel_sq);
    x_vel = mul(mul(x_vel, inv_vel_sq), (MAX_SPEED << 16));
    y_vel = mul(mul(y_vel, inv_vel_sq), (MAX_SPEED << 16));
  }

  player->sprite.x_vel = x_vel;
  player->sprite.y_vel = y_vel;


  if (abs(xAcc) + abs(yAcc) > (1 << 15)) {
    player->sprite.rotation = mul(atan2(yAcc, xAcc), radtodeg) + (180 << 16);
  }
  player->sprite.rot_vel = 0;

  update_pos_rot(&player->sprite, to_fixed(1));
}

//reset on death
void reset(player_t* player){
  (player->lives)--;
  init_sprite(&player->sprite, PLAYER, 0, PLAYER_SIZE >> 1, 0 ,0 ,0, 0 ,0, 0);
}
