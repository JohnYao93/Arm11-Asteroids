#include "gpio.h"

void pinMode(int pin, const PIN_MODE mode) {
  unsigned int* base;
  if (pin < 10) {
    base = (unsigned int*) GPFSEL0;
  } else if (pin < 20) {
    base = (unsigned int*) GPFSEL1;
  } else if (pin < 30) {
    base = (unsigned int*) GPFSEL2;
  } else if (pin < 40) {
    base = (unsigned int*) GPFSEL3;
  } else if (pin < 50) {
    base = (unsigned int*) GPFSEL4;
  } else if (pin < 60) {
    base = (unsigned int*) GPFSEL5;
  } else {
    return;
  }
  pin   = (pin % 10) * 3;
  *base = ((*base) & (~(0x7 << pin))) | (mode << pin);
}

PIN_STATE digitalRead(const int pin) {
  if (pin < 32) {
    return ((*GPLEV0) >> pin) & 1;
  } else {
    return ((*GPLEV1) >> (pin - 32)) & 1;
  }
}


void digitalWrite(const int pin, const PIN_STATE state) {
  if (state == PIN_HIGH) {
    if (pin < 32) {
      (*GPSET0) = 1 << pin;
    } else {
      (*GPSET1) = 1 << (pin - 32);
    }
  } else {
    if (pin < 32) {
      (*GPCLR0) = 1 << pin;
    } else {
      (*GPCLR1) = 1 << (pin - 32);
    }
  }
}
