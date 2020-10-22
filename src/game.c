#include "sprites.h"
#include "sprite_graphics.h"
#include "lfb.h"
#include "fixedpoint.h"
#include "player.h"
#include "game.h"
#include "random.h"

#define SCORE_FONT_SCALE  2

void init_game(gamestate_t* ret, int px, int py) {
  init_player(&ret->player, px, py);
  ret->noBullets = 0;
  ret->noSprites = 0;
  ret->rand_seed = 0xDEADBEEF;
  ret->MISSILE_MINE_RADIUS = 150;
  ret->explosionX = -1;
  ret->explosionY = -1;
  ret->regainMissileCounter = 700;
  ret->explosionRadius = -1;
}

void copy_game(const gamestate_t* src, gamestate_t* dst) {
  dst->noBullets = src->noBullets;
  dst->noSprites = src->noSprites;
  for (int i = 0; i < src->noSprites; ++i) {
    dst->sprites[i] = src->sprites[i];
  }
  for (int i = 0; i < src->noBullets; ++i) {
    dst->bullets[i] = src->bullets[i];
  }
  dst->player = src->player;

  dst->rand_seed = src->rand_seed;
  dst->MISSILE_MINE_RADIUS = src->MISSILE_MINE_RADIUS;
  dst->explosionX = src->explosionX;
  dst->explosionY = src->explosionY;
  dst->explosionRadius = src->explosionRadius;
  dst->regainMissileCounter = src->regainMissileCounter;
}

void draw_game(display_t* display, gamestate_t* game, bool blackout) {
  if (blackout && game->explosionX != -1) {
    const int x = game->explosionX;
    const int y = game->explosionY;
    const int r = game->explosionRadius;
    fillBox(display, 0xFF000000, x - r, y - r, x + r, y + r);
  }
  for (int i = 0; i < game->noSprites; ++i) {
    draw_asteroid(display, &game->sprites[i], blackout);
  }
  for (int i = 0; i < game->noBullets; ++i) {
    draw_bullet(display, &game->bullets[i], blackout);
  }
  if (blackout || ((game->player.invincibilityFrames & 0x2) == 0x2)) {
    draw_player(display, &game->player.sprite, blackout);
  }

  /*drawNumPos(display, game->joystickXPos, 10, display->height - 50);
  drawNumPos(display, game->joystickYPos, 10, display->height - 40);
  drawNumPos(display, game->btnWhite | 0x10000000, 10, display->height - 30);
  drawNumPos(display, game->btnGreen | 0x20000000, 10, display->height - 20);
  drawNumPos(display, game->btnRed   | 0x30000000, 10, display->height - 10);*/

  draw_hud(display, &game->player);
}

void draw_hearts(display_t* display, int lives) {
  for (int y = 0; y < HEART_SIZE << 1; y++) {
    for (int x = 0; x < HEART_SIZE << 1; x++) {
      unsigned char pixel = HEART_SPRITE[(y >> 1) * HEART_SIZE + (x >> 1)];
      unsigned int col;
      if (pixel == HEART_RED) {
        col = 0xFFFF0000;
      } else if (pixel == HEART_WHITE) {
        col = 0xFFFFFFFF;
      } else {
        continue;
      }
      for (int i = 0; i < 3; i++) {
        setPixel(display, x + 20 + i * HEART_SIZE * 2, y + display->doubleBufYOff + 20, (i < lives)? col : 0xFF3f3f3f);
      }
    }
  }
}

void draw_score(display_t* display, int score, int missiles, int mines) {

  for (int cur = 0; cur < 8; ++cur) {
    drawCharDataScaled(display, 20 + (7-cur)*14, 20 + HEART_SIZE * 2, SCORE_FONT_SCALE, getNumData(score % 10));
    score /= 10;
  }

  for (int cur = 0; cur < 4; ++cur) {
    drawCharDataScaledCol(display, 20 + (7-cur)*14, 35 + HEART_SIZE * 2, SCORE_FONT_SCALE, getNumData(missiles % 10), 0xFFFF0000);
    missiles /= 10;
  }

  for (int cur = 0; cur < 4; ++cur) {
    drawCharDataScaledCol(display, 20 + (7-cur)*14, 50 + HEART_SIZE * 2, SCORE_FONT_SCALE, getNumData(mines % 10), 0xFF00FF00);
    mines /= 10;
  }
}

