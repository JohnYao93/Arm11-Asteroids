#ifndef EXTENSION_INPUT_H_INCLUDED
#define EXTENSION_INPUT_H_INCLUDED

#include "../gpio.h"

#define JOYSTICK_POS_MAX (523)
#define JOYSTICK_POS_MIN (-500)

typedef enum {
  BTN_UP   = PIN_LOW,
  BTN_DOWN = PIN_HIGH
} BTN_STATE;

void initInput();

int getJoystickYPos();
int getJoystickXPos();

BTN_STATE getButton1State();
BTN_STATE getButton2State();
BTN_STATE getButton3State();

#define GET_WHITE_BTN_STATE() (getButton1State())
#define GET_GREEN_BTN_STATE() (getButton2State())
#define GET_RED_BTN_STATE()   (getButton3State())

#endif // EXTENSION_INPUT_H_INCLUDED
