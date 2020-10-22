#ifndef SPRITES_H
#define SPRITES_H

#include "random.h"
#include "fixedpoint.h"
#include "lfb.h"
#include "draw.h"

typedef unsigned char sprite_data_t;

#define PLAYER_TYPE 0
#define AST_L_TYPE  1
#define AST_M_TYPE  2
#define AST_S_TYPE  3

#define NUM_AST_VARIANTS 5

#define PLAYER_SIZE 32 * 2
#define AST_L_SIZE 64 * 2
#define AST_M_SIZE (AST_L_SIZE >> 1)
#define AST_S_SIZE (AST_M_SIZE >> 1)

#define PLAYER_ROT_STEP 2
#define ASTEROID_ROT_STEP 10

#define NUM_PLAYER_ROT 360/PLAYER_ROT_STEP
#define NUM_AST_ROT    360/ASTEROID_ROT_STEP

#define AST_RESOLUTION 30
#define NUM_AST_RES 360/AST_RESOLUTION

#define PLAYER_BOT_ANGLE 40

// Usage: PLAYER[rot][y*PLAYER_SIZE + x]
// Usage: AST_*[variant][rot][y*(type size) + x]
// Usage: SPRITES[sprite_type][size][variant][rot][pix]

extern unsigned char AST_L_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_L_SIZE*AST_L_SIZE];
extern unsigned char AST_M_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_M_SIZE*AST_M_SIZE];
extern unsigned char AST_S_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_S_SIZE*AST_S_SIZE];
extern unsigned char PLAYER_SPRITE[NUM_PLAYER_ROT][PLAYER_SIZE*PLAYER_SIZE];

/** HEART */

#define HEART_SIZE 16
#define HEART_BLACK 0
#define HEART_WHITE 1
#define HEART_RED   2
extern unsigned char HEART_SPRITE[HEART_SIZE * HEART_SIZE];


void setSprites(display_t* display);

#endif // SPRITES_H
