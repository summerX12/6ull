led.bin:led.s
	arm-none-eabi-gcc -g -c led.s -o led.o
	arm-none-eabi-ld -Ttext 0x80000000 led.o -o led.elf
	arm-none-eabi-objcopy -O binary -S -g led.elf led.bin
clean:
	rm -rf *.o *.elf *.bin
