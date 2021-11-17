all: start.o led.o
	arm-none-eabi-ld -Tled.lds $^ -o led.elf
	arm-none-eabi-objcopy -O binary -S -g led.elf led.bin

%.o : %.s
	arm-none-eabi-gcc -g -c $^ -o start.o
%.o : %.c
	arm-none-eabi-gcc -g -c $^ -o led.o

clean:
	rm -rf *.o *.elf *.bin
