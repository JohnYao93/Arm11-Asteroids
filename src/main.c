#include "lfb.h"

#include "sprites.h"
#include "game.h"
#include "player.h"
#include "draw.h"
#include "fixedpoint.h"
#include "random.h"

void main()
{
  // Initialise frame buffer
  display_t display = initDisplay();

  // Set bits in control register to enable branch predictor + instruction cache + data caching
  unsigned int controlRegister;
  asm volatile ("MRC p15, 0, %0, c1, c0, 0" : "=r" (controlRegister));
  controlRegister |= 0x1802;
  asm volatile ("MCR p15, 0, %0, c1, c0, 0" :: "r" (controlRegister));

  // Initialise input
  initInput();

	//setMaxPower();

	/*for (int i = 0; i < 20; ++i) {
		black(&display);
	}*/
	// For testing loading bar ^^

	drawNum(&display, 0xDEADBEEF);

	setSprites(&display);

	gamestate_t games[BUF_FRAMES];
	init_game(&games[0], 600, 600);

	games[0].player.sprite.rot_vel = to_fixed(0);
  games[0].player.sprite.x_vel   = to_fixed(0);
  games[0].player.sprite.y_vel   = to_fixed(0);

  unsigned long long seed = 0x1432BCDE;

	/*add_sprite(&game0, AST_L, AST_L_SIZE >> 1, 600, 600, 5, 10, 0, 5);
	add_sprite(&game0, AST_M, AST_M_SIZE >> 1, 600, 600, 7, -7, 0, 5);
	add_sprite(&game0, AST_S, AST_S_SIZE >> 1, 600, 600, 3, 6, 0, 5);*/
	for (int i = 0; i < 3; ++i) {
	  const int type = 1+(rand(&seed)%3);
	  add_sprite(&games[0], type, (rand(&seed) % NUM_AST_VARIANTS), 
	      1 << (7-type),
	      rand(&seed)%display.width, rand(&seed)% (AST_L_SIZE >> 1),
	      (rand(&seed)%20) - 10, (rand(&seed)%20) -10,
	      0, (rand(&seed)%10) - 5);
	}

	for (int i = 0; i < BUF_FRAMES; ++i) copy_game(&games[0], &games[i]);

	/*sprite_t astS, astM, astL;
	init_sprite(&astL, AST_L, AST_L_SIZE >> 1, 600, 600, 5, 10, 0, 5);
	init_sprite(&astM, AST_M, AST_M_SIZE >> 1, 600, 600, 7, -7, 0, 5);
	init_sprite(&astS, AST_S, AST_S_SIZE >> 1, 600, 600, 3, 6, 0, 5);

	player_t player;
	init_player(&player, display.width >> 1, display.height >> 1);
	player.sprite.rot_vel = to_fixed(5);*/

	for (int i = 0; i < BUF_FRAMES; ++i) {
	  black(&display);
	  update_display(&display);
	}

	int newAstCount = 0;

	while (1) {
		// undo games[i]
		const int i = display.curBufFrame;
		draw_game(&display, &games[i], true);

		//Copy last frame of game into this one
		copy_game(&games[(i-1+BUF_FRAMES)%BUF_FRAMES], &games[i]);

		//Update this frame
		update_game(&display, &games[i]);
		if (++newAstCount >= 40) {
		  const int type = 1 + (rand(&seed) & 1);
		  add_sprite(&games[i], type, (rand(&seed) % NUM_AST_VARIANTS),
		    (AST_L_SIZE >> type),
		    rand(&seed) % display.width, display.height - (AST_L_SIZE >> 1),
		    (rand(&seed) % 16) - 8, (rand(&seed) % 16) - 8,
		    0,
		    (rand(&seed) % 16) - 8);
		  newAstCount = 0;
		}

	  //Draw this frame
	  draw_game(&display, &games[i], false);

    /**
		// Update display to most early frame rendered
		drawNumPos(&display, finvsqrt(to_fixed(4)), 10, 10);
		drawNumPos(&display, finvsqrt(to_fixed(16)), 10, 20);
		drawNumPos(&display, finvsqrt(to_fixed(1)>>2), 10, 30);

		drawNumPos(&display, mul(to_fixed(-2), to_fixed(-2)), 10, 50);
		 */
		update_display(&display);
		seed += games[i].player.sprite.x;
    waitForFrame(&games[i]);
	}
}
