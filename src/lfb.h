#ifndef LFB_H
#define LFB_H


#define BUF_FRAMES 2
#define COL_DEPTH  32
#define COL_T      unsigned int

typedef struct Display {
  unsigned short success;          // 1 if the framebuffer has been successfully initialised, else 0

  unsigned int width;              // The width of the physical display
  unsigned int height;             // The height of the physical display
  unsigned int pitch;              // The pitch (pixels per row) of the physical display
  unsigned int pitchWords;         // The number of words per row on the physical display

  unsigned char *pixelData;        // A pointer to the array where pixel data must be written to
  volatile unsigned int curBufFrame;
  volatile unsigned int doubleBufYOff;

  unsigned int debugLinesPrinted;  // The number of lines printed to the screen with the debug printer
} display_t;

display_t initDisplay(void);
void setPixel(display_t *display, int x, int y, COL_T col);
void pride(display_t *display, int colIx);
void fillBox(display_t* display, COL_T col, int x1, int y1, int x2, int y2);
void setMaxPower();
void black(display_t* display);

void update_display(display_t* display);
void update_display_forced(display_t* display, int framebuffer);

#endif //LFB_H
