#include "input.h"
#include "spi.h"

#define JOYSTICK_X_CHANNEL (1)
#define JOYSTICK_Y_CHANNEL (0)
#define JOYSTICK_DEADBAND_MAX (520)
#define JOYSTICK_DEADBAND_MIN (480)

#define BTN_1_PIN (17) // P1->11 (White)
#define BTN_2_PIN (27) // P1->13 (Green)
#define BTN_3_PIN (22) // P1->15 (Red)

void initInput() {
  initSpi(); // initialise Joystick SPI interface

  // Initialise buttons
  pinMode(BTN_1_PIN, PIN_MODE_INPUT);
  pinMode(BTN_2_PIN, PIN_MODE_INPUT);
  pinMode(BTN_3_PIN, PIN_MODE_INPUT);
}

static inline int getJoystickAxisPos(int channel) {
  int val = readSpiChannel(channel);
  if (val >= JOYSTICK_DEADBAND_MIN && val <= JOYSTICK_DEADBAND_MAX) {
    return 0;
  }
  return val - 500;
}

int getJoystickYPos() {
  return getJoystickAxisPos(JOYSTICK_Y_CHANNEL);
}

int getJoystickXPos() {
  return getJoystickAxisPos(JOYSTICK_X_CHANNEL);
}

BTN_STATE getButton1State() {
  return digitalRead(BTN_1_PIN);
}

BTN_STATE getButton2State() {
  return digitalRead(BTN_2_PIN);
}

BTN_STATE getButton3State() {
  return digitalRead(BTN_3_PIN);
}
