
#define MMIO_BASE       0x3F000000
#define GPIO_BASE       MMIO_BASE+0x00200000

#define GPFSEL0         ((volatile unsigned int*)(GPIO_BASE+0x00000000))
#define GPFSEL1         ((volatile unsigned int*)(GPIO_BASE+0x00000004))
#define GPFSEL2         ((volatile unsigned int*)(GPIO_BASE+0x00000008))
#define GPFSEL3         ((volatile unsigned int*)(GPIO_BASE+0x0000000C))
#define GPFSEL4         ((volatile unsigned int*)(GPIO_BASE+0x00000010))
#define GPFSEL5         ((volatile unsigned int*)(GPIO_BASE+0x00000014))
#define GPSET0          ((volatile unsigned int*)(GPIO_BASE+0x0000001C))
#define GPSET1          ((volatile unsigned int*)(GPIO_BASE+0x00000020))
#define GPCLR0          ((volatile unsigned int*)(GPIO_BASE+0x00000028))
#define GPCLR1          ((volatile unsigned int*)(GPIO_BASE+0x0000002C))
#define GPLEV0          ((volatile unsigned int*)(GPIO_BASE+0x00000034))
#define GPLEV1          ((volatile unsigned int*)(GPIO_BASE+0x00000038))
#define GPEDS0          ((volatile unsigned int*)(GPIO_BASE+0x00000040))
#define GPEDS1          ((volatile unsigned int*)(GPIO_BASE+0x00000044))
#define GPHEN0          ((volatile unsigned int*)(GPIO_BASE+0x00000064))
#define GPHEN1          ((volatile unsigned int*)(GPIO_BASE+0x00000068))
#define GPPUD           ((volatile unsigned int*)(GPIO_BASE+0x00000094))
#define GPPUDCLK0       ((volatile unsigned int*)(GPIO_BASE+0x00000098))
#define GPPUDCLK1       ((volatile unsigned int*)(GPIO_BASE+0x0000009C))

typedef enum {
  PIN_MODE_INPUT  = 0x0,
  PIN_MODE_OUTPUT = 0x1,
  PIN_MODE_ALT0   = 0x4
} PIN_MODE;

typedef enum {
  PIN_LOW  = 0,
  PIN_HIGH = 1
} PIN_STATE;

void pinMode(int pin, const PIN_MODE mode);

PIN_STATE digitalRead(const int pin);

void digitalWrite(const int pin, const PIN_STATE state);
