
#include "mbox.h"
#include "lfb.h"


display_t initDisplay(void)
{
	const int width  = 1024;
	const int height = 768;

    display_t display;
    display.debugLinesPrinted = 1;
	display.curBufFrame = BUF_FRAMES - 1;
	display.doubleBufYOff = display.curBufFrame * height;


    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003;     // == SET PHYSICAL WIDTH AND HEIGHT ==
    mbox[3] = 8;           //
    mbox[4] = 8;           //
    mbox[5] = width;       // Physical width
    mbox[6] = height;      // Physical height

    mbox[7] = 0x48004;     // == SET VIRTUAL WIDTH AND HEIGHT ==
    mbox[8] = 8;           //
    mbox[9] = 8;           //
    mbox[10] = width;      // Virtual width
    mbox[11] = height * BUF_FRAMES;
						   // Virtual height

    mbox[12] = 0x48009;    // == SET VIRTUAL OFFSET ==
    mbox[13] = 8;          //
    mbox[14] = 8;          //
    mbox[15] = 0;          // Virtual x offset
    mbox[16] = 0;          // Virtual y offset

    mbox[17] = 0x48005;    // == SET COLOUR DEPTH (bits per pixel) ==
    mbox[18] = 4;          //
    mbox[19] = 4;          //
    mbox[20] = COL_DEPTH;  // Colour depth

    mbox[21] = 0x48006;    // == SET PIXEL ORDER ==
    mbox[22] = 4;          //
    mbox[23] = 4;          //
    mbox[24] = 1;          // Pixel order; prefer RGB over BGR

    mbox[25] = 0x40001;    // == GET THE FRAMEBUFFER POINTER ==
    mbox[26] = 8;          //
    mbox[27] = 8;          //
    mbox[28] = 4096;       // Framebuffer pointer
    mbox[29] = 0;          // Pixel data alignment (ignored)

    mbox[30] = 0x40008;    // == GET PITCH (pixels per row) ==
    mbox[31] = 4;          //
    mbox[32] = 4;          //
    mbox[33] = 0;          // Pitch

    mbox[34] = MBOX_TAG_LAST;

    // Make sure colour depth has been set properly and framebuffer pointer was retrieved
    if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;
        display.width=mbox[5];
        display.height=mbox[6];
        display.pitch=mbox[33];
        display.pixelData=(void*)((unsigned long)mbox[28]);

        display.pitchWords = display.pitch / sizeof(unsigned int);

        display.success = 1;
    } else {
        display.success = 0;
    }

    return display;
}

void update_display_forced(display_t* display, int framebuffer) {
	display->curBufFrame = framebuffer % BUF_FRAMES;
	display->doubleBufYOff = display->curBufFrame * display->height;

	const unsigned int newBufOff = framebuffer * display->height;
	mbox[0] = 7*4;
    mbox[1] = MBOX_REQUEST;
	mbox[2] = 0x48009;    // == SET VIRTUAL OFFSET ==
    mbox[3] = 8;          //
    mbox[4] = 8;          //
    mbox[5] = 0;          // Virtual x offset
    mbox[6] = newBufOff;  // Virtual y offset

	mbox_call(8);
}

void update_display(display_t* display) {
	// 1 frame finished

	// First, update buffering offset to next buffer
	display->curBufFrame = (display->curBufFrame + 1) % BUF_FRAMES;
	display->doubleBufYOff = display->curBufFrame * display->height;

	// Then, update virtual pointer to 1 past that (oldest rendered frame)
	const unsigned int newBufOff = ((display->curBufFrame + 1) % BUF_FRAMES) * display->height;
	mbox[0] = 7*4;
    mbox[1] = MBOX_REQUEST;
	mbox[2] = 0x48009;    // == SET VIRTUAL OFFSET ==
    mbox[3] = 8;          //
    mbox[4] = 8;          //
    mbox[5] = 0;          // Virtual x offset
    mbox[6] = newBufOff;  // Virtual y offset

	mbox_call(8);
	volatile int i = 0;
	while(i--) {asm volatile("nop");}
}

void setMaxPower() {
	// NOT WORKING
	mbox[0] = 9 * 4;
	mbox[1] = MBOX_REQUEST;
	mbox[2] = 0x30004; // Get max clock rate
	mbox[3] = 4;
	mbox[5] = 8;
	mbox[6] = 3; // ARM system clock
	mbox[7] = 0; // Rate
	mbox[8] = MBOX_TAG_LAST;
	if (mbox_call(1) && mbox[7] != 0) {
		mbox[0] = 12 * 4;
		mbox[1] = MBOX_REQUEST;
		mbox[2] = 0x38002; // Get max clock rate
		mbox[3] = 12;
		mbox[5] = 8;
		mbox[6] = 3; // ARM system clock
		mbox[7]; // Rate - set to max (got previously)
		mbox[8] = 0;

		mbox[9] = 0;
		mbox[10] = 0;
		mbox[11] = MBOX_TAG_LAST;
		mbox_call(1); // Attempt to set up
	}
}

COL_T genColour(unsigned int i) {
    int r = 0;
    int g = 0;
    int b = 0;

    i %= 256*3;

    if (i < 256) {
        r = 255-i;
        g = i;
    } else if (i < 512) {
        g = 511-i;
        b = i-256;
    } else {
        b = 767-i;
        r = i-512;
    }

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void fillBox(display_t* display, COL_T col, int x1, int y1, int x2, int y2) {
	x1 = (x1 < 0)? 0:x1;
	x2 = (x2 > display->width)? display->width:x2;
	y1 = (y1 < 0)? 0:y1;
	y2 = (y2 > display->height)? display->height:y2;
	for (int y = y1; y < y2; ++y) {
		for (int x = x1; x < x2; ++x) {
			setPixel(display, x, y, col);
		}
	}
}

void pride(display_t *display, int colIx) {
    int col = genColour(colIx);
	for (unsigned int y = 0; y < display->height; y++) {
		for (unsigned int x = 0; x < display->width; x++) {
			setPixel(display, x, y, col);
		}

		col = genColour(colIx+y);
	}
}

void black(display_t* display) {
	for (unsigned int y = display->doubleBufYOff; y < display->height + display->doubleBufYOff; ++y) {
		for (unsigned int x = 0; x < display->width; ++x) {
			setPixel(display, x, y, 0xFF000000);
		}
	}
}

void setPixel(display_t *display, int x, int y, COL_T col) {
    *(((volatile unsigned int*)display->pixelData) + (y * display->pitchWords) + x) = col;
}
