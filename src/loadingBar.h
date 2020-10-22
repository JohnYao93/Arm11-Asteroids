#include "lfb.h"

typedef struct LoadingBar {
  int x, y;       // The root x and y coordinates of the loading bar
  int width;      // How wide the loading bar is, in pixels
  int thickness;  // The thickness of the bar, in pixels
  int progress;   // How far along the loading bar is, in pixels (from 0 to width-6 to account for the border)
} loadingBar_t;

void drawBar(display_t *display, loadingBar_t* bar);

void drawProgress(display_t *display, loadingBar_t *bar, int progress);

void clearBar(display_t *display, loadingBar_t *bar);

loadingBar_t initLoadingBar(int x, int y, int width, int thickness);