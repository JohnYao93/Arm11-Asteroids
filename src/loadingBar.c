#include "loadingBar.h"
#include "draw.h"

#define CHAR_SIZE  20

void drawBar(display_t *display, loadingBar_t* bar) {
  // Draw "LOADING..." text
  int charScale = CHAR_SIZE/5;
  int textWidth = CHAR_SIZE*19;
  int xOffset = (bar->width - textWidth) >> 1;
  drawCharScaled(display, bar->x + xOffset, bar->y, charScale, 'L');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*2, bar->y, charScale, 'O');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*4, bar->y, charScale, 'A');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*6, bar->y, charScale, 'D');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*8, bar->y, charScale, 'I');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*10, bar->y, charScale, 'N');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*12, bar->y, charScale, 'G');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*14, bar->y, charScale, '.');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*16, bar->y, charScale, '.');
  drawCharScaled(display, bar->x + xOffset + CHAR_SIZE*18, bar->y, charScale, '.');

  // Draw bar outline
  int yOffset = CHAR_SIZE*2;
  for (int dy=0; dy < (bar->thickness + 6); dy++) {
    setPixel(display, bar->x, bar->y + yOffset + dy, WHITE);
    setPixel(display, bar->x + bar->thickness + 5, bar->y + yOffset + dy, WHITE);
  }
  for (int dx=1; dx < (bar->width-1); dx++) {
    setPixel(display, bar->x, bar->y + yOffset, WHITE);
    setPixel(display, bar->x, bar->y + yOffset + bar->thickness + 5, WHITE);
  }
    
}

void drawProgress(display_t *display, loadingBar_t *bar, int progress) {
  if (progress < bar->progress)
    return;
  int yOffset = CHAR_SIZE*2 + 3;
  int innerBarWidth = bar->width-6;
  if (progress > innerBarWidth)
    progress = innerBarWidth;
  for (int dx=progress; dx<innerBarWidth; dx++)
    for (int dy=0; dy<bar->thickness; dy++)
      setPixel(display, bar->x + 3 + dx, bar->y + yOffset + dy, WHITE);
}

void clearBar(display_t *display, loadingBar_t *bar) {
  for (int dy=0; dy< (CHAR_SIZE*2 + bar->thickness + 6); dy++)
    for (int dx=0; dx < bar->width; dx++)
      setPixel(display, bar->x + dx, bar->y + dy, BLACK);
}

loadingBar_t initLoadingBar(int x, int y, int width, int thickness) {
  int minBarWidth = CHAR_SIZE*19;

  if (width < minBarWidth)
    width = minBarWidth;

  loadingBar_t bar;
  bar.progress = 0;
  bar.x = x;
  bar.y = y;
  bar.width = width;
  bar.thickness = thickness;

  return bar;
}