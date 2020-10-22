#include "sprite_graphics.h"

unsigned char AST_L_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_L_SIZE*AST_L_SIZE];
unsigned char AST_M_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_M_SIZE*AST_M_SIZE];
unsigned char AST_S_SPRITE[NUM_AST_VARIANTS][NUM_AST_ROT][AST_S_SIZE*AST_S_SIZE];
unsigned char PLAYER_SPRITE[NUM_PLAYER_ROT][PLAYER_SIZE*PLAYER_SIZE];
unsigned char HEART_SPRITE[HEART_SIZE*HEART_SIZE];

fixed_t lerp(fixed_t a, fixed_t b, fixed_t p) {
	return a + mul((b - a), p);
}

#define DRAW_CROSS(ptr, size, x, y, col) \
	do {\
		ptr[y*size+x]     = col;\
		ptr[y*size+x-1]   = col;\
		ptr[(y-1)*size+x] = col;\
		ptr[y*size+x+1]   = col;\
		ptr[(y+1)*size+x] = col;\
	} while(0)

void drawLine(unsigned char* canvas, unsigned size, fixed_t x1, fixed_t y1, fixed_t x2, fixed_t y2) {
	int maxLerp = to_int((abs(x1-x2) > abs(y1-y2))? abs(x1-x2):abs(y1-y2));
	fixed_t offsetEach = div((1 << 16), to_fixed(maxLerp));
	fixed_t offset = 0;
	int sizeMin = size-1;
	for (int i = 0; i < maxLerp; ++i, offset += offsetEach) {
		int x = to_int(lerp(x1, x2, offset));
		int y = to_int(lerp(y1, y2, offset));
		if (x > 0 && x < sizeMin && y > 0 && y < sizeMin) {
			DRAW_CROSS(canvas, size, x, y, 0xFF);
		}
	}
}

/**
	 2,3,4,       8,9,10
 17   , 21,   23,      27
32 ,      38,           44
48,                     60
64,                     76
	81,                  91
	 98,              106
		115,          121
			132       136
				149   151
				   166
					*/

void fillRed(int start, int finish) {
	for (int i = start + 1; i < finish; i++) {
		HEART_SPRITE[i] = HEART_RED;
	}
}

void drawHeart() {
	for (int i = 0; i < HEART_SIZE * HEART_SIZE; i++) {
		HEART_SPRITE[i] = HEART_BLACK;
	}

	HEART_SPRITE[2] = HEART_WHITE;
	HEART_SPRITE[3] = HEART_WHITE;
	HEART_SPRITE[4] = HEART_WHITE;
	HEART_SPRITE[8] = HEART_WHITE;
	HEART_SPRITE[9] = HEART_WHITE;
	HEART_SPRITE[10] = HEART_WHITE;

	HEART_SPRITE[17] = HEART_WHITE;
	fillRed(17, 21);
	HEART_SPRITE[21] = HEART_WHITE;
	HEART_SPRITE[23] = HEART_WHITE;
	fillRed(23, 27);
	HEART_SPRITE[27] = HEART_WHITE;

	HEART_SPRITE[32] = HEART_WHITE;
	fillRed(32, 38);
	HEART_SPRITE[38] = HEART_WHITE;
	fillRed(38, 44);
	HEART_SPRITE[44] = HEART_WHITE;

	HEART_SPRITE[48]  = HEART_WHITE;
	fillRed(48, 60);
	HEART_SPRITE[60]  = HEART_WHITE;

	HEART_SPRITE[64]  = HEART_WHITE;
	fillRed(64, 76);
	HEART_SPRITE[76]  = HEART_WHITE;

	HEART_SPRITE[81]  = HEART_WHITE;
	fillRed(81, 91);
	HEART_SPRITE[91]  = HEART_WHITE;

	HEART_SPRITE[98]  = HEART_WHITE;
	fillRed(98, 106);
	HEART_SPRITE[106] = HEART_WHITE;

	HEART_SPRITE[115] = HEART_WHITE;
	fillRed(115, 121);
	HEART_SPRITE[121] = HEART_WHITE;

	HEART_SPRITE[132] = HEART_WHITE;
	fillRed(132, 136);
	HEART_SPRITE[136] = HEART_WHITE;

	HEART_SPRITE[149] = HEART_WHITE;
	fillRed(149, 151);
	HEART_SPRITE[151] = HEART_WHITE;

	HEART_SPRITE[166] = HEART_WHITE;

	/***********************************/

	HEART_SPRITE[34] = HEART_WHITE;
	HEART_SPRITE[35] = HEART_WHITE;
	HEART_SPRITE[50] = HEART_WHITE;

}


