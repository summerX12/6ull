header_file := -fno-builtin -I$(shell pwd)/include
export header_file

all : start.o main.o device/device.o
	arm-none-eabi-ld -Tbase.lds $^ -o base.elf
	arm-none-eabi-objcopy -O binary -S -g base.elf base.bin

%.o : %.s
	arm-none-eabi-gcc -g -c $^

%.o : %.c
	arm-none-eabi-gcc $(header_file) -c $^

device/device.o:
	make -C device all

clean:
	make -C device clean
	rm -f *.o *.elf *.bin

