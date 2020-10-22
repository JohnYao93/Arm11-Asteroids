#include "spi.h"
#include "../gpio.h"

/**
 GPIO access is VERRRY SLOW so delay calls have been commented out

 SPI     docs : https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
 MCP3008 docs : http://www.hertaville.com/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c.html

 (wikipedia also useful)
*/

#define SPI_CE0    8
#define SPI_SCLK  11
#define SPI_MISO   9
#define SPI_MOSI  10

// GPIO access is VERRRY SLOW so delay calls have been commented out
#define SPI_SCLK_LOW_TIME  (0)
#define SPI_SCLK_HIGH_TIME (0)

typedef unsigned char uint8_t;

static void __attribute__((optimize("O0"))) delay(int steps) {
  int cnt  = 0;
  while (cnt++ < steps) {}
}

/** Initialise SPI interface with SPI device. */
void initSpi(){
  pinMode(SPI_CE0,  PIN_MODE_OUTPUT);
  pinMode(SPI_SCLK, PIN_MODE_OUTPUT);
  pinMode(SPI_MISO, PIN_MODE_INPUT);
  pinMode(SPI_MOSI, PIN_MODE_OUTPUT);

  // deactivate SPI transfer
  digitalWrite(SPI_CE0, PIN_HIGH);
  digitalWrite(SPI_SCLK, PIN_LOW);
}

static void initTransfer() {
  digitalWrite(SPI_CE0, PIN_LOW); // activate chip
  // delay(SPI_SCLK_HIGH_TIME);
}

static void terminateTransfer() {
  // delay(SPI_SCLK_LOW_TIME);
  digitalWrite(SPI_CE0, PIN_HIGH); // deactivate chip
}

/* Pseudocode used: https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
 * Simultaneously transmit and receive a byte on the SPI.
 *
 * Polarity and phase are assumed to be both 0, i.e.:
 *   - input data is captured on rising edge of SCLK.
 *   - output data is propagated on falling edge of SCLK.
 *
 * Returns the received byte.
 */
static uint8_t transferByte(uint8_t byte_out) {
    uint8_t byte_in = 0;
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        /* Shift-out a bit to the MOSI line */
        digitalWrite(SPI_MOSI, (byte_out & bit) ? PIN_HIGH : PIN_LOW);

        /* Delay for at least the peer's setup time */
        // delay(SPI_SCLK_LOW_TIME);

        /* Pull the clock line high */
        digitalWrite(SPI_SCLK, PIN_HIGH);

        /* Shift-in a bit from the MISO line */
        if (digitalRead(SPI_MISO) == PIN_HIGH) {
            byte_in |= bit;
        }

        /* Delay for at least the peer's hold time */
        // delay(SPI_SCLK_HIGH_TIME);

        /* Pull the clock line low */
        digitalWrite(SPI_SCLK, PIN_LOW);
    }
    return byte_in;
}

int readSpiChannel(const int channel) {
  // (SGL/DIF = 1, D2,D1,D0=channel)
  const uint8_t data = 0x80 | ((channel & 0x7) << 4);

  initTransfer();
  transferByte(1);                             // start transfer byte
  int val = (transferByte(data) << 8) & 0x300; // 2 least significant received bits are the most significant val bits
  val    |=  transferByte(0);                  // received bits are the 8 least significant val bits
  terminateTransfer();

  return val;
}