void drawAsteroid(unsigned char* canvas, fixed_t offsets[NUM_AST_RES], int rotOffset, int canvasSize) {
	for (int i = 0; i < canvasSize*canvasSize; ++i) {
		canvas[i] = 0; // Blank first
	}
	const fixed_t radius_f = to_fixed((canvasSize >> 1) - 2);
	const fixed_t rot_off_f = div(mul(PI, to_fixed(rotOffset)), to_fixed(180));
	const fixed_t i_to_rads = div(PI, to_fixed(NUM_AST_RES)) << 1; // 2pi/N
	fixed_t pointsX[NUM_AST_RES];
	fixed_t pointsY[NUM_AST_RES];
	fixed_t rot = 0;
	for (int i = 0; i < NUM_AST_RES; ++i, rot += i_to_rads) {
		fixed_t thisRad = mul(radius_f, offsets[i]);
		pointsX[i] = mul(thisRad, sin(rot + rot_off_f)) + radius_f;
		pointsY[i] = mul(thisRad, cos(rot + rot_off_f)) + radius_f;
	}
	for (int i = 1; i < NUM_AST_RES; ++i) {
		drawLine(canvas, canvasSize, pointsX[i-1], pointsY[i-1], pointsX[i], pointsY[i]);
	}
	drawLine(canvas, canvasSize, pointsX[0], pointsY[0], pointsX[NUM_AST_RES - 1], pointsY[NUM_AST_RES - 1]);
}

void drawPlayer(unsigned char* canvas, int rotOffset) {
	for (int i = 0; i < PLAYER_SIZE*PLAYER_SIZE; ++i) {
		canvas[i] = 0; // Blank first
	}

	const fixed_t radius_f  = to_fixed((PLAYER_SIZE >> 1) - 2);
	const fixed_t rot_off_f = div(mul(PI, to_fixed(rotOffset)), to_fixed(180));
	const fixed_t points_f  = div(mul(PI, to_fixed(PLAYER_BOT_ANGLE)), to_fixed(180));

	const fixed_t x1 = mul(radius_f, cos(rot_off_f + PI)) + radius_f;
	const fixed_t y1 = mul(radius_f, sin(rot_off_f + PI)) + radius_f;

	const fixed_t x2 = mul(radius_f, cos(rot_off_f + points_f)) + radius_f;
	const fixed_t y2 = mul(radius_f, sin(rot_off_f + points_f)) + radius_f;

	const fixed_t x3 = radius_f;
	const fixed_t y3 = radius_f;

	const fixed_t x4 = mul(radius_f, cos(rot_off_f - points_f)) + radius_f;
	const fixed_t y4 = mul(radius_f, sin(rot_off_f - points_f)) + radius_f;

	drawLine(canvas, PLAYER_SIZE, x1, y1, x2, y2);
	drawLine(canvas, PLAYER_SIZE, x2, y2, x3, y3);
	drawLine(canvas, PLAYER_SIZE, x3, y3, x4, y4);
	drawLine(canvas, PLAYER_SIZE, x4, y4, x1, y1);

}

void setSprites(display_t* display) {
	unsigned long long seed = (0x1234ABCD);

	fillBox(display, 0xFFFFFFFF, 2, (display->height - 64) >> 1, display->width - 2, (display->height+64) >> 1);
	fillBox(display, 0xFF000000, 4, (display->height - 60) >> 1, display->width - 4, (display->height+60) >> 1);
	fixed_t done = 0;
	fixed_t px_per_step = div(to_fixed(display->width - 12), to_fixed(NUM_AST_VARIANTS * NUM_AST_ROT + NUM_PLAYER_ROT));
	for (int variant = 0; variant < NUM_AST_VARIANTS; ++variant) {
	  seed += getJoystickXPos();
	  seed += getJoystickYPos();
		fixed_t offsets[NUM_AST_RES]; // To hold the offsets
		for (int rot = 0; rot < NUM_AST_RES; ++rot) {
	    seed += getJoystickXPos();
	    seed += getJoystickYPos();
			offsets[rot] = (rand(&seed) >> 16); // fixed 0-1
			offsets[rot] = mul(offsets[rot], (1 << 14)) + (3 << 14); // 0.75 - 1
		}
		for (int rot = 0, rotI = 0; rot < 360; rot += ASTEROID_ROT_STEP, ++rotI) {
			drawAsteroid(AST_L_SPRITE[variant][rotI], offsets, rot, AST_L_SIZE);
			drawAsteroid(AST_M_SPRITE[variant][rotI], offsets, rot, AST_M_SIZE);
			drawAsteroid(AST_S_SPRITE[variant][rotI], offsets, rot, AST_S_SIZE);
			fillBox(display, 0xFFFFFFFF, 6 + to_int(done), (display->height - 56) >> 1, 6 + to_int(done + px_per_step), (display->height + 56) >> 1);
			//update_display_forced(display, 0);
			done += px_per_step;
		}
	}
	for (int rot = 0, rotI = 0; rot < 360; rot += PLAYER_ROT_STEP, ++rotI) {
		drawPlayer(PLAYER_SPRITE[rotI], rot);
		fillBox(display, 0xFFFFFFFF, 6 + to_int(done), (display->height - 56) >> 1, 6 + to_int(done + px_per_step), (display->height + 56) >> 1);
		//update_display_forced(display, 0);
		done += px_per_step;
	}

	drawHeart();
}
