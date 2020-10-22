#include "draw.h"
#include "lfb.h"


int getNumData(unsigned int n) {
  if (n == 0) {
      return 0b0111011001101011001101110;
  } else if (n == 1) {
    return 0b0010001100001000010011111;
  } else if (n == 2) {
    return 0b1111000001011101000011111;
  } else if (n == 3) {
    return 0b1111000001011100000111110;
  } else if (n == 4) {
    return 0b1000110001111110000100001;
  } else if (n == 5) {
    return 0b1111110000111100000111110;
  } else if (n == 6) {
    return 0b0111010000111101000101110;
  } else if (n == 7) {
    return 0b1111100010001000100001000;
  } else if (n == 8) {
    return 0b0111010001011101000101110;
  } else if (n == 9) {
    return 0b0111010001011110000101110;
  } else if (n == 0xA) {
    return 0b0111010001111111000110001;
  } else if (n == 0xB) {
    return 0b1111010001111101000111110;
  } else if (n == 0xC) {
    return 0b1111110000100001000011111;
  } else if (n == 0xD) {
    return 0b1111010001100011000111110;
  } else if (n == 0xE) {
    return 0b1111110000111101000011111;
  } else if (n == 0xF) {
    return 0b1111110000111101000010000;
  }
  return 0b1010101010101010101010101;
}

/** Returns the pixel data for a character, encoded in 32 bit binary

    @arg ch => the character to get pixel data of

    @return => the pixel data of the character

    Note:
     -> currently supports 'A'-'Z', '0'-'9', '?', '!', '.'
     -> lowercase chars are automatically converted to uppercase
     -> undefined chars are represented by a special drawn character
     -> since 25 pixels are required for pixel data,
        the first 7 bits of the return value should be ignored
 */
int getCharData(char ch) {
  // return 0b1111000001011101000011111;

  if ('a' <= ch && ch <= 'z')
    ch -= ('a'-'A');

  if (ch == ' ') {
    return 0b0000000000000000000000000;
  } else if (ch == '0') {
    return 0b0111011001101011001101110;
  } else if (ch == '1') {
    return 0b0010001100001000010011111;
  } else if (ch == '2') {
    return 0b1111000001011101000011111;
  } else if (ch == '3') {
    return 0b1111000001011100000111110;
  } else if (ch == '4') {
    return 0b1000110001111110000100001;
  } else if (ch == '5') {
    return 0b1111110000111100000111110;
  } else if (ch == '6') {
    return 0b0111010000111101000101110;
  } else if (ch == '7') {
    return 0b1111100010001000100001000;
  } else if (ch == '8') {
    return 0b0111010001011101000101110;
  } else if (ch == '9') {
    return 0b0111010001011110000101110;
  } else if (ch == 'A') {
    return 0b0111010001111111000110001;
  } else if (ch == 'B') {
    return 0b1111010001111101000111110;
  } else if (ch == 'C') {
    return 0b1111110000100001000011111;
  } else if (ch == 'D') {
    return 0b1111010001100011000111110;
  } else if (ch == 'E') {
    return 0b1111110000111101000011111;
  } else if (ch == 'F') {
    return 0b1111110000111101000010000;
  } else if (ch == 'G') {
    return 0b0111110000101101000101111;
  } else if (ch == 'H') {
    return 0b1000110001111111000110001;
  } else if (ch == 'I') {
    return 0b1111100100001000010011111;
  } else if (ch == 'J') {
    return 0b1111100100001000010011000;
  } else if (ch == 'K') {
    return 0b1000110010111001001010001;
  } else if (ch == 'L') {
    return 0b1000010000100001000011111;
  } else if (ch == 'M') {
    return 0b1000111011101011000110001;
  } else if (ch == 'N') {
    return 0b1000111001101011001110001;
  } else if (ch == 'O') {
    return 0b0111010001100011000101110;
  } else if (ch == 'P') {
    return 0b1111010001111101000010000;
  } else if (ch == 'Q') {
    return 0b0111010001100011001001101;
  } else if (ch == 'R') {
    return 0b1111010001111111001010001;
  } else if (ch == 'S') {
    return 0b1111100100001000010000100;
  } else if (ch == 'T') {
    return 0b1111100100001000010000100;
  } else if (ch == 'U') {
    return 0b1000110001100011000101110;
  } else if (ch == 'V') {
    return 0b1000110001100010101000100;
  } else if (ch == 'W') {
    return 0b1000110001100011010101010;
  } else if (ch == 'X') {
    return 0b1000101010001000101010001;
  } else if (ch == 'Y') {
    return 0b1000101010001000100010000;
  } else if (ch == 'Z') {
    return 0b1111100010001000100011111;
  } else if (ch == '!') {
    return 0b0010000100001000000000100;
  } else if (ch == '?') {
    return 0b0111000010011100000001000;
  } else if (ch == '.') {
    return 0b0000000000000000000000100;
  }
  return 0b1010101010101010101010101;
}

