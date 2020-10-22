#ifndef DRAW_H
#define DRAW_H

#include "lfb.h"

#define BLACK  0xFF000000
#define WHITE  0xFFFFFFFF

int getNumData(unsigned int n);
void drawText(display_t *display, char* str);
int getCharData(char ch);
void drawChar(display_t *display, int x, int y, int charData);

void drawNum(display_t *display, unsigned int num);
void drawNumPos(display_t *display, unsigned int num, int x, int y);

void drawCharDataScaled(display_t *display, int x, int y, int scale, int charData);
void drawCharScaled(display_t *display, int x, int y, int scale, char ch);
void drawCharDataScaledCol(display_t *display, int x, int y, int scale, int charData, int playerCol);

#endif // DRAW_H