void gameOverScreen(display_t* display, gamestate_t* game) {
  update_display_forced(display, 0);
  black(display);
  draw_game(display, game, false);
  int scale = 10;
  int x = (display->width - ((scale*45)+2)) >> 1;
  int numx = ((scale*5)+2) * x + scale;
  int y = (display->height  - ((scale*5)+2)) >> 1;
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('G'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('A'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('M'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('E'), 0xFF7F7F7F);
  x++;
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('O'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('V'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('E'), 0xFF7F7F7F);
  drawCharDataScaledCol(display, ((scale*5)+2) * (x++), y, scale, getCharData('R'), 0xFF7F7F7F);

  int score = game->player.score;
  for (int cur = 0; cur < 8; ++cur) {
    drawCharDataScaledCol(display, numx + (7-cur)*((scale >> 1)*5 + 2), y + (scale*10), scale >> 1, getNumData(score % 10), 0xFF7F7F7F);
    score /= 10;
  }

  while (1) {
    if (GET_RED_BTN_STATE() || GET_GREEN_BTN_STATE() || GET_WHITE_BTN_STATE()) {
      black(display);
      break;
    }
  }
}

void draw_hud(display_t* display, player_t* player) {
  draw_hearts(display, player->lives);
  draw_score(display, player->score, player->numMissiles, player->numMines);
}

void update_game(display_t* display, gamestate_t* game) {
  game->player.score += 1;
  game->explosionX = -1;
  game->explosionY = -1;
  game->explosionRadius = -1;
  if (--game->regainMissileCounter == 0) {
    game->regainMissileCounter = 700;
    game->player.numMissiles += 8;
    game->player.numMines += 4;
  }

  for (int i = game->noSprites - 1; i >= 0; --i) {
    if (!update_asteroid(display, &game->sprites[i], game)) {
      split_asteroid(game, i);
    }
  }
  for (int i = game->noBullets - 1; i >= 0; --i) {
    if (!update_bullet(display, &game->bullets[i], game)) {
      // Move last bullet into this spot, and decrease noBullets
      game->bullets[i] = game->bullets[--game->noBullets];
    }
  }

  // FETCH INPUT
  game->joystickXPos = getJoystickXPos();
  game->joystickYPos = getJoystickYPos();
  game->btnWhite     = GET_WHITE_BTN_STATE();
  game->btnGreen     = GET_GREEN_BTN_STATE();
  game->btnRed       = GET_RED_BTN_STATE();

  // UPDATE PLAYER
  update_player(&game->player, game->sprites, game->noSprites, game->joystickXPos, game->joystickYPos);

  if (game->player.lives == -1) {
    // ded
    gameOverScreen(display, game);
    main();
  }

  if (game->btnWhite || game->btnGreen || game->btnRed) {
    if (game->player.cooldown == -1 || !game->player.firedLastFrame) {
      int type = game->btnWhite? 0: (game->btnRed? 1:2);
      if ((type == 0) || (type == 1 && game->player.numMissiles != 0) || (type == 2 && game->player.numMines != 0)) {
        if (type == 1) {
          game->player.numMissiles -= 1;
        } else if (type == 2) {
          game->player.numMines -= 1;
        }
        add_bullet(game, type);
        game->player.cooldown = 8;
        game->player.firedLastFrame = true;
      }
    }
  } else {
    game->player.firedLastFrame = false;
  }
  game->player.cooldown -= (game->player.cooldown != -1);

  check_bounds(display, &game->player.sprite);
}

void split_asteroid(gamestate_t* game, int index) {
  sprite_t ast = game->sprites[index];
  game->player.score += ast.type * 100;
  game->sprites[index] = game->sprites[--game->noSprites];
  // If !small, add two more, size - 1
  if (ast.type != AST_S) {
    for (int i = 0; i < 2; ++i) {
      add_sprite_fixed(game, ast.type + 1, ast.variant, ast.radius >> 1, ast.x, ast.y,
        ast.x_vel + (rand(&game->rand_seed) >> 13), ast.y_vel + (rand(&game->rand_seed) >> 13),
        0, ast.rot_vel + (rand(&game->rand_seed) >> 12));
    }
  }
}

void add_sprite(gamestate_t* game, SPRITE_TYPES type, int variant, int radius, int x, int y, int x_vel, int y_vel,
   int rotation, int rot_vel) {
  if (game->noSprites >= MAX_SPRITES) return;
  init_sprite(&game->sprites[game->noSprites++], type, variant, radius, x, y, x_vel, y_vel, rotation, rot_vel);
}

void add_sprite_fixed(gamestate_t* game, SPRITE_TYPES type, int variant, fixed_t radius, fixed_t x, fixed_t y, fixed_t x_vel, fixed_t y_vel,
   fixed_t rotation, fixed_t rot_vel) {
  if (game->noSprites >= MAX_SPRITES) return;
  init_sprite_fixed(&game->sprites[game->noSprites++], type, variant, radius, x, y, x_vel, y_vel, rotation, rot_vel);
}

void add_bullet(gamestate_t* game, int type) {
  if (game->noBullets >= MAX_BULLETS) return;
  const int x_vel = -cos(mul(game->player.sprite.rotation, degtorad)) << 5;
  const int y_vel = -sin(mul(game->player.sprite.rotation, degtorad)) << 5;
  init_sprite_fixed(&game->bullets[game->noBullets++], BULLET_BASE+type, 0, 1, game->player.sprite.x+x_vel, game->player.sprite.y+y_vel,
    x_vel, y_vel, 0, 0);
}

void draw_bullet(display_t* display, sprite_t* sprite, bool blackout) {
  const unsigned int col = sprite->type == BULLET_BASE? 0xFFFFFFFF : (sprite->type == BULLET_MISSILE? 0xFFFF0000 : 0xFF00FF00);
  const int x = to_int(sprite->x);
  const int y = to_int(sprite->y) + display->doubleBufYOff;
  setPixel(display, x  , y  , blackout? 0xFF000000:col);
  setPixel(display, x+1, y  , blackout? 0xFF000000:col);
  setPixel(display, x  , y+1, blackout? 0xFF000000:col);
  setPixel(display, x+1, y+1, blackout? 0xFF000000:col);
}

void draw_asteroid(display_t* display, sprite_t* sprite, bool blackout){

  const int size = to_int(sprite->radius << 1);
  const int spriteRot = to_int(sprite->rotation) / ASTEROID_ROT_STEP;
  sprite_data_t* spritedata;
  if (sprite->type == AST_L)      {spritedata = AST_L_SPRITE[sprite->variant][spriteRot]; }//ASTEROID_SPRITES[sprite->type - 1][0][spriteRot / PLAYER_ROT_STEP];
  else if (sprite->type == AST_M) {spritedata = AST_M_SPRITE[sprite->variant][spriteRot]; }
  else                            {spritedata = AST_S_SPRITE[sprite->variant][spriteRot]; }

  int yoff = (to_int(sprite->y - sprite->radius));
  yoff += (yoff < 0)? display->height:0;

  int xoff = (to_int(sprite->x - sprite->radius));
  xoff += (xoff < 0)? display->width:0;

  const unsigned int col = blackout ? 0xFF000000 : 0xFFFFFFFF;

  for (int y = 0; y < size; ++y) {
    yoff -= (y+yoff > display->height)? display->height:0;
    const int yyoff = (y+yoff+ display->doubleBufYOff) * display->pitchWords;
	  for (int x = 0; x < size; ++x) {
		  if (*spritedata++ != 0) {
			  *(((volatile unsigned int*)display->pixelData) +
			    ((yyoff)) + (x+xoff)) = col;
		  } //slow
	  }
  }
}

void draw_player(display_t* display, sprite_t* sprite, bool blackout){

  const int size = PLAYER_SIZE;
  const int spriteRot = to_int(sprite->rotation) / PLAYER_ROT_STEP;
  sprite_data_t* spritedata = PLAYER_SPRITE[spriteRot];

  int yoff = (to_int(sprite->y - sprite->radius));
  yoff += (yoff < 0)? display->height:0;

  int xoff = (to_int(sprite->x - sprite->radius));
  xoff += (xoff < 0)? display->width:0;

  const unsigned int col = blackout ? 0xFF000000 : 0xFFFFFFFF;

  for (int y = 0; y < size; ++y) {
    yoff -= (y+yoff > display->height)? display->height:0;
    const int yyoff = (y+yoff+ display->doubleBufYOff) * display->pitchWords;
	  for (int x = 0; x < size; ++x) {
		  if (*spritedata++ != 0) {
			  *(((volatile unsigned int*)display->pixelData) +
			    ((yyoff)) + (x+xoff)) = col;
		  } //slow
	  }
  }
}



void check_bounds(display_t* display, sprite_t* sprite) {
  /*if(sprite->x + sprite->radius >= to_fixed(display->width)
    || sprite->x - sprite->radius <= 0){
    sprite->x_vel = mul(sprite->x_vel, to_fixed(-1)); //invert x velocity to "bounce"
    if(sprite->x + sprite->radius >= to_fixed(display->width)){ //crimp it
      sprite->x = to_fixed(display->width - 1) - sprite->radius;
    } else{
      sprite->x = sprite->radius + to_fixed(1);
    }
  }

  if(sprite->y + sprite->radius >= to_fixed(display->height)
    || sprite->y - sprite->radius <= 0){
    sprite->y_vel = mul(sprite->y_vel, to_fixed(-1)); //invert y velocity
    if(sprite->y + sprite->radius >= to_fixed(display->height)){
      sprite->y = to_fixed(display->height - 1) - sprite->radius;
    }else{
      sprite->y = sprite->radius + to_fixed(1);
    }
  }*/

  const int dspW = to_fixed(display->width);
  const int dspH = to_fixed(display->height);
  sprite->x = (sprite->x + dspW) % dspW;
  sprite->y = (sprite->y + dspH) % dspH;
}

bool update_asteroid(display_t* display, sprite_t* sprite, gamestate_t* gamestate) {
  for (int i = 0; i < gamestate->noBullets; ++i) {
    const sprite_t b = gamestate->bullets[i];
    if (b.x < (sprite->x - sprite->radius) || b.x > (sprite->x + sprite->radius)) continue;
    if (b.y < (sprite->y - sprite->radius) || b.y > (sprite->y + sprite->radius)) continue;
    if (mul(b.x, b.x) + mul(b.y, b.y) <= mul(sprite->radius, sprite->radius)) {
      gamestate->bullets[i].x = 0xF0000000;
      gamestate->bullets[i].y = 0xF0000000; // px offscreen
      if (gamestate->bullets[i].type == BULLET_MISSILE && gamestate->explosionX == -1) {
        const int x = to_int(sprite->x);
        const int y = to_int(sprite->y);
        const int r = to_int(sprite->radius);
        gamestate->explosionX = x;
        gamestate->explosionY = y;
        gamestate->explosionRadius = r;
        fillBox(display, 0xFFC00000, x-r, y-r, x+r, y+r);
      }
      return false;
    }
  }
  update_pos_rot(sprite, to_fixed(1));
  check_bounds(display, sprite);
  return true;
}

bool update_bullet(display_t* display, sprite_t* sprite, gamestate_t* gamestate) {
  const fixed_t radius = to_fixed(gamestate->MISSILE_MINE_RADIUS);
  if (sprite->type == BULLET_BASE) {
  } else if (sprite ->type == BULLET_MISSILE) {
    int max = radius << 1;
    int maxInd = -1;
    for (int i = 0; i < gamestate->noSprites; ++i) {
      const sprite_t ast = gamestate->sprites[i];
      const fixed_t absx = abs(ast.x-sprite->x);
      if (absx > radius) continue;
      const fixed_t absy = abs(ast.y-sprite->y);
      if (absy > radius) continue;
      if (absx + absy < max) {
        max = absx+absy;
        maxInd = i;
      }
    }
    if (maxInd != -1) {
      sprite->x_vel = ((gamestate->sprites[maxInd].x - sprite->x) >> 1);
      sprite->y_vel = ((gamestate->sprites[maxInd].y - sprite->y) >> 1);
    }
  } else {
    sprite->x_vel = mul(sprite->x_vel, (3 << 14));
    sprite->y_vel = mul(sprite->y_vel, (3 << 14));
    if (gamestate->explosionX == -1) {
      bool exploded = false;
      for (int i = gamestate->noSprites - 1; i >= 0; --i) {
        const sprite_t ast = gamestate->sprites[i];
        const fixed_t absx = abs(ast.x-sprite->x);
        if (absx > radius) continue;
        const fixed_t absy = abs(ast.y-sprite->y);
        if (absy > radius) continue;
        // Explode!
        exploded = true;
        split_asteroid(gamestate, i);
      }
      if (exploded) {
        fillBox(display, 0xFF007f00, to_int(sprite->x - radius), to_int(sprite->y - radius), to_int(sprite->x + radius), to_int(sprite->y + radius));
        gamestate->explosionX = to_int(sprite->x);
        gamestate->explosionY = to_int(sprite->y);
        gamestate->explosionRadius = to_int(radius);
        return false;
      }
    }
  }

  sprite->x += sprite->x_vel;
  sprite->y += sprite->y_vel;

  const int x = to_int(sprite->x);
  const int y = to_int(sprite->y);
  return (x > 1) && (x < display->width - 1) && (y > 1) && (y < display->height - 1);
}

void waitForFrame(gamestate_t* game) {
  volatile int count = 25;

  for (int i = 0; i < game->noSprites; i++) {
    if (game->sprites[i].type == AST_M) {
      count -= 1;
    } else if (game->sprites[i].type == AST_L) {
      count -= 5;
    }
  }
  if (count < 5) {
    if (count < -8) {
      count = 2000;
    } else {
      count = 8000;
    }
  } else {
    count *= 1800;
  }
  while (count-- > 0) {}

}