/** Draws a single character to the given display

    @arg display => the display to draw to
    @arg x       => the x-coordinate of the character on the screen
    @arg y       => the y-coordinate of the character on the screen
    @arg ch      => the the character to be drawn

    Note:
     -> The character is drawn with a 1 pixel black border, exluding the right border
     -> The right border of the last character in a line should be drawn by newLine()
 */
void drawChar(display_t *display, int x, int y, int charData) {
  // Draw top and bottom borders
  for (int dx=0; dx<=5; dx++) {
    setPixel(display, x+dx, y, BLACK);
    setPixel(display, x+dx, y+6, BLACK);
  }

  // Draw left border
  for (int dy=1; dy<=5; dy++)
    setPixel(display, x, y+dy, BLACK);

  // Draw character
  y += display->doubleBufYOff;
  for (int dy=5; dy>=1; dy--) {
      for (int dx=5; dx>=1; dx--) {
          setPixel(display, x+dx, y+dy, ((charData & 1) ? WHITE : BLACK));
          charData >>= 1;
      }
  }
}

void drawCharDataScaled(display_t *display, int x, int y, int scale, int charData) {
  // For each "pixel" in the character's data...
  for (int dy1 = 5*scale+display->doubleBufYOff; dy1 > display->doubleBufYOff; dy1 -= scale) {
    for (int dx1 = 5*scale; dx1 > 0; dx1 -= scale) {
      // ...if it's white...
      unsigned int col = (charData & 1) ? WHITE:BLACK;
        // ...draw each pixel of the scaled char.
      for (int dy2 = 0; dy2 < scale; dy2++) {
        for (int dx2 = 0; dx2 < scale; dx2++) {
          setPixel(display, x+dx1+dx2, y+dy1+dy2, col);
        }
      }
      charData >>= 1;
    }
  }
}

void drawCharDataScaledCol(display_t *display, int x, int y, int scale, int charData, int playerCol) {
  // For each "pixel" in the character's data...
  for (int dy1 = 5*scale+display->doubleBufYOff; dy1 > display->doubleBufYOff; dy1 -= scale) {
    for (int dx1 = 5*scale; dx1 > 0; dx1 -= scale) {
      // ...if it's white...
      unsigned int col = (charData & 1) ? playerCol:BLACK;
        // ...draw each pixel of the scaled char.
      for (int dy2 = 0; dy2 < scale; dy2++) {
        for (int dx2 = 0; dx2 < scale; dx2++) {
          setPixel(display, x+dx1+dx2, y+dy1+dy2, col);
        }
      }
      charData >>= 1;
    }
  }
}

void drawCharScaled(display_t *display, int x, int y, int scale, char ch) {
  int charData = getCharData(ch);
  drawCharDataScaled(display, x, y, scale, charData);
}

/** Draws a string to the given display

    @arg display => the display to draw to
    @arg str     => the string to draw

    @return      => the number of characters drawn

    Note:
     -> This function does not set the "printer" to a new line;
        subsequent calls to this function without calling newLine() will
        overwrite previously printed strings.
     -> The return value should be equal to the length of the string
 */
int drawString(display_t *display, char* str) {
  int y = display->height - (display->debugLinesPrinted*7);

  int i = 0;
  while (str[i] != '\0') {
    drawChar(display, i*6, y, str[i]);
    i++;
  }

  return i;
}

/** Draws the end of the current line, and sets the "printer" to a new line

    @arg display  => The display to draw the end of the current line to
    @arg numChars => The number of characters already drawn on the current line
 */
void newLine(display_t *display, int numChars) {
  // Increment number of lines printed
  int y = display->height - ((display->debugLinesPrinted++)*7);
  int x = numChars*6;

  // Draw right border of last character
  for (int dy=0; dy<7; dy++)
    setPixel(display, x, y+dy, BLACK);
}

void drawText(display_t *display, char* str) {
  int numChars = drawString(display, str);
  newLine(display, numChars);
}

void drawNumPos(display_t *display, unsigned int num, int x, int y) {
	for (int i=0; i<8; i++) {
    int numData = getNumData((num >> (28-(4*i))) & 0xF);
    drawChar(display, x + i*6, y + display->doubleBufYOff, numData);
  }
}

void drawNum(display_t *display, unsigned int num) {
  drawNumPos(display, num, 0, display->height - (display->debugLinesPrinted*7));
  newLine(display, 8);
}
