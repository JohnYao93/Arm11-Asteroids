SRCS = $(wildcard *.c) $(wildcard input/*.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -O3 -ffast-math -funroll-loops -ftree-vectorize -ffreestanding -nostdinc -nostdlib -nostartfiles

all: kernel7.img clean

start.o: start.S
	arm-none-eabi-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

kernel7.img: start.o $(OBJS)
	arm-none-eabi-gcc -nostdlib -nostartfiles start.o $(OBJS) -l:libgcc.a -T link.ld -o kernel7.elf
	arm-none-eabi-objcopy -O binary kernel7.elf kernel7.img

clean:
	rm kernel7.elf *.o input/*.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel7.img -serial stdio
