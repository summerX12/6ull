all : button.o led.o
	arm-none-eabi-ld -r $^ -o device.o

%.o : %.c
	arm-none-eabi-gcc ${header_file} -c $^

%.o : %.c
	arm-none-eabi-gcc ${header_file} -c $^

clean:
	rm -rf *.o *.bak
