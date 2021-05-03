CC = gcc
CFLAGS = -std=gnu11 -Werror -Wall -Wextra -O3 -fopenmp

.PHONY: all clean

all: serial parallel

clean:
	$(RM) serial parallel

parallel:
	$(CC) $(CFLAGS) task1_parallel.c -o parallel

serial:
	$(CC) $(CFLAGS) task1_serial.c -o serial
	